#include "portaudio/portaudio.h"
#include "audio.h"
#include <iostream>
#include <boost/bind.hpp>

int recordCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	return 0;
}
class portAudio {
public:
	~portAudio() {
		err_ = Pa_Terminate();
		if (err_ != paNoError)
			std::cout << "PortAudio error: "<< Pa_GetErrorText(err_) << std::endl;
	}
	int open(rate,input) {
		PaStream *stream;
		PaError err;
		/* Open an audio I/O stream. */
		err = Pa_OpenDefaultStream(&stream,
			2,          /* no input channels */
			0,          /* stereo output */
			paFloat32,  /* 32 bit floating point output */
			SAMPLE_RATE,
			256,        /* frames per buffer, i.e. the number
						of sample frames that PortAudio will
						request from the callback. Many apps
						may want to use
						paFramesPerBufferUnspecified, which
						tells PortAudio to pick the best,
						possibly changing, buffer size.*/
			recordCallback, /* this is your callback function */
			this); /*This is a pointer that will be passed to
					your callback*/
		if (err != paNoError) std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	}
private:
	
	portAudio() {
		 err_ = Pa_Initialize();
		 if (err_ != paNoError)
			 std::cout << "PortAudio error: "<< Pa_GetErrorText(err_) << std::endl;
	}
	PaError err_;
};