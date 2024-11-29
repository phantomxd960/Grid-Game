#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include<vector>

//#include<bits/stdc++.h>
#include "../inventory/Storage.h"
using namespace std;


class Enemy;
class Dragon;

class Player
{
    private:

    string name;
    int coins;
    int xp;
    int health;
    int currentWeapon;
    vector<string>inventory;
    Storage storage;

    int playerlevel;
    int x;
    int y;

public:
    Player(const std::string &name);
    // Default Constructor
    //Player() : playerX(0), playerY(0) {}

    // Getter for player's X-coordinate
    int getPlayerX();
    void setPlayerX(int xc);
    void setPlayerY(int yc);
    // Getter for player's Y-coordinate
    int getPlayerY();
    int getplayerLevel(){
        return playerlevel;
    }
    void setplayerLevel(int level){
        playerlevel=level;
    }
    Storage getStorage();


    /*void playLevel1(Player &player,Enemy &enemy, Storage &storage);
    void playLevel2(Player &player,Enemy &enemy, Storage &storage, Ammo &sword);
    void playLevel3(Dragon &dragon, Player &player,vector<Ammo>&weapons);*/

    string getname();
    void startdragongame(Dragon &dragon,Player &player,vector<Ammo>&weapons);
    void buyHealth();
    void showStatus();
    int getxp();
    int getcurrentWeapon();
    int gethealth();
    void takeDamage(int damage);
    void gotoStore(vector<Ammo>&weapons,Player &player);
    void addWeapon(vector<Ammo>&weapons);
    void changeWeapon(const string&weapon);
    void earnCoins(int amount);
    int getCoins() const;
    void losecoins(int amount);
    void gainXP(int amount);
    void losexp(int amount){
        xp-=amount;
    }
    bool isAlive() const;
    void gotoCave(vector<Ammo>&weapons,Player &player);
};

#endif
