// day4.cpp

#include <bitset>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
using namespace std;

list<unsigned> numbers;
typedef struct {
    uint8_t square[25];
    bitset<25> stamped;
    bool bingo;
} board;

list<board> boards;

void load(void) {
    ifstream file("day4.txt");
    unsigned char c = '0';
    unsigned n;
    numbers.clear();
    while (file >> n) {
        file >> c;
        if (c != ',')
            break;
        numbers.push_back(n);
    }
    boards.clear();
    while (!file.eof()) {
        board b;
        b.stamped.reset();
        b.bingo = false;
        for (int i = 0; i < 25; i++) {
            file >> n;
            n += (c - '0') * 10;
            c = '0';
            b.square[i] = n;
        }
        boards.push_back(b);
    }
    file.close();
}

bool bingo(board& b, uint8_t sq) {
    for (int i = 0; i < 25; i++)
        if (b.square[i] == sq) {
            b.stamped.set(i);
            if (b.stamped[0] && b.stamped[1] && b.stamped[2] && b.stamped[3] && b.stamped[4])
                return true;
            if (b.stamped[5] && b.stamped[6] && b.stamped[7] && b.stamped[8] && b.stamped[9])
                return true;
            if (b.stamped[10] && b.stamped[11] && b.stamped[12] && b.stamped[13] && b.stamped[14])
                return true;
            if (b.stamped[15] && b.stamped[16] && b.stamped[17] && b.stamped[18] && b.stamped[19])
                return true;
            if (b.stamped[20] && b.stamped[21] && b.stamped[22] && b.stamped[23] && b.stamped[24])
                return true;
            if (b.stamped[0] && b.stamped[5] && b.stamped[10] && b.stamped[15] && b.stamped[20])
                return true;
            if (b.stamped[1] && b.stamped[6] && b.stamped[11] && b.stamped[16] && b.stamped[21])
                return true;
            if (b.stamped[2] && b.stamped[7] && b.stamped[12] && b.stamped[17] && b.stamped[22])
                return true;
            if (b.stamped[3] && b.stamped[8] && b.stamped[13] && b.stamped[18] && b.stamped[23])
                return true;
            if (b.stamped[4] && b.stamped[9] && b.stamped[14] && b.stamped[19] && b.stamped[24])
                return true;
        }
    return false;
}

int part1(void) {
    load();
    for (auto n : numbers)
        for (auto& b : boards)
            if (bingo(b, n)) {
                unsigned sum = 0;
                for (int i = 0; i < 25; i++)
                    if (!b.stamped[i])
                        sum += b.square[i];
                return sum * n;
            }
    return 0;
}

int part2(void) {
    load();
    board last_board;
    unsigned last_n;
    for (auto n : numbers)
        for (auto& b : boards)
            if (!b.bingo && bingo(b, n)) {
                b.bingo = true;
                last_board = b;
                last_n = n;
            }
    unsigned sum = 0;
    for (int i = 0; i < 25; i++)
        if (!last_board.stamped[i])
            sum += last_board.square[i];
    return sum * last_n;
}

int main() {
    cout << "Part 1 - " << part1() << endl;
    cout << "Part 2 - " << part2() << endl;
}

