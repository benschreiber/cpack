#include <cstdint>

int compress(const uint8_t* input, uint8_t* output);

void set_bit(uint8_t*, int&, bool);
void set_bit(uint8_t*, int&, bool, bool);
void set_bit(uint8_t*, int&, bool, bool, bool, bool);

void set_byte(uint8_t*, int&, uint8_t);
void set_idx(uint8_t*, int&, int);
