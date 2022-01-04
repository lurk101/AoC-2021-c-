#define title "--- Day 2: Dive! ---"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace chrono;

const char* lines[] = {
#include "day2.txt"
};

int part1() {
    int x = 0, z = 0;
    for (auto& line : lines) {
        int distance;
        string l(line);
        switch (l[0]) {
        case 'f':
            distance = stoi(l.substr(8));
            x += distance;
            break;
        case 'd':
            distance = stoi(l.substr(5));
            z += distance;
            break;
        default:
            distance = stoi(l.substr(3));
            z -= distance;
            break;
        }
    }
    return x * z;
}

int part2() {
    int x = 0, z = 0, aim = 0;
    for (auto& line : lines) {
        string l(line);
        int distance;
        switch (l[0]) {
        case 'f':
            distance = stoi(l.substr(8));
            x += distance;
            z += aim * distance;
            break;
        case 'd':
            distance = stoi(l.substr(5));
            aim += distance;
            break;
        default:
            distance = stoi(l.substr(3));
            aim -= distance;
            break;
        }
    }
    return x * z;
}

int main() {
    auto start = high_resolution_clock::now();
    cout << title << endl
         << "Part 1 - " << part1() << endl
         << "Part 2 - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
