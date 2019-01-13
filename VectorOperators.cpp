#include "MainHeader.h"

std::vector<int64u> operator+(std::vector<int64u> a, std::vector<int64u> b) {
	for(int32u i = 0; i < a.size(); i++) {
		b[i] += a[i];
	}
	fitToBase10(&b);

	return b;
}

std::vector<int64u> operator*(std::vector<int64u> a, int64u b) {
	for(int32u i = 0; i < a.size(); i++) {
		a[i] *= b;
	}
	fitToBase10(&a);

	return a;
}

void fitToBase10(std::vector<int64u> *a) {
	int64u tmp = (int64u)pow(10, DIGITS);

	for(int32u i = 0; i < (*a).size() - 1; i++) {
		if((*a)[i] / tmp) {
			(*a)[i + 1] += (*a)[i] / tmp;
			(*a)[i] %= tmp;
		}
	}
	if((*a)[(*a).size() - 1] / tmp) {
		(*a).push_back((*a)[(*a).size() - 1] / tmp);
		(*a)[(*a).size() - 2] %= tmp;
	}
}