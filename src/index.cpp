#include "wav.h"
int main(int argc,char** argv) {
	TWav wv(argv[1]);
	int16_t sample = 0;
	uint32_t num = wv.wav_info.size2 / wv.wav_info.block_align;
	for (size_t i = 0; i < num; i++)
	{
		sample = wv.GetSample();
		printf("%d ", sample);
	}
	return 0;
}