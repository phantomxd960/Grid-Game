#include "Storage.h"
#include "./Ammo.h"
#include <algorithm>

Storage:: Storage(Ammo &stick): weapons({}){
    addItem(stick);
}
void Storage::addItem(Ammo &item, int quantity)
{
    weapons.push_back(item);
    //items[item] += quantity;
}

bool Storage::hasItem(Ammo &item) const
{
    return std::find(weapons.begin(),weapons.end(),item) != weapons.end();
}

vector<Ammo> Storage::getItem(){
    return weapons;
}
