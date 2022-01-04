#define title "--- Day 6: Lanternfish ---"
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;
using namespace chrono;

unsigned inline mod9(unsigned n) { return n >= 9 ? n - 9 : n; }

uint64_t fish[9] = {};

uint64_t part(unsigned days) {
    static unsigned day0_ix = 0;
    for (unsigned day = 0; day < days; day++, day0_ix = mod9(day0_ix + 1))
        fish[mod9(day0_ix + 7)] += fish[day0_ix];
    uint64_t count = 0;
    for (unsigned n = 0; n < 9; n++)
        count += fish[n];
    return count;
}

const char* line =
#include "day6.txt"
    ;

int main() {
    auto start = high_resolution_clock::now();
    char c;
    stringstream ss(line);
    while (ss >> c) {
        fish[c - '0']++;
        ss >> c;
    }
    cout << title << endl
         << "Part 1 - " << part(80) << endl
         << "Part 2 - " << part(256 - 80) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
