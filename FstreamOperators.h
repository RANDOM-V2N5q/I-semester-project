#pragma once

void operator>>(std::ifstream &file, BigInt &a);
void operator<<(std::ofstream &file, BigInt &a);