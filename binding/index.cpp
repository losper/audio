
#include <iostream>
#include "../src/audio.h"
#include "jsAudio.h"

struct pa_plugin gp;
static const pa_function_list_entry my_module_funcs[] = {
	{ "record", audioRecord, PA_VARARGS /*nargs*/ },
	{ "open", audioOpen, PA_VARARGS /*nargs*/ },
	{ "play", audioPlay, PA_VARARGS /*nargs*/ },
	{ "shutdown", audioShutdown, PA_VARARGS /*nargs*/ },
	{ NULL, NULL, 0 }
};
int passoa_init(pa_plugin p) {
    gp = p;
    gp.put_function_list(p.ctx, -1, my_module_funcs);
	portAudioInit();
	return 0;
}
int passoa_exit() {
	portAudioExit();
	return 0;
}