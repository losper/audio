#include "plugin.h"

int audioOpen(pa_context* ctx);
int audioRecord(pa_context* ctx);
int audioPlay(pa_context* ctx);
int audioShutdown(pa_context* ctx);
int audioFingerprint(pa_context* ctx);