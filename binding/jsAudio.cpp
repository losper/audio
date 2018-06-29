#include "../src/audio.h"
#include "duk/duktape.h"
#include "jsLoop.h"
#include "jsAudio.h"
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
class jsAudioInterface {
public:
	static std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>& mgr() {
		static std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>> mgr;
		return mgr;
	};
	virtual int resume() = 0;
	virtual int record() = 0;
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
	jsAudio(duk_context* ctx, boost::asio::io_service& io, int sec, uint32_t channel, uint32_t fmt, uint32_t sampleRate, unsigned long framesPerBuffer)
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
		closed(1){
		bindRef();
		worker = new boost::asio::io_service::work(io);
		max = sec*sampleRate;
		samples = new T[max*channel];
	}
	~jsAudio() {
		freeRef();
		delete samples;
		delete worker;
	}
	void stop() {
		pa.stop();
	}
	void start() {
		pa.start();
	}
	int play(void* p, uint32_t len) {
		if (closed) {
			closed =pa.open(0, channel, fmt, sampleRate, framesPerBuffer, playCallback, this);
		}
		if (!closed) {
			
			uint32_t bytes=max*channel * sizeof(T);
			if (len > bytes) {
				len = bytes;
			}
			memcpy(samples, p, len);
			pa.start();
		}
		return 0;
	}
	int resume() {
		pa.stop();
		pa.start();
		return 0;
	}
	int record() {
		if (closed) {
			closed = pa.open(1, channel, fmt, sampleRate, framesPerBuffer, recordCallback, this);
		}
		if (!closed) {
			pa.start();
		}
		return 0;
	}
	int onWritten(const void* outputBuffer, unsigned long framesPerBuffer) {
		T *rptr = &samples[idx * channel];
		T *wptr = (T*)outputBuffer;
		unsigned int i;
		int finished;
		unsigned int framesLeft = max - idx;

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
			idx += framesLeft;
			finished = paComplete;
		}
		else
		{
			for (i = 0; i<framesPerBuffer; i++)
			{
				*wptr++ = *rptr++;  /* left */
				if (channel == 2) *wptr++ = *rptr++;  /* right */
			}
			idx += framesPerBuffer;
			finished = paContinue;
		}
		if (finished) {
			io_.post(boost::bind(&jsAudio::__onWritten, this));
			idx = 0;
		}
		return finished;
	}
	int onReaded(const void* inputBuffer, unsigned long framesPerBuffer) {
		const T *rptr = (const T*)inputBuffer;
		T *wptr = &samples[idx * channel];
		long framesToCalc;
		long i;
		int finished;
		unsigned long framesLeft = max - idx;

		if (framesLeft < framesPerBuffer)
		{
			framesToCalc = framesLeft;
			finished = paComplete;
		}
		else
		{
			framesToCalc = framesPerBuffer;
			finished = paContinue;
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
		idx += framesToCalc;
		if (finished) {
			io_.dispatch(boost::bind(&jsAudio::__onReaded, this));
			idx = 0;
		}
		return finished;
	}
private:
	static void remove_this(jsAudio<T>* pr) {
		std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>& m=mgr();
		std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it=m.find(pr);
		m.erase(it);
	}
	void __onWritten() {
		duk_eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
		duk_push_int(ctx_, ref_);
		duk_push_string(ctx_, "done");
		duk_pcall(ctx_, 2);
		duk_pop(ctx_);
	}
	void __onReaded() {
		duk_eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
		duk_push_int(ctx_, ref_);
		duk_push_string(ctx_, "data");
		duk_push_external_buffer(ctx_);
		duk_config_buffer(ctx_, -1, (void*)samples, max*channel*sizeof(T));
		duk_pcall(ctx_, 3);
		/*if (duk_is_number(ctx_, -1) && 0 == duk_to_int(ctx_, -1)) {
			pa.stop();
			pa.start();
		}
		else {
			pa.stop();
			io_.post(boost::bind(&jsAudio<T>::remove_this,this));
		}*/
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
	uint32_t idx;
	uint32_t max;
	uint32_t sec;
	uint32_t channel;
	uint32_t fmt;
	uint32_t sampleRate;
	uint32_t framesPerBuffer;
	int closed;
};

int audioOpen(duk_context* ctx) {
	for (int i = 0; i < 5; i++) {
		if (!duk_is_number(ctx, i)){
			std::cout << duk_get_type(ctx, i) << std::endl;
			return 0;
		}
	}
	if (!duk_is_function(ctx, 5)) {
		return 0;
	}
	jsAudioInterface* jai = NULL;
	switch (duk_to_int(ctx, 2)) {
	case paInt32:
		{
			boost::shared_ptr<jsAudio<int32_t>> pr(new jsAudio<int32_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4))) ;
			jai = pr->attach();
		}
		break;
	case paInt16:
		{
			boost::shared_ptr<jsAudio<int16_t>> pr(new jsAudio<int16_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4)));
			jai = pr->attach();
		}
		break;
	case paInt8:
		{
			boost::shared_ptr<jsAudio<int8_t>> pr(new jsAudio<int8_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4)));
			jai = pr->attach();
		}
		break;
	case paFloat32:
		{
			boost::shared_ptr<jsAudio<float_t>> pr(new jsAudio<float_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4)));
			jai = pr->attach();
		}
		break;
	case paUInt8:
		{
			boost::shared_ptr<jsAudio<uint8_t>> pr(new jsAudio<uint8_t>(ctx, jsGetLoop(), duk_to_int(ctx, 0), duk_to_int(ctx, 1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4)));
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
	if (!duk_is_pointer(ctx,0)) {
		return 0;
	}
	std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it=jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(duk_to_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->record();
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
int audioResume(duk_context* ctx) {
	if (!duk_is_pointer(ctx, 0)) {
		return 0;
	}
	std::map<jsAudioInterface*, boost::shared_ptr<jsAudioInterface>>::iterator it = jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(duk_to_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->resume();
	}
	return 0;
}