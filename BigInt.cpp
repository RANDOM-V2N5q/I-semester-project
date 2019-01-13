#include "MainHeader.h"

std::vector<int32u> BigInt::fitToBase2plus(std::vector<int64u> a) {
	std::vector<int32u> rsl;

	for(int i = 0; i < (int)a.size() - 1; i++) {
		if(a[i] >> 32) {
			a[i + 1] += a[i] >> 32;
			a[i] <<= 32;
			a[i] >>= 32;
		}
		rsl.push_back((int32u)a[i]);
	}
	if(a[a.size() - 1] >> 32) {
		a.push_back(a[a.size() - 1] >> 32);
		a[a.size() - 2] <<= 32;
		a[a.size() - 2] >>= 32;
		rsl.push_back((int32u)a[a.size() - 2]);
	}
	rsl.push_back((int32u)a[a.size() - 1]);

	return rsl;
}

std::vector<int32u> BigInt::fitToBase2minus(std::vector<int64> a) {
	std::vector<int32u> rsl;

	for(int32u i = 0; i < a.size(); i++) {
		if(a[i] < 0) {
			a[i + 1]--;
			a[i] += (int64)1 << 32;
		}
		rsl.push_back((int32u)a[i]);
	}
	
	return rsl;
}

std::vector<int64u> BigInt::convertToBase10() {
	std::vector<int64u> rsl;
	std::vector<int64u> tmp;

	tmp.push_back(1);
	rsl.push_back(0);

	if(!positive) {
		std::cout << '-';
	}
	for(int32u i = 0; i < number.size(); i++) {
		for(int j = 0; j < 32; j++) {
			if(number[i] & 1 << j) {
				rsl = rsl + tmp;
			}
			tmp = tmp * 2;
		}
	}

	return rsl;
}

int BigInt::MSBPosition() {
	int n = 31;
	while(n > -1 && !(number[number.size() - 1] & ((int32u)1 << n--)));

	return ((number.size() - 1) * 32) + (n + 1);
}

void BigInt::popZeros() {
	while(number.size() > 1 && number[number.size() - 1] == 0) {
		number.pop_back();
	}
}

bool BigInt::sign(BigInt a, BigInt b) {
	return (a.positive == b.positive) ? 1 : 0;
}

BigInt::BigInt() {
	positive = true;
}

BigInt::~BigInt() {
}

BigInt::BigInt(std::string str) {
	std::vector<int64u> num;

	{
		int str_len = str.length();

		if(str[0] == '-') {
			positive = false;
			str_len--;
		}
		else {
			positive = true;
		}

		for(int i = 0; i < str_len / DIGITS; i++) {
			num.push_back(std::stoll(str.substr(str_len - ((i + 1) * DIGITS), DIGITS)));
		}
		if(positive && str_len % DIGITS > 0) {
			num.push_back(std::stoll(str.substr(0, str_len % DIGITS)));
		}
		else if(str_len % DIGITS > 0) {
			num.push_back(std::stoll(str.substr(1, str_len % DIGITS)));
		}
	}

	{
		int bit{};
		int32u segment{};

		while(num.size() > 0) {
			if(num[0] % 2) {
				segment |= 1 << bit;
			}
			bit++;

			if(bit == 32) {
				number.push_back(segment);
				bit = 0;
				segment = 0;
			}

			for(int i = (int)num.size() - 1; i > 0; i--) {
				if(num[i] % 2) {
					num[i - 1] += (int64u)pow(10, DIGITS);
				}
				num[i] /= 2;
			}
			num[0] /= 2;

			if((num.size() > 0) && (num[num.size() - 1] == 0)) {
				num.pop_back();
			}
		}

		if(bit > 0) {
			number.push_back(segment);
		}
	}
}

