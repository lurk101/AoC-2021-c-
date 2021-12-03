// day3.cpp

#include <fstream>
#include <iostream>
#include <list>
#include <string>
using namespace std;

list<unsigned> codes;
int bits;

void load(void) {
    ifstream file("day3.txt");
    string s;
    codes.clear();
    while (file >> s)
        codes.push_back(strtoul(s.c_str(), NULL, 2));
    bits = s.length();
    file.close();
}

int part1(void) {
    load();
    int gamma = 0;
    for (int b = bits - 1; b >= 0; b--) {
        gamma <<= 1;
        int count = 0;
        for (auto& code : codes)
            if (code & (1 << b))
                count++;
        if (count > (codes.size() - count))
            gamma |= 1;
    }
    return gamma * (gamma ^ ((unsigned)-1 >> (32 - bits)));
}

int rating(bool one) {
    load();
    list<unsigned> pruned;
    for (int b = bits - 1; b >= 0; b--) {
        int count = 0;
        for (auto& code : codes)
            if (code & (1 << b))
                count++;
        pruned.clear();
        int match = (count >= (codes.size() - count)) ? 1 << b : 0;
        for (auto& code : codes)
            if (((code & (1 << b)) == match) ^ one)
                pruned.push_back(code);
        codes = pruned;
        if (codes.size() == 1)
            break;
    }
    return codes.front();
}

int main() {
    cout << "Part 1 - " << part1() << endl;
    cout << "Part 2 - " << rating(true) * rating(false) << endl;
}

