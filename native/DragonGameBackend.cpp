#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "src_DragonGameBackendJNI.h"

// ------------------------------------------------------------
// DRAGON CLASS
// ------------------------------------------------------------

class Dragon {
private:
    int health;
    int playerFailures;

public:
    Dragon() : health(4000), playerFailures(0) {}

    int getHealth() const {
        return health;
    }

    void reduceHealth(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void incrementFailures() {
        playerFailures++;
    }

    int getPlayerFailures() const {
        return playerFailures;
    }
};

// ------------------------------------------------------------
// STRUCTURE DATA (STATIC GAME STATE)
// ------------------------------------------------------------

static std::unordered_map<std::string, std::pair<int,int>> structures = {
    {"Knight Camp",  {200, 1}},
    {"Mage Tower",   {500, 1}},
    {"Defence Tower",{200, 1}},
    {"Saint Tower",  {300, 1}}
};

// ------------------------------------------------------------
// VALID POINTER REGISTRY (PREVENTS DOUBLE FREE / UAF)
// ------------------------------------------------------------

static std::unordered_set<Dragon*> validDragons;

// ------------------------------------------------------------
// SAFE ACCESS MACROS
// ------------------------------------------------------------

#define DRAGON_OR_RETURNVALUE(ptr, retValue) \
    Dragon* dragon = reinterpret_cast<Dragon*>(ptr); \
    if (!dragon || validDragons.count(dragon) == 0) return retValue;

#define DRAGON_OR_RETURNVOID(ptr) \
    Dragon* dragon = reinterpret_cast<Dragon*>(ptr); \
    if (!dragon || validDragons.count(dragon) == 0) return;


// ------------------------------------------------------------
// DAMAGE + LOGIC HELPERS
// ------------------------------------------------------------

int randomDamage(int min, int max) {
    return rand() % (max - min + 1) + min;
}

bool useDefenseTower() {
    auto &d = structures["Defence Tower"];
    if (d.first > 0 && d.second > 0) {
        d.first = 0;
        return true;
    }
    return false;
}

void dragonRandomAttack() {
    if (useDefenseTower()) return;

    std::vector<std::string> active;
    for (auto &s : structures)
        if (s.second.first > 0 && s.second.second > 0)
            active.push_back(s.first);

    if (active.empty()) return;

    std::string target = active[rand() % active.size()];
    int dmg = randomDamage(50,150);
    structures[target].first -= dmg;
    if (structures[target].first < 0)
        structures[target].first = 0;
}


// ------------------------------------------------------------
// JNI IMPLEMENTATION
// ------------------------------------------------------------

extern "C" {

// --------------------- CREATE DRAGON -------------------------
JNIEXPORT jlong JNICALL Java_src_DragonGameBackendJNI_createDragon
  (JNIEnv *env, jobject obj)
{
    Dragon* d = new Dragon();
    validDragons.insert(d);
    return reinterpret_cast<jlong>(d);
}

// --------------------- DESTROY DRAGON ------------------------
JNIEXPORT void JNICALL Java_src_DragonGameBackendJNI_destroyDragon
  (JNIEnv *env, jobject obj, jlong dragonPtr)
{
    Dragon* d = reinterpret_cast<Dragon*>(dragonPtr);

    if (!d || validDragons.count(d) == 0)
        return;

    validDragons.erase(d);
    delete d;
}

// --------------------- GET HEALTH -----------------------------
JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_getDragonHealth
  (JNIEnv *env, jobject obj, jlong dragonPtr)
{
    DRAGON_OR_RETURNVALUE(dragonPtr, -1);
    return dragon->getHealth();
}

// --------------------- ATTACK DRAGON --------------------------
JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_attackDragonWithDamage
  (JNIEnv *env, jobject obj, jlong basePtr, jlong dragonPtr, jint move)
{
    DRAGON_OR_RETURNVALUE(dragonPtr, -1);

    std::string structure;
    if (move == 1) structure = "Knight Camp";
    else if (move == 2) structure = "Mage Tower";
    else if (move == 3) structure = "Saint Tower";
    else return -1;

    auto &entry = structures[structure];
    if (entry.second <= 0 || entry.first <= 0) return -1;

    int dmg = 0;

    if (structure == "Knight Camp")
        dmg = randomDamage(200,500);
    else if (structure == "Mage Tower")
        dmg = randomDamage(500,800);
    else if (structure == "Saint Tower") {
        structures["Knight Camp"].first =
            std::min(500, structures["Knight Camp"].first + 100);
        structures["Mage Tower"].first =
            std::min(800, structures["Mage Tower"].first + 200);
    }

    dragon->reduceHealth(dmg);
    return dmg;
}

// --------------------- DRAGON RANDOM ATTACK -------------------
JNIEXPORT void JNICALL Java_src_DragonGameBackendJNI_dragonAttack
  (JNIEnv *env, jobject obj, jlong basePtr, jlong dragonPtr)
{
    // basePtr unused — no crash if invalid
    dragonRandomAttack();
}

// --------------------- ALL STRUCTURES DESTROYED ---------------
JNIEXPORT jboolean JNICALL Java_src_DragonGameBackendJNI_allStructuresDestroyed
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    for (auto &s : structures)
        if (s.second.first > 0 && s.second.second > 0)
            return JNI_FALSE;
    return JNI_TRUE;
}

// --------------------- GET STRUCTURE STATUS --------------------
JNIEXPORT jstring JNICALL Java_src_DragonGameBackendJNI_getStructureStatus
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    std::string out;

    for (auto &s : structures) {
        if (s.second.second > 0)
            out += s.first + ": " + std::to_string(s.second.first)
                 + " HP, Count: " + std::to_string(s.second.second) + "\n";
    }

    if (out.empty()) out = "No structures remaining.";
    return env->NewStringUTF(out.c_str());
}

// --------------------- GET AVAILABLE STRUCTURES ----------------
JNIEXPORT jstring JNICALL Java_src_DragonGameBackendJNI_getAvailableStructures
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    std::string out;
    int id = 1;

    for (auto &s : structures)
        if (s.second.first > 0 && s.second.second > 0)
            out += std::to_string(id++) + ". " + s.first + "\n";

    if (out.empty()) out = "No structures available.";
    return env->NewStringUTF(out.c_str());
}

// --------------------- REVIVE UNITS ----------------------------
JNIEXPORT jboolean JNICALL Java_src_DragonGameBackendJNI_reviveUnits
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    structures["Knight Camp"].first =
        std::min(500, structures["Knight Camp"].first + 100);
    structures["Mage Tower"].first =
        std::min(800, structures["Mage Tower"].first + 200);

    return JNI_TRUE;
}

// --------------------- GET PLAYER FAILURES ---------------------
JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_getPlayerFailures
  (JNIEnv *env, jobject obj, jlong dragonPtr)
{
    DRAGON_OR_RETURNVALUE(dragonPtr, -1);
    return dragon->getPlayerFailures();
}

} // extern "C"
