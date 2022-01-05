#define title "--- Day 15: Chiton ---"
#include <chrono>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;
using namespace std::chrono;

struct point_s {
    short row, col;
    point_s(const short row, const short col) : row(row), col(col) {}
    bool operator==(const point_s& p) const { return row == p.row && col == p.col; }
};

struct compare_risk {
    bool operator()(const pair<point_s, int>& p1, const pair<point_s, int>& p2) const {
        return p1.second > p2.second;
    }
};

short risk[100][100];

struct hash_point {
    int operator()(const point_s& p) const { return p.row * 100 + p.col; }
};

short dimension = 100;

vector<point_s> neighbours(const point_s& p) {
    return {point_s(p.row + 1, p.col), point_s(p.row - 1, p.col), point_s(p.row, p.col + 1),
            point_s(p.row, p.col - 1)};
}

bool inside(const point_s& p) {
    return p.row >= 0 && p.col >= 0 && p.row < dimension && p.col < dimension;
}

unsigned Dijkstra(unsigned part_n) {
    priority_queue<pair<point_s, short>, vector<pair<point_s, int>>, compare_risk> prio_q;
    unordered_set<point_s, hash_point> visited;
    if (part_n == 2)
        dimension *= 5;
    const point_s start = point_s(0, 0);
    const point_s goal = point_s(dimension - 1, dimension - 1);
    prio_q.emplace(start, 0);

    while (!prio_q.empty()) {
        const auto [cp, cost] = prio_q.top();
        prio_q.pop();
        if (cp == goal)
            return cost;
        for (const auto& n : neighbours(cp)) {
            if (inside(n) && (visited.find(n) == visited.end())) {
                unsigned extra_risk =
                    risk[n.row % 100][n.col % 100] + (n.row / 100) + (n.col / 100);
                if (extra_risk > 9)
                    extra_risk -= 9;
                prio_q.emplace(n, cost + extra_risk);
                visited.insert(n);
            }
        }
    }
    return 0;
}

const char* lines[] = {
#include "day15.txt"
};

int main() {
    auto start = high_resolution_clock::now();
    cout << title << endl;
    for (int y = 0; y < 100; y++)
        for (int x = 0; x < 100; x++)
            risk[y][x] = lines[y][x] - '0';
    cout << "Part 1 - " << Dijkstra(1) << endl
         << "Part 2 - " << Dijkstra(2) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
