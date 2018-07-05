#include "../src/audio.h"
#include "duk/duktape.h"
#include "jsLoop.h"
#include "jsAudio.h"
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "../src/circularbuffer.hpp"
class jsAudioInterface {
public:
	static std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>& mgr() {
		static std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>> mgr;
		return mgr;
	};
	virtual int destroy() = 0;
	virtual int record(uint32_t ) = 0;
	virtual int play(void* p, uint32_t len) = 0;
};
template<typename T>
class jsAudio 
	:public boost::enable_shared_from_this<jsAudio<T>> 
	,public jsAudioInterface{
public:
	jsAudio<T>* attach() {
		mgr().insert(std::pair<jsAudio<T>*, boost::shared_ptr<jsAudioInterface>>(this,shared_from_this()));
		return this;
	}
	jsAudio(duk_context* ctx, boost::asio::io_service& io, uint32_t channel, uint32_t fmt, uint32_t sampleRate, unsigned long framesPerBuffer)
		:io_(io),
		ctx_(ctx),
		ref_(-1),
		samples(0),
		idx(0),
		max(0),
		sec(sec),
		channel(channel),
		fmt(fmt),
		sampleRate(sampleRate),
		framesPerBuffer(framesPerBuffer),
		elapsed(0),
		duration(0),
		cb(0),
		closed(1){
		bindRef();
		worker = new boost::asio::io_service::work(io);
		samples = new T[framesPerBuffer*channel];
	}
	~jsAudio() {
		freeRef();
		if (cb)delete cb;
		if(samples)delete[] samples;
		if (worker)delete worker;
	}
	int play(void* p, uint32_t len) {
		if (closed) {
			closed =pa.open(0, channel, fmt, sampleRate, framesPerBuffer, playCallback, this);
		}
		if (!closed) {
			if (!cb) {
				cb = new CircularBuffer<T>(sampleRate*channel*4);
			}
			cb->write((const T*)p, len/sizeof(T));
			if(finished){
				pa.start();
			}
		}
		return 0;
	}
	int destroy() {
		pa.destroy();
		io_.post(boost::bind(&jsAudio<T>::remove_this, this));
		return 0;
	}
	int record(uint32_t duration) {
		if (closed) {
			closed = pa.open(1, channel, fmt, sampleRate, framesPerBuffer, recordCallback, this);
		}
		if (!closed) {
			this->duration=duration*sampleRate;
			std::cout << "record!!!!" << std::endl;
			pa.start();
		}
		return 0;
	}
	int onWritten(const void* outputBuffer, unsigned long framesPerBuffer) {
		uint32_t framesLeft = cb->read(samples, framesPerBuffer*channel);
		T *rptr = samples;
		T *wptr = (T*)outputBuffer;
		unsigned int i;

		if (framesLeft < framesPerBuffer)
		{
			/* final buffer... */
			for (i = 0; i<framesLeft; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if (channel == 2) *wptr++ = *rptr++;  /* right */
			}
			for (; i<framesPerBuffer; i++)
			{
				*wptr++ = 0;  /* left */
				if (channel == 2) *wptr++ = 0;  /* right */
			}
			finished = paComplete;
		}
		else
		{
			for (i = 0; i<framesPerBuffer; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if (channel == 2) *wptr++ = *rptr++;  /* right */
			}
			finished = paContinue;
		}
		if (finished) {
			io_.post(boost::bind(&jsAudio::__onDone, this));
		}
		return finished;
	}
	int onReaded(const void* inputBuffer, unsigned long framesPerBuffer) {
		const T *rptr = (const T*)inputBuffer;
		T *wptr = samples;
		long framesToCalc;
		long i;
		
		unsigned long framesLeft =  duration - elapsed;

		if (framesLeft > framesPerBuffer)
		{
			framesToCalc = framesPerBuffer;
			finished = paContinue; 
		}
		else
		{
			framesToCalc = framesLeft;
			finished = paComplete;
		}

		if (inputBuffer == NULL)
		{
			for (i = 0; i<framesToCalc; i++)
			{
				*wptr++ = 0;  /* left */
				if (channel == 2) *wptr++ = 0;  /* right */
			}
		}
		else
		{
			for (i = 0; i<framesToCalc; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if (channel == 2) *wptr++ = *rptr++;  /* right */
			}
		}
		elapsed += framesToCalc;
		io_.post(boost::bind(&jsAudio::__onReaded, this, boost::shared_ptr<std::vector<T>>(new std::vector<T>(samples, samples + framesToCalc*channel))));
		if (finished) {
			elapsed = 0;
			io_.post(boost::bind(&jsAudio::__onDone, this));
		}
		return finished;
	}
private:
	static void remove_this(jsAudio<T>* pr) {
		std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>& m=mgr();
		std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it=m.find(pr);
		m.erase(it);
	}
	void __onDone() {
		duk_eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
		duk_push_int(ctx_, ref_);
		duk_push_string(ctx_, "done");
		duk_pcall(ctx_, 2);
		duk_pop(ctx_);
	}
	void __onReaded(boost::shared_ptr<std::vector<T>> tmp) {
		//duk_push_heap_stash
		duk_eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
		duk_push_int(ctx_, ref_);
		duk_push_string(ctx_, "data");
		duk_push_external_buffer(ctx_);
		
		duk_config_buffer(ctx_, -1, (void*)tmp->data(), tmp->size()*sizeof(T)/*max*channel*sizeof(T)*/);
		duk_pcall(ctx_, 3);
		duk_pop(ctx_);
	}
	static int recordCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
	{
		jsAudio* pr = (jsAudio*)userData;
		return pr->onReaded(inputBuffer, framesPerBuffer);
	}
	static int playCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
	{
		jsAudio* pr = (jsAudio*)userData;
		return pr->onWritten(outputBuffer, framesPerBuffer);
	}
	void bindRef() {
		freeRef();
		duk_eval_string(ctx_, "(passoa_callbacks.push.bind(passoa_callbacks))");
		duk_dup(ctx_, -2);
		duk_pcall(ctx_, 1);
		if (duk_is_number(ctx_, -1)) {
			ref_ = duk_to_int(ctx_, -1);
		}
		duk_pop(ctx_);
	}
	void freeRef() {
		if (ref_ != -1) {
			duk_eval_string(ctx_, "(passoa_callbacks.free.bind(passoa_callbacks))");
			duk_push_int(ctx_, ref_);
			duk_pcall(ctx_, 1);
			duk_pop(ctx_);
		}
	}
private:
	portAudio pa;
	boost::asio::io_service& io_;
	duk_context* ctx_;
	int ref_;
	boost::asio::io_service::work* worker;
	T* samples;
	CircularBuffer<T>* cb;
	uint32_t idx;
	uint32_t max;
	uint32_t sec;
	uint32_t channel;
	uint32_t fmt;
	uint32_t sampleRate;
	uint32_t framesPerBuffer;
	uint32_t rec_len;
	uint32_t duration;
	uint32_t elapsed;
	int closed;
	int finished;
};

