// day1.cpp

#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
using namespace std;

int part(int window_size) {
    ifstream file("day1_data.txt");
    list<int> depths;
    int depth, count = 0;
    for (int i = 0; i < window_size; i++) {
        file >> depth;
        depths.push_back(depth);
    }
    int last_sum = accumulate(begin(depths), end(depths), 0);
    while (file >> depth) {
        depths.push_back(depth);
        depths.pop_front();
        int sum = accumulate(begin(depths), end(depths), 0);
        if (sum > last_sum)
            count++;
        last_sum = sum;
    }
    return count;
}

int main() {
    cout << "Part 1 - " << part(1) << endl;
    cout << "Part 2 - " << part(3) << endl;
}
