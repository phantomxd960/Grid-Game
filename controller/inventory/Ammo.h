#ifndef AMMO_H
#define AMMO_H

#include <string>
using namespace std;

class Ammo
{
    std::string name;
    int durability;
    int damage;

public:
    Ammo(const std::string &name, int durability,int damage);
    void use();
    bool isUsable() const;
    string getName();
    int getdamage();
   bool operator==(const Ammo& other) const {
        return name == other.name && durability == other.durability && damage == other.damage;
    }
};

#endif
