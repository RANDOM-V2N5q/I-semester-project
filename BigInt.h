#pragma once
typedef unsigned int int32u;
typedef unsigned long long int64u;
typedef long long int64;
constexpr int DIGITS = 17;

class BigInt {
private:
	std::vector<int32u> number;
	bool positive;

	std::vector<int32u> fitToBase2plus(std::vector<int64u>);
	std::vector<int32u> fitToBase2minus(std::vector<int64>);
	std::vector<int64u> convertToBase10();
	int MSBPosition();
	void popZeros();
	bool sign(BigInt, BigInt);
public:
	BigInt();
	~BigInt();

	BigInt(std::string);

	BigInt operator<<(int32u);
	void operator<<=(int32u);
	BigInt operator>>(int32u);
	void operator>>=(int32u);

	bool operator>(BigInt);
	bool operator<(BigInt);
	bool operator==(BigInt);
	bool operator!=(BigInt);
	bool operator<=(BigInt);
	bool operator>=(BigInt);

	BigInt operator+(BigInt);
	void operator+=(BigInt);

	BigInt operator-(BigInt);
	void operator-=(BigInt);

	BigInt operator*(BigInt);
	void operator*=(BigInt);

	BigInt operator/(BigInt);
	void operator/=(BigInt);

	BigInt operator%(BigInt);
	void operator%=(BigInt);

	void operator=(BigInt);
	void operator=(std::vector<int32u>);
	void operator=(std::string);

	BigInt power(BigInt);

	void print();
	void print(std::ofstream &file);
};