BigInt BigInt::operator<<(int32u a) {
	BigInt rsl;

	rsl.number.resize(a / 32);
	for(int32u i = 0; i < number.size(); i++) {
		rsl.number.push_back(number[i]);
	}

	rsl.number.push_back((int64)rsl.number[rsl.number.size() - 1] >> (32 - (a % 32)));
	for(int i = (int)rsl.number.size() - 2; i > ((int)a / 32); i--) {
		rsl.number[i] <<= a % 32;
		rsl.number[i] |= rsl.number[i - 1] >> (32 - (a % 32));
	}
	rsl.number[a / 32] <<= a % 32;

	if(rsl.number.size() > 0 && rsl.number[rsl.number.size() - 1] == 0) {
		rsl.number.pop_back();
	}

	return rsl;
}

void BigInt::operator<<=(int32u a) {
	(*this) = (*this) << a;
}

BigInt BigInt::operator>>(int32u a) {
	BigInt rsl;

	for(int32u i = a / 32; i < number.size(); i++) {
		rsl.number.push_back(number[i]);
	}

	for(int i = 0; i < ((int)rsl.number.size() - 1); i++) {
		rsl.number[i] >>= a % 32;
		rsl.number[i] |= rsl.number[i + 1] << (32 - (a % 32));
	}
	rsl.number[rsl.number.size() - 1] >>= a % 32;

	rsl.popZeros();
	return rsl;
}

void BigInt::operator>>=(int32u a) {
	(*this) = (*this) >> a;
}

bool BigInt::operator>(BigInt a) {
	if(number.size() > a.number.size()) {
		return 1;
	}
	else if(number.size() < a.number.size()) {
		return 0;
	}
	else {
		for(int i = (int)number.size() - 1; i > -1; i--) {
			if(number[i] > a.number[i]) {
				return 1;
			}
			else if(number[i] < a.number[i]) {
				return 0;
			}
		}
		return 0;
	}
}

bool BigInt::operator<(BigInt a) {
	return (!((*this) > a) && !((*this) == a)) ? 1 : 0;
}

bool BigInt::operator==(BigInt a) {
	if(number.size() != a.number.size()) {
		return 0;
	}
	else {
		for(int32u i = 0; i < number.size(); i++) {
			if(number[i] != a.number[i]) {
				return 0;
			}
		}
		return 1;
	}
}

bool BigInt::operator!=(BigInt a) {
	return (!((*this) == a)) ? 1 : 0;
}

bool BigInt::operator<=(BigInt a) {
	return (!((*this) > a)) ? 1 : 0;
}

bool BigInt::operator>=(BigInt a) {
	return (!((*this) < a)) ? 1 : 0;
}

BigInt BigInt::operator+(BigInt b) {
	BigInt a = (*this);

	if(a.positive == false && b.positive == true) {
		a.positive = true;
		return b - a;
	}
	else if(this->positive == true && b.positive == false) {
		b.positive = true;
		return a - b;
	}
	else {
		int32u i{};

		if(b > a) {
			BigInt tmp = a;
			a = b;
			b = tmp;
		}

		std::vector<int64u> num{};
		for(; i < b.number.size(); i++) {
			num.push_back((int64u)a.number[i] + b.number[i]);
		}
		for(; i < a.number.size(); i++) {
			num.push_back(a.number[i]);
		}
		BigInt rsl;
		rsl = fitToBase2plus(num);

		if(a.positive && b.positive) {
			rsl.positive = true;
		}
		else {
			rsl.positive = false;
		}

		return rsl;
	}
}

void BigInt::operator+=(BigInt a) {
	(*this) = (*this) + a;
}

BigInt BigInt::operator-(BigInt b) {
	BigInt a = (*this);

	if(a.positive == true && b.positive == true && a < b) {
		BigInt rsl = b - a;
		rsl.positive = false;
		return rsl;
	}
	else if(a.positive == false && b.positive == false && a > b) {
		a.positive = true;
		b.positive = true;
		BigInt rsl = a - b;
		rsl.positive = false;
		return rsl;
	}
	else if(a.positive == false && b.positive == false && a < b) {
		a.positive = true;
		b.positive = true;
		BigInt rsl = b - a;
		rsl.positive = true;
		return rsl;
	}
	else if((a.positive == false && b.positive == true) || (a.positive == true && b.positive == false)) {
		b.positive = !b.positive;
		return a + b;
	}
	else {
		int32u i{};

		std::vector<int64> num;
		for(; i < b.number.size(); i++) {
			num.push_back((int64)a.number[i] - b.number[i]);
		}
		for(; i < a.number.size(); i++) {
			num.push_back(a.number[i]);
		}
		BigInt rsl;
		rsl = fitToBase2minus(num);
		rsl.popZeros();
		rsl.positive = true;
		return rsl;
	}
}

