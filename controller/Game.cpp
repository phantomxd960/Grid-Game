#include "Game.h"
#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <climits>
#include <algorithm>
#include <array>
#include <list>
#include <cstdlib>
#include "./Grid.h"
#include "./player/Player.h"
#include "./enemy/Enemy.h"
using namespace std;
const int INF = INT_MAX;
vector<array<int, 2>> directions = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
Game::Game(int gridSize, int coins)
    : grid(gridSize), playerX(0), playerY(0), enemyX(0), enemyY(0),
      coinsCollected(0), coinCount(coins)
{
    srand(time(0));
}
Grid Game::getGrid()
{
    return grid;
}
void Game::placePlayer(Player &player, int level)
{
    auto [x, y] = grid.getRandomEmptyCell();
    playerX = x;
    playerY = y;
    player.setPlayerX(playerX);
    player.setPlayerY(playerY);
    player.setplayerLevel(level);
    grid.setCell(playerX, playerY, PLAYER);
}
void Game::placeEnemy(Enemy &enemy, int level)
{
    if (level == 1)
    {
        enemyX = 9;
        enemyY = 9;
    }
    else if (level == 2)
    {
        enemyX = 7;
        enemyY = 7;
    }
    enemy.setEnemyX(enemyX);
    enemy.setEnemyY(enemyY);
    grid.setCell(enemyX, enemyY, ENEMY);
}
void Game::placeCoins(int count)
{
    for (int i = 0; i < count; ++i)
    {
        auto [x, y] = grid.getRandomEmptyCell();
        grid.setCell(x, y, COIN);
    }
}
void Game::placeInaccessible()
{
    for (int i = 0; i < 7; ++i)
    {
        auto [x, y] = grid.getRandomEmptyCell();
        grid.setCell(x, y, INACCESSIBLE);
    }
}
void Game::nextLevel(Player &player, Enemy &enemy)
{
    int level = player.getplayerLevel() + 1;
    initializeLevel(player, enemy, level);
}
void Game::initializeLevel(Player &player, Enemy &enemy, int level)
{
    grid.clear();
    placePlayer(player, level);
    placeEnemy(enemy, level);
    coinCount = (player.getplayerLevel() + 1) * 5;
    placeCoins(coinCount);
    if (player.getplayerLevel() == 2)
    {
        placeInaccessible();
    }
}
pair<int, int> Game::getNextEnemyMove()
{
    vector<vector<int>> dist(grid.getSize(), vector<int>(grid.getSize(), INF));
    queue<pair<int, int>> queue;
    queue.push({playerX, playerY});
    dist[playerX][playerY] = 0;
    while (!queue.empty())
    {
        auto current = queue.front();
        queue.pop();
        for (const auto &dir : directions)
        {
            int nx = current.first + dir[0];
            int ny = current.second + dir[1];
            if (isValidPosition(nx, ny) && dist[nx][ny] == INF)
            {
                dist[nx][ny] = dist[current.first][current.second] + 1;
                queue.push({nx, ny});
            }
        }
    }
    pair<int, int> currentPos = {enemyX, enemyY};
    for (int step = 0; step < 2; ++step)
    {
        int shortestDist = INF;
        pair<int, int> nextPos = currentPos;
        for (const auto &dir : directions)
        {
            int nx = currentPos.first + dir[0];
            int ny = currentPos.second + dir[1];
            if (isValidPosition(nx, ny) && dist[nx][ny] < shortestDist)
            {
                shortestDist = dist[nx][ny];
                nextPos = {nx, ny};
            }
        }
        if (nextPos == currentPos)
        {
            break;
        }
        currentPos = nextPos;
    }
    return currentPos;
}
bool Game::isValidPosition(int x, int y)
{
    return x >= 0 && y >= 0 && x < grid.getSize() && y < grid.getSize();
}
void Game::movePlayer(int dx, int dy, Player &player, Enemy &enemy)
{
    bool flag = false;
    int nx = playerX + dx, ny = playerY + dy;
    if (isValidPosition(nx, ny))
    {
        if (nx == enemy.getEnemyX() && ny == enemy.getEnemyY())
        {
            cout << "🟥 Invalid move" << endl;
            flag = true;
        }
        if (grid.getCell(nx, ny) == INACCESSIBLE)
        {
            if (player.getxp() < 10)
            {
                cout << "🟥 You can't move onto an inaccessible block!" << endl;
                flag = true;
            }
            player.losexp(10);
        }
        if (nx == previousPlayerPos[0] && ny == previousPlayerPos[1])
        {
            cout << "🟥 Invalid move! You cannot move back to the previous cell directly." << endl;
            flag = true;
        }
        if(flag == false)
        {
            grid.setCell(playerX, playerY, EMPTY);
            previousPlayerPos[0] = playerX;
            previousPlayerPos[1] = playerY;
            playerX = nx;
            playerY = ny;
            player.setPlayerX(playerX);
            player.setPlayerY(playerY);
            if (grid.getCell(nx, ny) == COIN)
            {
                player.earnCoins(100);
                coinCount--;
            }
            grid.setCell(playerX, playerY, PLAYER);
        }
    }
    else
    {
        cout << "🟥 Invalid move! You can't move onto an inaccessible block or outside the grid!" << endl;
    }
    auto [newEnemyX, newEnemyY] = getNextEnemyMove();
    grid.setCell(enemyX, enemyY, EMPTY);
    enemyX = newEnemyX;
    enemyY = newEnemyY;
    enemy.setEnemyX(enemyX);
    enemy.setEnemyY(enemyY);
    grid.setCell(enemyX, enemyY, ENEMY);
    if (playerX == enemyX && playerY == enemyY)
    {
        std::cout << "Game Over! You were caught by the enemy." << std::endl;
        if (player.getplayerLevel() == 2)
        {
            if (player.getCoins() >= 500)
            {
                player.gainXP(20);
                std::cout << "Level 2 complete! Coins: " << player.getCoins() << "\n" << std::endl;
                return;
            }
        }
        if (player.getplayerLevel() == 1)
        {
            if (player.getCoins() >= 300)
            {
                nextLevel(player, enemy);
                player.gainXP(10);
                std::cout << "Level 1 complete! Coins: " << player.getCoins() << "\n" << std::endl;
                return;
            }
        }
        cout << "Coins you collected " << player.getCoins() << endl;
        cout << "Sorry you don't have enough coins to build Town to go to next level" << endl;
        cout << "You have to play again\n" << endl;
        initializeLevel(player, enemy, player.getplayerLevel());
        return;
    }
    cout << "Player moved to (" << playerX << "," << playerY << ")" << endl;
    cout << "Enemy moved to (" << enemyX << "," << enemyY << ")" << endl;
}
int Game::getCoinsCollected() const
{
    return coinsCollected;
}
