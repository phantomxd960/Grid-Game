#ifndef GAME_H
#define GAME_H

#include <utility>
class Player;
class Enemy;
#include "./Grid.h"

class Game {
private:
    Grid grid;
    int playerX, playerY;
    //Player player;
    //Enemy enemy;
    int enemyX, enemyY;
    vector<int> previousPlayerPos = { -1, -1 }; // Initialize to an invalid position
    int coinsCollected;
    int coinCount;
    //int level=0;

public:
    Game(int gridSize, int coins);

    Grid getGrid();
    void placePlayer(Player &player,int level);
    void placeEnemy(Enemy &enemy,int level);
    void placeCoins(int count);
    void placeInaccessible();
    void nextLevel(Player &player, Enemy &enemy);
    void initializeLevel(Player &player, Enemy &enemy,int level);
    pair<int, int> getNextEnemyMove();
    void movePlayer(int dx, int dy, Player &player, Enemy &enemy);
    int getCoinsCollected() const;
    //int getLevel() const;
    bool isValidPosition(int x, int y);
};

#endif // GAME_H
