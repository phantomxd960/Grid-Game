#include "Ammo.h"
#include<string>
using namespace std;

Ammo::Ammo(const std::string &name, int durability,int damage) : name(name), durability(durability),damage(damage) {}

void Ammo::use()
{
    if (durability > 0)
        --durability;
}

bool Ammo::isUsable() const
{
    return durability > 0;
}
string Ammo::getName(){
    return name;
}
int Ammo::getdamage(){
    return damage;
}

