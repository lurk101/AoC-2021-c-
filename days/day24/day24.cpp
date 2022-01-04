#define title "--- Day 24: Arithmetic Logic Unit ---"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <sstream>
using namespace std;
using namespace chrono;

#define N 14

struct block {
    bool is_pop;
    int8_t div, off, mod;
} blocks[N];

int64_t part(int part_n) {
    typedef pair<int8_t, int8_t> item;
    item stack[N];
    unsigned stack_idx = 0;
    int8_t w[N];
    for (auto& i : w)
        i = 0;
    for (int i = 0; i < 14; i++) {
        auto& block(blocks[i]);
        if (!block.is_pop)
            stack[stack_idx++] = {i, block.mod};
        else {
            auto& it(stack[--stack_idx]);
            int8_t d = block.off + it.second;
            w[it.first] = part_n == 2 ? max(1 - d, 1) : min(9 - d, 9);
            w[i] = w[it.first] + d;
        }
    }
    uint64_t n = 0;
    for (auto& digit : w)
        n = n * 10 + digit;
    return n;
}

const char* lines[] = {
#include "day24.txt"
};

int main() {
    auto start = high_resolution_clock::now();
    unsigned block_id = 0, line_id = 0;
    for (auto& line : lines) {
        string l(line);
        switch (line_id) {
        case 4:
            blocks[block_id].div = stol(l.substr(6));
            blocks[block_id].is_pop = blocks[block_id].div == 26;
            break;
        case 5:
            blocks[block_id].off = stol(l.substr(6));
            break;
        case 15:
            blocks[block_id].mod = stol(l.substr(6));
        }
        line_id = (line_id + 1) % 18;
        if (line_id == 0)
            block_id++;
    }
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
