#include "../src/audio.h"
#include "duk/duktape.h"
#include "jsLoop.h"
#include "jsAudio.h"
#include <boost/bind.hpp>




class jsAudio {
public:
	jsAudio(duk_context* ctx,boost::asio::io_service& io)
		:io_(io),
		ctx_(ctx),
		ref_(-1){
		worker = new boost::asio::io_service::work(io);
	}
	void stop() {
		pa.stop();
		delete worker;
	}
	void start() {
		pa.start();
	}
	int open(int channel,int type,int sampleRate,int framesPerBuffer) {
		bindRef();
		return pa.open(sampleRate, framesPerBuffer, recordCallback);
	}
	void onReaded(const void* buf, unsigned long len) {
		duk_eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
		duk_push_int(ctx_, ref_);
		duk_push_string(ctx_, "data");
		duk_push_external_buffer(ctx_);
		duk_config_buffer(ctx_, -1, (void*)buf, len);
		duk_push_int(ctx_, len);
		duk_pcall(ctx_, 4);
		duk_pop(ctx_);
	}
private:
	static int recordCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData)
	{
		jsAudio* pr = (jsAudio*)userData;
		pr->onReaded(inputBuffer, framesPerBuffer);
		return 0;
	}
	void bindRef() {
		duk_eval_string(ctx_, "(passoa_callbacks.push.bind(passoa_callbacks))");
		duk_dup(ctx_, -2);
		duk_pcall(ctx_, 1);
		if (duk_is_number(ctx_, -1)) {
			ref_ = duk_to_int(ctx_, -1);
		}
		duk_pop(ctx_);
	}
private:
	portAudio pa;
	boost::asio::io_service& io_;
	duk_context* ctx_;
	int ref_;
	boost::asio::io_service::work* worker;
};

int audioOpen(duk_context* ctx) {
	static jsAudio jsa(ctx, jsGetLoop());
	duk_push_pointer(ctx,&jsa);
	return 1;
}
int audioRecord(duk_context* ctx) {
	if (duk_is_pointer(ctx, 0)) {
		jsAudio* pr = (jsAudio*)duk_to_pointer(ctx, 0);
		pr->open(duk_to_int(ctx,1), duk_to_int(ctx, 2), duk_to_int(ctx, 3), duk_to_int(ctx, 4));
		pr->start();
	}
	return 0;
}
int audioStop(duk_context* ctx) {
	if (duk_is_pointer(ctx, 0)) {
		jsAudio* pr = (jsAudio*)duk_to_pointer(ctx,0);
		pr->stop();
	}
	return 0;
}