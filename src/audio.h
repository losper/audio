#include "portaudio.h"
#include <stdint.h>

class portAudio {
public:
	portAudio();
	~portAudio();
	int open(int direction, uint32_t channel, uint32_t pmt, uint32_t sampleRate, unsigned long framesPerBuffer, PaStreamCallback psc,void* userData);
	int start();
	int stop();
	int destroy();
private:
	PaStream *stream;
	PaError err_;
};
int portAudioInit();
int portAudioExit();
