// day6.cpp
#include <fstream>
#include <iostream>
using namespace std;

unsigned inline mod9(unsigned n) { return n >= 9 ? n - 9 : n; }

uint64_t fish[9] = {0};

uint64_t part(unsigned days) {
    static unsigned day0_ix = 0;
    for (unsigned day = 0; day < days; day++, day0_ix = mod9(day0_ix + 1))
        fish[mod9(day0_ix + 7)] += fish[day0_ix];
    uint64_t count = 0;
    for (unsigned n = 0; n < 9; n++)
        count += fish[n];
    return count;
}

int main() {
    ifstream file("day6.txt");
    char c;
    while (file >> c) {
        fish[c - '0']++;
        file >> c;
    }
    cout << "Part 1 - " << part(80) << endl;
    cout << "Part 2 - " << part(256 - 80) << endl;
}
