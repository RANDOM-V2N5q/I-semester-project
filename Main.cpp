#include "MainHeader.h"

using namespace std;

int main(int argc, char* argv[]) {
	BigInt a, b;

	if(argc < 5) {
		system("echo -1");
		return -1;
	}

	{
		std::ifstream file_1(argv[1]);
		std::ifstream file_2(argv[3]);
		if(file_1.is_open() && file_2.is_open()) {
			file_1 >> a;
			file_2 >> b;

			file_1.close();
			file_2.close();
		}
		else {
			system("echo -2");
			return -2;
		}
	}

	{
		std::ofstream file(argv[4]);
		if(file.is_open()) {
			BigInt c;
			std::string arg = argv[2];
			if(arg == "sum") {
				c = a + b;
				file << c;
			}
			else if(arg == "sub") {
				c = a - b;
				file << c;
			}
			else if(arg == "mul") {
				c = a * b;
				file << c;
			}
			else if(arg == "div") {
				c = a / b;
				file << c;
			}
			else if(arg == "mod") {
				c = a % b;
				file << c;
			}
			else if(arg == "pow") {
				c = a.power(b);
				file << c;
			}
		}
		else {
			system("echo -3");
			return -3;
		}
	}

	return 0;
}