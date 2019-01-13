#include "MainHeader.h"

void operator>>(std::ifstream &file, BigInt &a) {
	std::string str;

	file >> str;
	a = str;
}

void operator<<(std::ofstream &file, BigInt &a) {
	a.print(file);
}