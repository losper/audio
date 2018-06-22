
#include "audio.h"
#include <iostream>

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;


portAudio::~portAudio() {
	err_ = Pa_Terminate();
	if (err_ != paNoError)
		std::cout << "PortAudio error: "<< Pa_GetErrorText(err_) << std::endl;
}
int portAudio::open(double sampleRate,unsigned long framesPerBuffer,PaStreamCallback psc) {
	/* Open an audio I/O stream. */
	PaStreamParameters  inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default input device.\n");
		return -1;
	}
	inputParameters.channelCount = 1;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	err_ = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		sampleRate,
		framesPerBuffer,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		psc,
		this);

	if (err_ != paNoError) std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudio::start() {
	err_ = Pa_StartStream(stream);
	if (err_ != paNoError) std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudio::stop() {
	err_ = Pa_Terminate();
	if (err_ != paNoError)
		std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
portAudio::portAudio() {
	err_ = Pa_Initialize();
	if (err_ != paNoError)
		std::cout << "PortAudio error: "<< Pa_GetErrorText(err_) << std::endl;
}

