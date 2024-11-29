#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Frontend.h"
#include "jni.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "enemy/Dragon.h"
#include "inventory/Storage.h"
#include "inventory/Ammo.h"
#include "./Game.h"
#include <algorithm>
#include <cctype>

using namespace std;

Player hero("Hero");
Enemy enemy("Ogre");

Game game1(10, 5);
Game game2(8, 7);

Ammo sword("Sword", 5, 70);

Ammo dagger("Dagger", 1, 100);
Ammo clawHammer("Claw Hammer", 1, 250);
vector<Ammo> weapons = {sword, dagger, clawHammer};

Dragon dragon = Dragon("Dragon", 1050, 20, "Fire Dragon");
Dragon slime = Dragon("Dragon", 2000, 13, "Slime");
Dragon fangedBeast = Dragon("Dragon", 3000, 80, "Fanged Beast");
int damagetoDragon;
int damage;

std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void handleLevel1()
{
    game1.initializeLevel(hero, enemy, 1);
    game1.getGrid().displayGrid();
    cout << "Level 1: Avoid the enemy on the grid and collect coins as much as possible to build the town hall!" << endl;
}

void handleLevel2()
{
    game2.initializeLevel(hero, enemy, 2);
    game2.getGrid().displayGrid();
    cout << "Level 2: Avoid the enemy with restricted areas!" << endl;
}

void handleLevel3()
{

    cout << "Level 3: Welcome to the Dragon Slayer game!" << endl;
}

void handleMove(const string &direction, const string &level)
{
    if (level == "1")
    {

        if (direction == "W" || direction == "w")
        {
            game1.movePlayer(-1, 0, hero, enemy);
        }
        else if (direction == "A" || direction == "a")
        {
            game1.movePlayer(0, -1, hero, enemy);
        }
        else if (direction == "S" || direction == "s")
        {
            game1.movePlayer(1, 0, hero, enemy);
        }
        else if (direction == "D" || direction == "d")
        {
            game1.movePlayer(0, 1, hero, enemy);
        }
        else
        {
            std::cout << "You exited the game.\n";
            return;
        }
        game1.getGrid().displayGrid();
        cout << "Movement done.\n";
    }
    else if (level == "2")
    {

        if (direction == "W" || direction == "w")
        {
            game2.movePlayer(-1, 0, hero, enemy);
        }
        else if (direction == "A" || direction == "a")
        {
            game2.movePlayer(0, -1, hero, enemy);
        }
        else if (direction == "S" || direction == "s")
        {
            game2.movePlayer(1, 0, hero, enemy);
        }
        else if (direction == "D" || direction == "d")
        {
            game2.movePlayer(0, 1, hero, enemy);
        }
        else
        {
            std::cout << "You exited the game.\n";
            return;
        }
        game2.getGrid().displayGrid();

        cout << "Movement done.\n";
    }
}

void handleAction(const string &action)
{

    if (action == "1")
    {
        cout << "Fight Dragon: " << dragon.getName() << "!\n";
    }
    else if (action == "2")
    {
        cout << "Enter Store.\n";
    }
    else if (action == "3")
    {
        cout << "Go to Cave. Who would you like to fight?\n";
    }
    else if (action == "4")
    {
        cout << "\n";
    }
}
void playandWIn(Dragon &dragon, const string &action)
{
    if (action == "1")
    {

        Ammo cweapon = hero.getStorage().getItem().back();
        if (cweapon.isUsable())
        {
            cweapon.use();
            damagetoDragon = cweapon.getdamage();
            dragon.takeDamage(damagetoDragon);
        }
        else
        {
            cout << "Sorry, You don't have chances left for your current weapon to attack. Go to store to buy weapons.\n";
            return;
        }

        if (!dragon.isDefeated() && hero.isAlive())
        {
            damage = dragon.getdamage();
            hero.takeDamage(damage);
        }

        if (dragon.isDefeated() && hero.isAlive())
        {
            cout << "You defeated the dragon!\n";
            hero.gainXP(50);
            hero.earnCoins(100);
            hero.showStatus();
            return;
        }
        else if (!hero.isAlive())
        {
            cout << "The dragon defeated you. Game over. 💀\n";
            return;
        }
        hero.showStatus();
        cout << "You attacked the dragon " << dragon.getName() << " for " << damagetoDragon << " damage.\n";
    }
    else if (action == "2")
    {

        dragon.takeDamage(30);

        if (!dragon.isDefeated() && hero.isAlive())
        {
            damage = dragon.getdamage();
            hero.takeDamage(damage);
        }

        if (dragon.isDefeated() && hero.isAlive())
        {
            cout << "You defeated the dragon!\n";
            hero.gainXP(50);
            hero.earnCoins(100);
            hero.showStatus();
            return;
        }
        else if (!hero.isAlive())
        {
            cout << "The dragon defeated you. Game over. 💀\n";
            return;
        }
        hero.showStatus();
        cout << "You dodged the dragon " << dragon.getName() << " attack.\n";
    }
    else if (action == "3")
    {
        cout << "Enter store.\n";
    }
}
void handledragonAction(const string &action, const string &dragonName)
{
    if (dragonName == "Slime")
    {
        playandWIn(slime, action);
    }

    else if (dragonName == "FangedBeast")
    {
        playandWIn(fangedBeast, action);
    }
    else
    {
        playandWIn(dragon, action);
    }
}

