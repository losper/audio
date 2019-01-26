
#include "audio.h"
#include <iostream>

portAudio::~portAudio() {
}
int portAudio::open(int direction, uint32_t channel, uint32_t pmt,uint32_t sampleRate,unsigned long framesPerBuffer,PaStreamCallback psc,void* userData) {
	/* Open an audio I/O stream. */
	PaStreamParameters  parameters;
	if (direction) {
		parameters.device = Pa_GetDefaultInputDevice(); /* default input device */
		if (parameters.device == paNoDevice) {
			fprintf(stderr, "Error: No default input device.\n");
			return -1;
		}
		
		parameters.channelCount = channel;                    /* stereo input */
		parameters.sampleFormat = pmt;
		parameters.suggestedLatency = Pa_GetDeviceInfo(parameters.device)->defaultLowInputLatency;
		parameters.hostApiSpecificStreamInfo = NULL;
		
		err_ = Pa_OpenStream(
			&stream,
			&parameters,
			NULL,                  /* &outputParameters, */
			sampleRate,
			framesPerBuffer,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			psc,
			userData);
	}
	else {
		parameters.device = Pa_GetDefaultOutputDevice(); /* default input device */
		if (parameters.device == paNoDevice) {
			fprintf(stderr, "Error: No default output device.\n");
			return -1;
		}

		parameters.channelCount = channel;                    /* stereo input */
		parameters.sampleFormat = pmt;
		parameters.suggestedLatency = Pa_GetDeviceInfo(parameters.device)->defaultLowOutputLatency;
		parameters.hostApiSpecificStreamInfo = NULL;

		err_ = Pa_OpenStream(
			&stream,
			NULL,                  /* &outputParameters, */
			&parameters,
			sampleRate,
			framesPerBuffer,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			psc,
			userData);
	}
	
	if (err_ != paNoError) std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudio::start() {
	for (int i = 0; i < 100;i++){
		if (Pa_IsStreamActive(stream)) {
			Pa_Sleep(10);
		}
		else {
			break;
		}
	}
	if (!Pa_IsStreamStopped(stream)) {
		stop();
	}
	err_ = Pa_StartStream(stream);
	if (err_ != paNoError) std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudio::stop() {
	
	err_ = Pa_StopStream(stream);
	if (err_ != paNoError)
		std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudio::destroy()
{
	Pa_CloseStream(stream);
	return 0;
}
portAudio::portAudio() {
}

int portAudioInit() {
	PaError err_ = Pa_Initialize();
	if (err_ != paNoError)
		std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}
int portAudioExit() {
	PaError err_ = Pa_Terminate();
	if (err_ != paNoError)
		std::cout << "PortAudio error: " << Pa_GetErrorText(err_) << std::endl;
	return 0;
}