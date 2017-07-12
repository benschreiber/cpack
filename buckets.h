#include <iostream>
using std::cout;
using std::endl;

void updateBuckets(long bucket[10], size_t size) {
	if(size > 64*8) {bucket[0]++;}
	else if(size == 64*8) {bucket[1]++;}
	else if(size >= 64*7) {bucket[2]++;}
	else if(size >= 64*6) {bucket[3]++;}
	else if(size >= 64*5) {bucket[4]++;}
	else if(size >= 64*4) {bucket[5]++;}
	else if(size >= 64*3) {bucket[6]++;}
	else if(size >= 64*2) {bucket[7]++;}
	else if(size >= 64*1) {bucket[8]++;}
	else {bucket[9]++;}
}

void printBuckets(const long bucket[10]) {
	cout << "Histogram of lines at each compression level" << endl;
	cout << "Percentages are compressed size / uncompressed size" << endl;
	cout << "Lower ratios are better" << endl;
	cout << ">=  100% - " << bucket[0] + bucket[1] << endl;
	cout << ">= 87.5% - " << bucket[2] << endl; 
	cout << ">= 75.0% - " << bucket[3] << endl; 
	cout << ">= 62.5% - " << bucket[4] << endl; 
	cout << ">= 50.0% - " << bucket[5] << endl; 
	cout << ">= 37.5% - " << bucket[6] << endl; 
	cout << ">= 25.0% - " << bucket[7] << endl; 
	cout << ">= 12.5% - " << bucket[8] << endl; 
	cout << ">= 00.0% - " << bucket[9] << endl; 
}
