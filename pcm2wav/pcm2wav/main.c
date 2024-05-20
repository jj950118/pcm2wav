#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct WavHeader {
	//chunk
	char riff[4];  // "riff"
	unsigned int size;
	char wav[4];  // "WAVE"
	//subchunk1
	char fmt[4];  // "fmt "
	unsigned int fmt_size;
	unsigned short format;
	unsigned short channels;
	unsigned int sample_rate;
	unsigned int bytes_per_second;
	unsigned short block_size;
	unsigned short bit;
	//subchunk2
	char data[4];  // "data"
	unsigned int data_size;
}wavheader;

void main(int argc, void* argv[])
{
	FILE* fp_out = NULL;
	if (argc == 3)
	{
		fp_out = fopen("./default.wav", "wb");
	}
	else if (argc == 4)
	{
		fp_out = fopen(argv[3], "wb");
	}
	else
	{
		printf("Usage:\n");
		printf("1¡¢exe raw.pcm samplerate out.wav\n");
		printf("2¡¢exe raw.pcm samplerate(default.wav in current filedir\n");
		return -1;
	}
	FILE* fp_in = fopen(argv[1], "rb");

	wavheader header;
	fwrite(&header, sizeof(wavheader), 1, fp_out);
	int count = 0;
	int frame = 512;
	int remain = 0;
	short buff[512];
	while (1)
	{
		remain = fread(buff, sizeof(short), frame, fp_in);
		if (remain == frame)
		{
			fwrite(buff, sizeof(short), frame, fp_out);
			count += 1;
		}
		else
		{
			break;
		}
	}

	size_t farnum = 0;
	header.data_size = (count * frame + remain) * 2;//len(int16pcm) * 2
	char header_riff[4] = { 'R','I','F','F' };
	char header_fmt[4] = { 'f','m','t',' ' };
	char header_data[4] = { 'd','a','t','a' };
	char header_wave[4] = { 'W','A','V','E' };
	memcpy(header.riff, header_riff, 4);
	memcpy(header.fmt, header_fmt, 4);
	memcpy(header.data, header_data, 4);
	memcpy(header.wav, header_wave, 4);
	header.fmt_size = 16;
	header.bit = 16;
	header.channels = 1;
	header.format = 1;
	header.sample_rate = atoi(argv[2]);
	header.size = header.data_size + 36;
	header.block_size = header.channels * header.bit / 8;
	header.bytes_per_second = header.sample_rate * header.block_size;
	fseek(fp_out, 0, SEEK_SET);
	fwrite(&header, sizeof(wavheader), 1, fp_out);
	fclose(fp_out);
}