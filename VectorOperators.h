#pragma once

std::vector<int64u> operator+(std::vector<int64u> a, std::vector<int64u> b);
std::vector<int64u> operator*(std::vector<int64u> a, int64u b);
void fitToBase10(std::vector<int64u> *a);