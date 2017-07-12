#include "decompress.h"
#include <iostream>
using std::cout;
using std::endl;

// returns 0. could be modified to return 1 on error.
int decompress(const uint8_t input[68], uint8_t output[64], int in_size)
{
	uint8_t dict[64];
	int dict_size = 0;
	int out_idx = 0;

	for(int i = 0; i < in_size; /**/) {
		bool bit1 = read_bit(input, i);
		bool bit2 = read_bit(input, i);

		if(!bit1 && !bit2) { // (00) zzzz
			output[out_idx++] = 0;
			output[out_idx++] = 0;
			output[out_idx++] = 0;
			output[out_idx++] = 0;
		} else if(!bit1 && bit2) { // (01) xxxx
			uint8_t a = read_byte(input, i);
			uint8_t b = read_byte(input, i);
			uint8_t c = read_byte(input, i);
			uint8_t d = read_byte(input, i);
			
			dict[dict_size++] = a;
			dict[dict_size++] = b;
			dict[dict_size++] = c;
			dict[dict_size++] = d;

			output[out_idx++] = a;
			output[out_idx++] = b;
			output[out_idx++] = c;
			output[out_idx++] = d;
		} else if(bit1 && !bit2) { // (10) mmmm
			int idx = read_idx(input, i); 

			output[out_idx++] = dict[4*idx];
			output[out_idx++] = dict[4*idx+1];
			output[out_idx++] = dict[4*idx+2];
			output[out_idx++] = dict[4*idx+3];
		} else {
			bool bit3 = read_bit(input, i);
			bool bit4 = read_bit(input, i);

			if(!bit3 && !bit4) { // (1100) mmxx
				int idx = read_idx(input, i);
				uint8_t c = read_byte(input, i);
				uint8_t d = read_byte(input, i);

				output[out_idx++] = dict[4*idx];
				output[out_idx++] = dict[4*idx+1];
				output[out_idx++] = c;
				output[out_idx++] = d;
			} else if(!bit3 && bit4) { // (1101) zzzx
				uint8_t d = read_byte(input, i);
				
				output[out_idx++] = 0;
				output[out_idx++] = 0;
				output[out_idx++] = 0;
				output[out_idx++] = d;
			} else { // (1110) mmmx
				int idx = read_idx(input, i);
				uint8_t d = read_byte(input, i);

				output[out_idx++] = dict[4*idx];
				output[out_idx++] = dict[4*idx+1];
				output[out_idx++] = dict[4*idx+2];
				output[out_idx++] = d;
			}
		}
	}

	return 0;
}

uint8_t read_bit(const uint8_t* input, int& i)
{
	if (input[i/8] & (1 << (i%8))) {
		i++;
		return 1;
	} else {
		i++;
		return 0;
	}
}

uint8_t read_byte(const uint8_t* input, int& i)
{
	uint8_t byte = 0;
	byte += 128*read_bit(input, i);
	byte +=  64*read_bit(input, i);
	byte +=  32*read_bit(input, i);
	byte +=  16*read_bit(input, i);
	byte +=   8*read_bit(input, i);
	byte +=   4*read_bit(input, i);
	byte +=   2*read_bit(input, i);
	byte +=   1*read_bit(input, i);

	return byte;
}

int read_idx(const uint8_t* input, int& i)
{
	int idx = 0;
	idx += 8*read_bit(input, i);
	idx += 4*read_bit(input, i);
	idx += 2*read_bit(input, i);
	idx += 1*read_bit(input, i);

	return idx;
}