int audioOpen(duk_context* ctx) {
	for (int i = 0; i < 4; i++) {
		if (!duk_is_number(ctx, i)){
			std::cout << duk_get_type(ctx, i) << std::endl;
			return 0;
		}
	}
	if (!duk_is_function(ctx, 4)) {
		return 0;
	}
	jsAudioInterface* jai = NULL;
	switch (duk_to_int(ctx, 1)) {
	case paInt32:
		{
			boost::shared_ptr<jsAudio<int32_t>> pr(new jsAudio<int32_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3))) ;
			jai = pr->attach();
		}
		break;
	case paInt16:
		{
			boost::shared_ptr<jsAudio<int16_t>> pr(new jsAudio<int16_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3)));
			jai = pr->attach();
		}
		break;
	case paInt8:
		{
			boost::shared_ptr<jsAudio<int8_t>> pr(new jsAudio<int8_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3)));
			jai = pr->attach();
		}
		break;
	case paFloat32:
		{
			boost::shared_ptr<jsAudio<float_t>> pr(new jsAudio<float_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3)));
			jai = pr->attach();
		}
		break;
	case paUInt8:
		{
			boost::shared_ptr<jsAudio<uint8_t>> pr(new jsAudio<uint8_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3)));
			jai = pr->attach();
		}
		break;
	default:
		break;
	}
	duk_push_pointer(ctx,jai);
	return 1;
}

int audioRecord(duk_context* ctx) {
	if (!duk_is_pointer(ctx,0) && !duk_is_number(ctx,1)) {
		return 0;
	}
	std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it=jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(duk_to_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->record(duk_to_uint(ctx,1));
	}
	return 0;
}
int audioPlay(duk_context* ctx) {
	if (!duk_is_pointer(ctx, 0) || !duk_is_buffer_data(ctx,1)) {
		return 0;
	}
	std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it = jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(duk_to_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		duk_size_t len = 0;
		void* buf=duk_require_buffer_data(ctx, 1, &len);
		it->second->play(buf,len);
	}
	return 0;
}
int audioDestroy(duk_context* ctx) {
	if (!duk_is_pointer(ctx, 0)) {
		return 0;
	}
	std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it = jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(duk_to_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->destroy();
	}
	return 0;
}