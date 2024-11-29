#include "Player.h"
#include "../Grid.h"
#include "../inventory/Storage.h"
#include "../enemy/Enemy.h"
#include "../enemy/Dragon.h"
#include "../inventory/Ammo.h"
#include "../Game.h"
#include <iostream>
#include<vector>
using namespace std;

Ammo stick("stick",5,30);
Player::Player(const std::string &name) : name(name), health(100),coins(0),xp(0),currentWeapon(0), storage(stick) {
    
}

int Player::getPlayerX(){
    return x;
}
int Player::getPlayerY(){
    return y;
}
void Player::setPlayerX(int px){
    x=px;
}
void Player::setPlayerY(int py){
    y=py;
}
void Player::earnCoins(int amount)
{
    coins += amount;
}

int Player::getCoins() const
{
    return coins;
}
int Player::gethealth(){
    return health;
}
int Player:: getxp(){
    return xp;
}

void Player:: showStatus(){
        cout<<"Player status"<<endl;
        cout << "XP: " << xp << ", Health: " << health << ", Gold coins: " << coins << endl;
        cout << "Current Weapon: " << (storage.getItem())[currentWeapon].getName() << endl;
        cout << "Inventory: ";
        for (Ammo& item : storage.getItem()) cout << item.getName() << " ";
        cout << endl;
}

 void Player::addWeapon(vector<Ammo>&weapons) {
        if (currentWeapon < weapons.size() - 1 && coins >= 30) {
            //coins -= 30;
            if(currentWeapon==0){
                coins-=30;
            }
            else if(currentWeapon==1){
                coins-=95;
            }
            else if(currentWeapon==2){
                coins-=150;
            }
            currentWeapon++;
            //inventory.push_back(weapons[currentWeapon].getName());
            storage.addItem(weapons[currentWeapon]);
            
            cout << "You bought a " << weapons[currentWeapon].getName() << ".\n";
        } else if (currentWeapon == weapons.size() - 1) {
            cout <<"You already have the most powerful weapon!\n";
        } else {
            cout << "Not enough gold.\n";
        }
    }



    void Player::buyHealth(){
        if(coins>=10){
            losecoins(10);
            health+=10;
            cout<<"Exiting store.You bought 10 health. Current health: "<<health<<endl;
        }
        else{
            cout<<"Exiting store.Sorry,You don't have enough gold coins.\n";
        }
    }


void Player::takeDamage(int damage){
    health-=damage;
    if(health<0){
        health=0;
    }
}


void Player::losecoins(int amount){
    coins=(coins>=amount)? (coins-amount):0;
}
void Player::gainXP(int amount){
    xp+=amount;
}
bool Player::isAlive() const{
    return health>0;
}
Storage Player:: getStorage(){
        return storage;
    }
