#include "wav.h"
#define WAV_TEST
TWav::TWav(const char *filename) {
	fp = fopen(filename, "rb");
	assert(fp != NULL);
	fread(wav_info.riff_id, 1, 4, fp);
	wav_info.riff_id[4] = '\0';
	fread(&wav_info.size0, 4, 1, fp);
	fread(wav_info.wave_fmt, 1, 8, fp);
	wav_info.wave_fmt[8] = '\0';
	fread(&wav_info.size1, 4, 1, fp);
	fread(&wav_info.fmttag, 2, 1, fp);
	fread(&wav_info.channel, 2, 1, fp);
	fread(&wav_info.sample_per_sec, 4, 1, fp);
	fread(&wav_info.byte_per_sec, 4, 1, fp);
	fread(&wav_info.block_align, 2, 1, fp);
	fread(&wav_info.bit_per_sample, 2, 1, fp);
	if (wav_info.size1 == 0x10) {
		fread(wav_info.data_id, 1, 4, fp);
		wav_info.data_id[4] = '\0';
		fread(&wav_info.size2, 4, 1, fp);
	}
	else {
		fseek(fp, 6, SEEK_CUR);
		uint32_t offset;
		fread(&offset, 4, 1, fp);
		fseek(fp, offset, SEEK_CUR);
		fread(wav_info.data_id, 1, 4, fp);
		wav_info.data_id[4] = '\0';
		fread(&wav_info.size2, 4, 1, fp);
	}
	data_offset = wav_info.size0 + 8 - wav_info.size2;

#ifdef WAV_TEST
	printf("=====test file header=====\n");
	printf("riff_id        %-8s �ļ�ͷ��־\"RIFF\"\n", wav_info.riff_id);
	printf("size0          %-8d �ļ��ܳ�-8\n", wav_info.size0);
	printf("wave_fmt       %-8s �ļ�ͷ��־\"WAVEfmt \"\n", wav_info.wave_fmt);
	printf("size1          %-8s �����ļ�/��׼�ļ�\n", (wav_info.size1 == 0x12) ? "�����ļ�" : "��׼�ļ�");
	printf("fmttag         %-8d \n", wav_info.fmttag);
	printf("channel        %-8d ������\n", wav_info.channel);
	printf("sample_per_sec %-8d ������\n", wav_info.sample_per_sec);
	printf("byte_per_sec   %-8d ÿ�벥���ֽ���\n", wav_info.byte_per_sec);
	printf("block_align    %-8d ����һ��ռ�ֽ���\n", wav_info.block_align);
	printf("bit_per_sample %-8d ������\n", wav_info.bit_per_sample);
	printf("data_id        %-8s \"data\"��־\n", wav_info.data_id);
	printf("size2          %-8d ���������ֽ���\n", wav_info.size2);
	printf("=========================\n");
	printf("����ʱ��:      %d sec\n", wav_info.size2 / wav_info.byte_per_sec);
#endif
}

TWav::~TWav() {
	fclose(fp);
}

int16_t TWav::GetSample() {
	int16_t s1;
	fread(&s1, 2, 1, fp);
	//FIXME check file end
	return s1;
}

void TWav::StartFromOffset(int offset) {
	fseek(fp, data_offset + offset, SEEK_SET);
}
