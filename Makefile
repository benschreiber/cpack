all:
	clang++ main.cpp decompress.cpp compress.cpp -std=c++11 -g -o cpack -O3