void handleStoreAction(const string &action)
{

    if (action == "1")
    {
        hero.buyHealth();
    }
    else if (action == "2")
    {
        hero.addWeapon(weapons);
    }
    else if (action == "3")
    {
        cout << "Go back to fight" << endl;
    }
    else
    {
        cout << "Invalid store action!" << endl;
    }
}

void handleCaveAction(const string &choice)
{

    if (choice == "slime")
    {
        cout << "Battle completed with " + choice << endl;
    }
    else if (choice == "FangedBeast")
    {
        cout << "Battle completed with " + choice << endl;
    }
    else
    {
        cout << "Invalid monster!" << endl;
    }
}

string command = "";

extern "C"
{
    JNIEXPORT void JNICALL Java_Frontend_startLevel1(JNIEnv *env, jobject obj)
    {
        handleLevel1();
    }

    JNIEXPORT void JNICALL Java_Frontend_startLevel2(JNIEnv *env, jobject obj)
    {
        handleLevel2();
    }

    JNIEXPORT void JNICALL Java_Frontend_startLevel3(JNIEnv *env, jobject obj)
    {
        handleLevel3();
    }

    JNIEXPORT void JNICALL Java_Frontend_movePlayer(JNIEnv *env, jobject obj, jstring direction, jstring level)
    {
        const char *directionChars = env->GetStringUTFChars(direction, nullptr);
        const char *levelChars = env->GetStringUTFChars(level, nullptr);
        std::string directionVal(directionChars);
        std::string levelVal(levelChars);
        handleMove(directionVal, levelVal);
        env->ReleaseStringUTFChars(direction, directionChars);
        env->ReleaseStringUTFChars(level, levelChars);
    }

    JNIEXPORT void JNICALL Java_Frontend_performAction(JNIEnv *env, jobject obj, jstring action)
    {
        const char *actionChars = env->GetStringUTFChars(action, nullptr);
        std::string actionVal(actionChars);
        handleAction(actionVal);
        env->ReleaseStringUTFChars(action, actionChars);
    }

    JNIEXPORT void JNICALL Java_Frontend_performDragonAction(JNIEnv *env, jobject obj, jstring action, jstring dragonName)
    {
        const char *actionChars = env->GetStringUTFChars(action, nullptr);
        const char *dragonnameChars = env->GetStringUTFChars(dragonName, nullptr);
        std::string actionVal(actionChars);
        std::string dragonnameVal(dragonnameChars);
        handledragonAction(actionVal, dragonnameVal);
        env->ReleaseStringUTFChars(action, actionChars);
        env->ReleaseStringUTFChars(dragonName, dragonnameChars);
    }

    JNIEXPORT void JNICALL Java_Frontend_performStoreAction(JNIEnv *env, jobject obj, jstring action)
    {
        const char *actionChars = env->GetStringUTFChars(action, nullptr);
        std::string actionVal(actionChars);
        handleStoreAction(actionVal);
        env->ReleaseStringUTFChars(action, actionChars);
    }

    JNIEXPORT void JNICALL Java_Frontend_performCaveAction(JNIEnv *env, jobject obj, jstring monster)
    {
        const char *monsterChars = env->GetStringUTFChars(monster, nullptr);
        std::string monsterVal(monsterChars);
        handleCaveAction(monsterVal);
        env->ReleaseStringUTFChars(monster, monsterChars);
    }

    JNIEXPORT void JNICALL Java_Frontend_exitGame(JNIEnv *env, jobject obj)
    {
        cout << "Exiting current menu" << endl;
    }
}

int main()
{
    return 0;

}
