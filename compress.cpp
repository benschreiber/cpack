#include "compress.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

// returns size of compressed line in bytes
int compress(const uint8_t input[64], uint8_t output[68])
{
	int out_idx = 0;
	uint8_t dict[64] = {0};
	int dict_size = 0;

	for(int i = 0; i < 16; i++) {
		assert(out_idx < 544);

		bool found = false;

		uint8_t a = input[4*i];
		uint8_t b = input[4*i+1];
		uint8_t c = input[4*i+2];
		uint8_t d = input[4*i+3];

		// check against zero patterns
		if(a == 0 && b == 0 && c == 0) {
			if (d == 0) { // pattern zzzz ouput (00)
				set_bit(output, out_idx, 0, 0);
			} else { // pattern zzzx output (1101)B
				set_bit(output, out_idx, 1, 1, 0, 1);
				set_byte(output, out_idx, d);
			}
			continue;
		}

		// check against dictionary
		for(int j = 0; j < dict_size; j++) {	
			if(a == dict[4*j] && b == dict[4*j+1]) {
				if(c == dict[4*j+2]) {
					if(d == dict[4*j+3]) { // pattern mmmm output (10) bbbb
						set_bit(output, out_idx, 1, 0);
						set_idx(output, out_idx, j);
					} else { // pattern mmmx output (1110)bbbbB
						set_bit(output, out_idx, 1, 1, 1, 0);
						set_idx(output, out_idx, j);
						set_byte(output, out_idx, d);
					}
				} else { // pattern mmxx output (1100)bbbbBB
					set_bit(output, out_idx, 1, 1, 0, 0);
					set_idx(output, out_idx, j);
					set_byte(output, out_idx, c);
					set_byte(output, out_idx, d);
				}
				found = true;
				break;
			}
		}

		if(found) {
			continue;
		}

		// pattern xxxx output (01)BBBB
		set_bit(output, out_idx, 0, 1);
		set_byte(output, out_idx, a);
		set_byte(output, out_idx, b);
		set_byte(output, out_idx, c);
		set_byte(output, out_idx, d);

		// add new pattern to dictionary
		dict[4*dict_size] = a;
		dict[4*dict_size+1] = b;
		dict[4*dict_size+2] = c;
		dict[4*dict_size+3] = d;

		dict_size += 1;
	}

	return out_idx;
}

void set_bit(uint8_t* output, int& idx, bool a, bool b)
{
	set_bit(output, idx, a);
	set_bit(output, idx, b);
}

void set_bit(uint8_t* output, int& idx, bool a, bool b, bool c, bool d)
{
	set_bit(output, idx, a);
	set_bit(output, idx, b);
	set_bit(output, idx, c);
	set_bit(output, idx, d);
}


void set_byte(uint8_t* output, int& idx, uint8_t byte)
{
	set_bit(output, idx, byte & 128);
	set_bit(output, idx, byte & 64);
	set_bit(output, idx, byte & 32);
	set_bit(output, idx, byte & 16);
	set_bit(output, idx, byte & 8);
	set_bit(output, idx, byte & 4);
	set_bit(output, idx, byte & 2);
	set_bit(output, idx, byte & 1);
}

void set_idx(uint8_t* output, int& out_idx, int idx)
{
	set_bit(output, out_idx, idx & 8);
	set_bit(output, out_idx, idx & 4);
	set_bit(output, out_idx, idx & 2);
	set_bit(output, out_idx, idx & 1);
}

void set_bit(uint8_t* output, int& idx, bool a)
{
	if(a == 0) {
		output[idx/8] &= ~(1 << (idx%8));
	} else {
		output[idx/8] |= 1 << (idx%8);
	}

	idx++;
}