void BigInt::operator-=(BigInt a) {
	(*this) = (*this) - a;
}

BigInt BigInt::operator*(BigInt b) {
	BigInt a = *this;
	BigInt rsl;

	if(b > a) {
		BigInt tmp = a;
		a = b;
		b = tmp;
	}

	for(size_t i = 0; i < b.number.size(); i++) {
		std::vector<int64u> tmp(i);
		for(size_t j = 0; j < a.number.size(); j++) {
			tmp.push_back((int64u)b.number[i] * a.number[j]);
		}
		BigInt c;
		c = fitToBase2plus(tmp);
		rsl = rsl + c;
	}

	rsl.sign(a, b);
	return rsl;
}

void BigInt::operator*=(BigInt a) {
	(*this) = (*this) * a;
}

BigInt BigInt::operator/(BigInt b) {
	BigInt rsl;

	if(b.number.size() == 1 && b.number[0] == 0) {
		return rsl;
	}
	else if((*this) < b) {
		return rsl;
	}

	int deltaMSB = (MSBPosition() - b.MSBPosition());
	rsl.number.resize(((deltaMSB / 32) + 1));
	b = b << deltaMSB;

	BigInt tmp;
	for(int i = deltaMSB; i > -1; i--) {
		tmp = tmp + b;
		if((*this) >= tmp) {
			rsl.number[i / 32] |= 1 << (i % 32);
		}
		else {
			tmp = tmp - b;
		}
		b = b >> 1;
	}

	rsl.popZeros();
	rsl.sign((*this), b);
	return rsl;
}

void BigInt::operator/=(BigInt a) {
	(*this) = (*this) / a;
}

BigInt BigInt::operator%(BigInt a) {
	BigInt tmp;
	int deltaMSB = (MSBPosition() - a.MSBPosition());

	a = a << deltaMSB;

	for(int i = deltaMSB; i > -1; i--) {
		tmp = tmp + a;
		if((*this) < tmp) {
			tmp = tmp - a;
		}
		a = a >> 1;
	}

	return (*this) - tmp;
}

void BigInt::operator%=(BigInt a) {
	(*this) = (*this) % a;
}

void BigInt::operator=(BigInt a) {
	number = a.number;
	positive = a.positive;
}

void BigInt::operator=(std::vector<int32u> a) {
	number = a;
}

void BigInt::operator=(std::string str) {
	BigInt tmp(str);
	(*this) = tmp;
}

BigInt BigInt::power(BigInt a) {
	BigInt rsl("1");
	BigInt tmp = *this;

	while(a.number.size()>0 && a.number[0]>0) {
		if(a.number[0] % 2) {
			rsl *= tmp;
		}
		tmp *= tmp;
		a >>= 1;
	}

	return rsl;
}

void BigInt::print() {
	std::vector<int64u> rsl = convertToBase10();

	std::cout << rsl[rsl.size() - 1];
	std::cout.fill('0');
	for(int i = (int)rsl.size() - 2; i > -1; i--) {
		std::cout.width(DIGITS);
		std::cout << rsl[i];
	}
}

void BigInt::print(std::ofstream &file) {
	std::vector<int64u> rsl = convertToBase10();

	file << rsl[rsl.size() - 1];
	std::cout.fill('0');
	for(int i = (int)rsl.size() - 2; i > -1; i--) {
		std::cout.width(DIGITS);
		file << rsl[i];
	}
}
