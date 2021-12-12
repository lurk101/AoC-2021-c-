// day12.cpp
#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;
using namespace std::chrono;

uint8_t caves[16][8];
uint8_t tunnels[16];
uint8_t visits[16];
uint8_t lowcase[16];
string names[16];
uint8_t start_ix, end_ix;
unsigned n_caves;

void connect(unsigned from, unsigned to) { caves[from][tunnels[from]++] = to; }

unsigned get_cave_ix(string name) {
    unsigned i;
    for (i = 0; i < n_caves; i++)
        if (name == names[i])
            break;
    if (i == n_caves) {
        names[i] = name;
        if (name == "start")
            start_ix = i;
        else if (name == "end")
            end_ix = i;
        if (islower(name[0]))
            lowcase[i] = 1;
        visits[i] = 0;
        tunnels[i] = 0;
        n_caves++;
    }
    return i;
}

void load(void) {
    n_caves = 0;
    ifstream file("day12.txt");
    string from, to;
    for (;;) {
        file >> from;
        to = from.substr(from.find('-') + 1);
        from = from.substr(0, from.find('-'));
        unsigned from_ix = get_cave_ix(from);
        unsigned to_ix = get_cave_ix(to);
        connect(from_ix, to_ix);
        connect(to_ix, from_ix);
        if (file.eof())
            break;
    }
}

unsigned paths;
bool two_done;

void search(unsigned from, int part) {
    for (unsigned i = 0; i < tunnels[from]; i++) {
        uint8_t to = caves[from][i];
        if (to == end_ix) {
            paths++;
            continue;
        }
        if (to == start_ix)
            continue;
        bool done = false;
        if (lowcase[to])
            if (visits[to]) {
                if (two_done || (part == 1))
                    continue;
                two_done = true;
                done = true;
            }
        visits[to]++;
        search(to, part);
        visits[to]--;
        if (done)
            two_done = false;
    }
}

unsigned part(int part) {
    load();
    paths = 0;
    two_done = false;
    search(start_ix, part);
    return paths;
}

int main() {
    auto start = high_resolution_clock::now();
    cout << "Part 1 - " << part(1) << endl;
    cout << "Part 2 - " << part(2) << endl;
    cout << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
