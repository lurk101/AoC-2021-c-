#define title "--- Day 25: Sea Cucumber ---"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace chrono;

const char* lines[] = {
#include "day25.txt"
};

#define x_max 139
#define y_max 137

uint8_t trench[y_max][x_max], new_trench[y_max][x_max];

short move_right(void) {
    short moves = 0;
    memset(new_trench, '.', sizeof(new_trench));
    for (short y = 0; y < y_max; y++)
        for (short x = 0; x < x_max; x++) {
            if (trench[y][x] == '>') {
                if (trench[y][(x + 1) % x_max] == '.') {
                    new_trench[y][(x + 1) % x_max] = '>';
                    moves++;
                } else
                    new_trench[y][x] = '>';
            } else if (trench[y][x] == 'v')
                new_trench[y][x] = 'v';
        }
    memcpy(trench, new_trench, sizeof(trench));
    return moves;
}

short move_down(void) {
    short moves = 0;
    memset(new_trench, '.', sizeof(new_trench));
    for (short y = 0; y < y_max; y++)
        for (short x = 0; x < x_max; x++) {
            if (trench[y][x] == 'v') {
                if (trench[(y + 1) % y_max][x] == '.') {
                    new_trench[(y + 1) % y_max][x] = 'v';
                    moves++;
                } else
                    new_trench[y][x] = 'v';
            } else if (trench[y][x] == '>')
                new_trench[y][x] = '>';
        }
    memcpy(trench, new_trench, sizeof(trench));
    return moves;
}

short part1(void) {
    memset(trench, '.', sizeof(trench));
    for (short y = 0; y < sizeof(lines) / sizeof(lines[0]); y++)
        for (short x = 0; x < strlen(lines[y]); x++)
            trench[y][x] = lines[y][x];
    short steps = 1;
    for (;; steps++)
        if (move_right() + move_down() == 0)
            break;
    return steps;
}

int main() {
    auto start = high_resolution_clock::now();
    cout << title << endl
         << "Part 1  - " << part1() << endl
         << "Part 2  - free star" << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
