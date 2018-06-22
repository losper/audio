#include <boost/asio.hpp>
#include "portaudio/portaudio.h"

class portAudio {
public:
	portAudio();
	~portAudio();
	int open(double sampleRate, unsigned long framesPerBuffer, PaStreamCallback psc);
	int start();
	int stop();
private:
	PaStream *stream;
	PaError err_;
};