// day8
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

#define SIGNALS 10
#define DIGITS 4
#define SEGMENTS 7

string xlated[SIGNALS];
int xlate[SEGMENTS];
string signals[SIGNALS], digits[DIGITS];
string segments[] = {"abcefg", "cf",     "acdeg", "acdfg",   "bcdf",
                     "abdfg",  "abdefg", "acf",   "abcdefg", "abcdfg"};

void xlate_segments(void) {
    string v[SIGNALS];
    for (int i = 0; i < SIGNALS; i++) {
        string s, segment = segments[i];
        for (int j = 0; j < segment.length(); j++)
            s += xlate[segment[j] - 'a'] + 'a';
        sort(s.begin(), s.end());
        xlated[i] = s;
    }
}

bool check(void) {
    xlate_segments();
    for (int i = 0; i < SIGNALS; i++) {
        bool found = false;
        for (int j = 0; j < SIGNALS; j++)
            if (signals[i] == xlated[j]) {
                found = true;
                break;
            }
        if (!found)
            return false;
    }
    return true;
}

int main() {
    int lines = 200, part1 = 0, part2 = 0;
    ifstream file("day8.txt");
    for (int l = 0; l < lines; l++) {
        string bar;
        for (int j = 0; j < SIGNALS; j++) {
            file >> signals[j];
            sort(signals[j].begin(), signals[j].end());
        }
        file >> bar;
        for (int j = 0; j < DIGITS; j++) {
            file >> digits[j];
            sort(digits[j].begin(), digits[j].end());
            if (digits[j].length() < 5 || digits[j].length() == 7)
                part1++;
        }
        for (int j = 0; j < SEGMENTS; j++)
            xlate[j] = j;
        do
            if (check()) {
                int n = 0;
                for (int j = 0; j < DIGITS; j++) {
                    n *= 10;
                    for (int k = 0; k < SIGNALS; k++)
                        if (digits[j] == xlated[k])
                            n += k;
                }
                part2 += n;
                break;
            }
        while (next_permutation(xlate, xlate + SEGMENTS));
    }
    cout << "Part 1 - " << part1 << endl;
    cout << "Part 2 - " << part2 << endl;
}
