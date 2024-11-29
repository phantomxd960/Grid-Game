#ifndef GRID_H
#define GRID_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

enum Cell
{
    EMPTY = '.',
    PLAYER = 'P',
    ENEMY = 'E',
    COIN = 'C',
    INACCESSIBLE = 'x'
};

class Grid {
private:
    int size;
    std::vector<std::vector<char>> cells;

public:
    Grid(int size) : size(size), cells(size, std::vector<char>(size, EMPTY)) {}

    int getSize() const {
        return size;
    }

    int getCell(int x, int y) const {
        return cells[x][y];
    }

    void setCell(int x, int y, Cell type) {
        cells[x][y] = type;
    }

    void clear() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cells[i][j] = EMPTY;
            }
        }
    }
    void displayGrid() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout<<cells[i][j];
            }
            cout<<endl;
        }
    }


    std::pair<int, int> getRandomEmptyCell() {
        int x, y;
        do {
            x = rand() % size;
            y = rand() % size;
        } while (cells[x][y] != EMPTY);
        return {x, y};
    }
};

#endif
