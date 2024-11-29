#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "./Ammo.h"


class Storage
{
    vector<Ammo> weapons;

public:
    Storage(Ammo &stick);
    void addItem(Ammo &item, int quantity = 1);
    bool hasItem(Ammo &item) const;
    vector<Ammo> getItem();
};

#endif
