// day12.cpp
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
using namespace std;
using namespace std::chrono;

typedef struct {
    list<string> to;
    uint8_t visit;
} cave;
map<string, cave*> caves;

void connect(string from, string to) {
    auto c = caves.find(from);
    if (c != caves.end())
        c->second->to.push_back(to);
    else {
        cave* c2 = new cave;
        c2->visit = false;
        c2->to.push_back(to);
        caves.insert(pair<string, cave*>(from, c2));
    };
}

void load(void) {
    for (auto c : caves)
        delete &c.second->to;
    caves.clear();
    ifstream file("day12.txt");
    string from, to;
    for (;;) {
        file >> from;
        to = from.substr(from.find('-') + 1);
        from = from.substr(0, from.find('-'));
        connect(from, to);
        connect(to, from);
        if (file.eof())
            break;
    }
}

unsigned paths = 0;
bool two_done = false;

void search(list<string>& tos, int part) {
    for (auto& to : tos) {
        if (to == "end") {
            paths++;
            continue;
        }
        if (to == "start")
            continue;
        auto c = caves.find(to);
        bool done = false;
        if (islower(to[0])) {
            if (c->second->visit) {
                if (two_done || (part == 1))
                    continue;
                two_done = true;
                done = true;
            }
        }
        c->second->visit++;
        search(c->second->to, part);
        c->second->visit--;
        if (done)
            two_done = false;
    }
}

unsigned part(int part) {
    load();
    paths = 0;
    two_done = false;
    auto c = caves.find("start");
    search(c->second->to, part);
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
