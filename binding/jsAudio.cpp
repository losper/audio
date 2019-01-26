
#include "jsAudio.h"
#include <iostream>
#include <vector>
#include <functional>
#include "../src/circularbuffer.hpp"
#include <map>
#include <memory>
#include "../src/audio.h"

extern pa_plugin gp;

class jsAudioInterface {
public:
    static std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>& mgr() {
        static std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>> mgr;
        return mgr;
    };
    virtual int shutdown() = 0;
    virtual int record(uint32_t) = 0;
    virtual int play(void* p, uint32_t len) = 0;
};
template<typename T>
class jsAudio
    :public std::enable_shared_from_this<jsAudio<T>>
    , public jsAudioInterface {
public:
    jsAudio<T>* attach() {
        mgr().insert(std::pair<jsAudio<T>*, std::shared_ptr<jsAudioInterface>>(this, shared_from_this()));
        return this;
    }
    jsAudio(pa_context* ctx, uint32_t channel, uint32_t fmt, uint32_t sampleRate, unsigned long framesPerBuffer)
        :ctx_(ctx),
        ref_(-1),
        samples(0),
        channel(channel),
        fmt(fmt),
        sampleRate(sampleRate),
        framesPerBuffer(framesPerBuffer),
        elapsed(0),
        duration(0),
        cb(0),
        closed(1),
        shuting(0),
        finished(1) {
        bindRef();
        samples = new T[framesPerBuffer*channel];
        g_ioptr = gp.keep_io();
    }
    ~jsAudio() {
        freeRef();
        if (cb)delete cb;
        if (samples)delete[] samples;
        gp.release_io(g_ioptr);
    }
    int play(void* p, uint32_t len) {
        uint32_t ret = 0;
        if (closed) {
            closed = pa.open(0, channel, fmt, sampleRate, framesPerBuffer, playCallback, this);
        }
        if (!closed) {
            if (!cb) {
                cb = new CircularBuffer<T>(sampleRate*channel * 4);
            }
            ret = cb->write((const T*)p, len / sizeof(T)) * sizeof(T);
            if (finished) {
                finished = 0;
                pa.start();
            }
        }
        drain = (ret<len);
        return ret;
    }
    int shutdown() {
        shuting = 1;
        return 0;
    }
    int destroy() {
        if (shuting) {
            pa.destroy();
            bind_io(gp,std::bind(&jsAudio<T>::remove_this, this));
        }
        return 0;
    }
    int record(uint32_t duration) {
        if (closed) {
            closed = pa.open(1, channel, fmt, sampleRate, framesPerBuffer, recordCallback, this);
        }
        if (!closed) {
            this->duration = duration*sampleRate;
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
        if (finished) {
            bind_io(gp,std::bind(&jsAudio::__onEvent, this, "done"));
            bind_io(gp,std::bind(&jsAudio::destroy, this));
            return finished;
        }
        if (!shuting && drain && (cb->size() < (cb->capacity() / 4))) {
            bind_io(gp,std::bind(&jsAudio::__onEvent, this, "drain"));
        }
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
        elapsed += framesLeft;

        return 0;
    }
    int onReaded(const void* inputBuffer, unsigned long framesPerBuffer) {
        const T *rptr = (const T*)inputBuffer;
        T *wptr = samples;
        long framesToCalc;
        long i;

        unsigned long framesLeft = duration - elapsed;

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
        bind_io(gp,std::bind(&jsAudio::__onReaded, this, std::shared_ptr<std::vector<T>>(new std::vector<T>(samples, samples + framesToCalc*channel))));
        if (finished) {
            elapsed = 0;

            bind_io(gp,std::bind(&jsAudio::__onEvent, this, "done"));
            bind_io(gp,std::bind(&jsAudio::destroy, this));
        }
        return finished;
    }
private:
    static void remove_this(jsAudio<T>* pr) {
        std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>& m = mgr();
        std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>::iterator it = m.find(pr);
        m.erase(it);
    }
    void __onEvent(const char* ev) {
        gp.eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
        gp.push_int(ctx_, ref_);
        gp.push_string(ctx_, ev);
        gp.call(ctx_, 2);
        gp.pop(ctx_);
    }
    void __onReaded(std::shared_ptr<std::vector<T>> tmp) {
        //gp.push_heap_stash
        gp.eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
        gp.push_int(ctx_, ref_);
        gp.push_string(ctx_, "data");
        gp.push_external_buffer(ctx_, (void*)tmp->data(), tmp->size() * sizeof(T));
        gp.call(ctx_, 3);
        gp.pop(ctx_);
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
        gp.eval_string(ctx_, "(passoa_callbacks.push.bind(passoa_callbacks))");
        gp.dup(ctx_, -2);
        gp.call(ctx_, 1);
        if (gp.is_number(ctx_, -1)) {
            ref_ = gp.get_int(ctx_, -1);
        }
        gp.pop(ctx_);
    }
    void freeRef() {
        if (ref_ != -1) {
            gp.eval_string(ctx_, "(passoa_callbacks.free.bind(passoa_callbacks))");
            gp.push_int(ctx_, ref_);
            gp.call(ctx_, 1);
            gp.pop(ctx_);
        }
    }
private:
    portAudio pa;
    pa_context* ctx_;
    int ref_;
    T* samples;
    CircularBuffer<T>* cb;
    uint32_t channel;
    uint32_t fmt;
    uint32_t sampleRate;
    uint32_t framesPerBuffer;
    uint32_t duration;
    uint32_t elapsed;
    int closed;
    int finished;
    int drain;
    int shuting;
    void* g_ioptr;
};

int audioOpen(pa_context* ctx) {
    for (int i = 0; i < 4; i++) {
    	if (!gp.is_number(ctx, i)){
    		std::cout << gp.get_type(ctx, i) << std::endl;
    		return 0;
    	}
    }
    if (!gp.is_function(ctx, 4)) {
    	return 0;
    }
    jsAudioInterface* jai = NULL;
    switch (gp.get_int(ctx, 1)) {
    case paInt32:
    	{
    		auto pr=std::make_shared<jsAudio<int32_t>>(
                ctx,
                gp.get_int(ctx, 0),
                gp.get_int(ctx, 1),
                gp.get_int(ctx, 2),
                gp.get_int(ctx, 3)) ;
    		jai = pr->attach();
    	}
    	break;
    case paInt16:
    	{
            auto pr = std::make_shared<jsAudio<int16_t>>(
                ctx,
                gp.get_int(ctx, 0),
                gp.get_int(ctx, 1),
                gp.get_int(ctx, 2),
                gp.get_int(ctx, 3));
            jai = pr->attach();
    	}
    	break;
    case paInt8:
    	{
            auto pr = std::make_shared<jsAudio<int8_t>>(
                ctx,
                gp.get_int(ctx, 0),
                gp.get_int(ctx, 1),
                gp.get_int(ctx, 2),
                gp.get_int(ctx, 3));
            jai = pr->attach();
    	}
    	break;
    case paFloat32:
    	{
            auto pr = std::make_shared<jsAudio<float>>(
                ctx,
                gp.get_int(ctx, 0),
                gp.get_int(ctx, 1),
                gp.get_int(ctx, 2),
                gp.get_int(ctx, 3));
            jai = pr->attach();
    	}
    	break;
    case paUInt8:
    	{
            auto pr = std::make_shared<jsAudio<uint8_t>>(
                ctx,
                gp.get_int(ctx, 0),
                gp.get_int(ctx, 1),
                gp.get_int(ctx, 2),
                gp.get_int(ctx, 3));
            jai = pr->attach();
    	}
    	break;
    default:
    	break;
    }
    gp.push_pointer(ctx,jai);
    return 1;
}

int audioRecord(pa_context* ctx) {
	if (!gp.is_pointer(ctx,0) && !gp.is_number(ctx,1)) {
		return 0;
	}
	std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>::iterator it=jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(gp.get_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->record(gp.get_int(ctx,1));
	}
	return 0;
}
int audioPlay(pa_context* ctx) {
	if (!gp.is_pointer(ctx, 0) || !gp.is_buffer_data(ctx,1)) {
		gp.push_uint(ctx,0);
		return 1;
	}
	std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>::iterator it = jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(gp.get_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		int len = 0;
		void* buf=gp.get_buffer_data(ctx, 1, &len);
		gp.push_uint(ctx,it->second->play(buf,len));
	}
	return 1;
}
int audioShutdown(pa_context* ctx) {
	if (!gp.is_pointer(ctx, 0)) {
		return 0;
	}
	std::map<jsAudioInterface*, std::shared_ptr<jsAudioInterface>>::iterator it = jsAudioInterface::mgr().find(static_cast<jsAudioInterface*>(gp.get_pointer(ctx, 0)));
	if (it != jsAudioInterface::mgr().end()) {
		it->second->shutdown();
	}
	return 0;
}
