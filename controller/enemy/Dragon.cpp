#include "Dragon.h"
#include <iostream>

Dragon::Dragon(const std::string &type,int health,int damage,string name) : Enemy(type), health(health),damage(damage),name(name) {}

int Dragon:: gethealth()const{
    return health;
}
int Dragon::getdamage()const{
    return damage;
}
string Dragon::getName() const{
    return name;
}
void Dragon:: showStatus() const {
        cout<<"Dragon status"<<endl; 
        cout << name << " Status:\n";
        cout << "Health: " << health << ", Damage: " << damage << endl;
    }




void Dragon::takeDamage(int damage)
{
    health -= damage;
    if(health<0) health=0;
    std::cout << type << " took " << damage << " damage! Health: " << health << std::endl;
}

bool Dragon::isDefeated() const
{
    return health <= 0;
}
