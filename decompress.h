#include <cstdint>

int decompress(const uint8_t input[68], uint8_t output[64], int in_size);

uint8_t read_bit(const uint8_t* input, int& i);
uint8_t read_byte(const uint8_t* input, int& i);
int read_idx(const uint8_t* input, int& i);
	
