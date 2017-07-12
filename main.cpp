#include <fstream>
#include <iterator>
#include <iostream>
#include "compress.h"
#include "decompress.h"
#include "buckets.h"

/*
 * An Implementation of C-Pack
 * by Ben Schreiber 2/17 (modified 7/17)
 *
 * C-Pack is a cache line compression algorithm
 * described by a paper by Chen, Yang, Dick, Shang, and Lekatsas.
 * This implementation is primarily for testing its performance
 * on arbitrary binary input files.
 * 
 * see http://ieeexplore.ieee.org/document/5229354/
 */

int main(int argc, char ** argv)
{

	if (argc != 2) {
		std::cerr << "Need filename" << std::endl;
		return -1;
	}
	
	// enable check against decompress
	bool CHECK_ERR = true;

	// file i/o setup
	std::string file(argv[1]);
	std::ifstream input(file, std::ios::in | std::ios::binary | std::ios::ate);

	std::streampos size;

	size = input.tellg();
	input.seekg(0, std::ios::beg);

	// running totals for file 
	double input_size = 0;
	double output_size = 0;
	long buckets[10] = {0};
	
	// in worst case, output is actually slightly larger than input
	uint8_t output[68];
	uint8_t decomp[64];

	while(size >= 64) {
		char memblock[64];
		input.read(memblock, 64);

		double compsize = compress((uint8_t*)memblock, output);
		
		if(CHECK_ERR) {
			decompress(output, decomp, compsize);

			// check decompressed block against original
			for(int i = 0; i < 64; i++)
				if((uint8_t)memblock[i] != decomp[i])
					std::cout << "error" << std::endl;
		}

		input_size += 64*8;
		output_size += compsize;

		// put compression ratio into histogram
		updateBuckets(buckets, compsize);

		// note: trailing data less than 64 bytes is not compressed
		// this should minimally impact perf metrics
		size -= 64;
	}

	std::cout << "ratio: " << output_size / input_size << std::endl;
	printBuckets(buckets);
	
	return 0;
}	


