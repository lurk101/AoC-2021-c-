#define title "--- Day 1: Sonar Sweep ---"
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;
using namespace chrono;

const char* lines[] = {
#include "day1.txt"
};

int part1(void) {
    int count = 0, last_depth = 0, depth;
    last_depth = stoi(lines[0]);
    for (auto line : lines) {
        depth = stoi(line);
        if (depth > last_depth)
            count++;
        last_depth = depth;
    }
    return count;
}

int part2(void) {
    int depth[3], count = 1;
    int i = 0;
    for (; i < 3; i++)
        depth[i] = stoi(lines[i]);
    int last_sum = depth[0] + depth[1] + depth[2];
    depth[2] = depth[1];
    depth[1] = depth[0];
    for (; i < sizeof(lines) / sizeof(lines[0]); i++) {
        depth[0] = stoi(lines[i]);
        int sum = depth[0] + depth[1] + depth[2];
        if (sum > last_sum)
            count++;
        last_sum = sum;
        depth[2] = depth[1];
        depth[1] = depth[0];
    }
    return count;
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
