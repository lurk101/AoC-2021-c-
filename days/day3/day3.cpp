#define title "--- Day 3: Binary Diagnostic ---"
#include <chrono>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
using namespace chrono;

list<uint16_t> codes, save_codes;

int part1(void) {
    uint16_t gamma = 0;
    for (int b = 11; b >= 0; b--) {
        gamma <<= 1;
        int count = 0;
        uint16_t mask = 1 << b;
        for (auto& code : codes)
            if (code & mask)
                count++;
        if (count > (codes.size() - count))
            gamma |= 1;
    }
    return gamma * (gamma ^ (uint16_t(-1) >> 4));
}

unsigned rating(uint16_t one) {
    codes = save_codes;
    for (int b = 11; b >= 0; b--) {
        list<uint16_t> pruned;
        int count = 0;
        uint16_t mask = 1 << b;
        for (auto& code : codes)
            if (code & mask)
                count++;
        uint16_t match = (count >= (codes.size() - count)) ? mask : 0;
        for (auto& code : codes)
            if (((code & mask) == match) ^ one)
                pruned.push_back(code);
        codes = pruned;
        if (pruned.size() == 1)
            break;
    }
    return codes.front();
}

const char* lines[] = {
#include "day3.txt"
};

int main() {
    auto start = high_resolution_clock::now();
    for (auto& s : lines)
        codes.push_back(strtoul(s, NULL, 2));
    save_codes = codes;
    cout << title << endl << "Part 1 - " << part1() << endl;
    cout << "Part 2 - " << rating(1) * rating(0) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
