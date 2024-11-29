#ifndef DRAGON_H
#define DRAGON_H

#include "Enemy.h"
#include<string>
using namespace std;

class Dragon : public Enemy
{
    private:
    int health;
    int damage;
    string name;

public:
    Dragon(const std::string &type,int health,int damage,string name);
    string getName()const;
    int gethealth() const;
    int getdamage()const;
    string gettype() const;
    void takeDamage(int damage);
    bool isDefeated() const;
    void showStatus() const;
    //bool isAlive();
};

#endif
