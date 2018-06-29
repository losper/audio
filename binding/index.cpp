#include "duk/duktape.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "../src/audio.h"
#include "jsLoop.h"
#include "jsAudio.h"

static const duk_function_list_entry my_module_funcs[] = {
	{ "record", audioRecord, DUK_VARARGS /*nargs*/ },
	{ "open", audioOpen, DUK_VARARGS /*nargs*/ },
	{ "play", audioPlay, DUK_VARARGS /*nargs*/ },
	{ "resume", audioResume, DUK_VARARGS /*nargs*/ },
	{ NULL, NULL, 0 }
};
int passoa_init(duk_context* ctx,boost::asio::io_service* io_) {
	jsSetLoop(io_);
	duk_put_function_list(ctx, -1, my_module_funcs);
	portAudioInit();
	return 0;
}
int passoa_exit() {
	portAudioExit();
	return 0;
}