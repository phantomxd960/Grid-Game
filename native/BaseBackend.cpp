#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "src_BaseBackendJNI.h"

// ------------------------------------------------------------
//  BaseBackend CLASS
// ------------------------------------------------------------
class BaseBackend {
private:
    int goldCoins;
    int townhallLevel;
    std::unordered_map<std::string, int> structureCosts;
    std::unordered_map<std::string, int> structures;

    void initializeStructureCosts() {
        structureCosts = {
            {"Townhall",     200 + townhallLevel * 150},
            {"Knight Camp",  200},
            {"Mage Tower",   300},
            {"Defence Tower",500},
            {"Saint Tower",  400}
        };
    }

public:
    BaseBackend() : goldCoins(0), townhallLevel(0) {
        initializeStructureCosts();
    }

    void addGold(int amount) {
        goldCoins += amount;
    }

    int getGold() const {
        return goldCoins;
    }

    int getTownhallLevel() const {
        return townhallLevel;
    }

    int getStructureCost(const std::string &structureName) const {
        auto it = structureCosts.find(structureName);
        return (it != structureCosts.end()) ? it->second : -1;
    }

    bool hasTownhall() const {
        return townhallLevel > 0;
    }

    int getStructureCount(const char *structureName) const {
        std::string name(structureName);
        if (structures.find(name) != structures.end()) {
            return structures.at(name);
        }
        return 0;
    }

    int getMaxLimit(const std::string &structureName) const {
        // max limits per townhall level 1..5
        static const int maxLimits[5][4] = {
            {2, 1, 0, 0},
            {4, 2, 1, 1},
            {6, 3, 1, 2},
            {8, 4, 2, 2},
            {10,5, 2, 3}
        };

        if (townhallLevel == 0) return 0;

        if (structureName == "Knight Camp")   return maxLimits[townhallLevel-1][0];
        if (structureName == "Mage Tower")    return maxLimits[townhallLevel-1][1];
        if (structureName == "Defence Tower") return maxLimits[townhallLevel-1][2];
        if (structureName == "Saint Tower")   return maxLimits[townhallLevel-1][3];

        return 0;
    }

    int build(const char *structureName, int cost) {
        std::string structure(structureName);

        if (structure == "Townhall") {
            if (townhallLevel >= 5) return -2;
            if (goldCoins < cost)   return -1;
            goldCoins -= cost;
            townhallLevel++;
            initializeStructureCosts();
            return 1;
        }

        if (townhallLevel == 0) return -2;

        int currentCount = structures[structure];
        int maxLimit = getMaxLimit(structure);
        if (currentCount >= maxLimit) return -2;
        if (goldCoins < cost) return -1;

        goldCoins -= cost;
        structures[structure]++;
        return 1;
    }
};

// ------------------------------------------------------------
// VALID POINTER REGISTRY (PREVENTS DOUBLE DELETE + USE AFTER FREE)
// ------------------------------------------------------------
static std::unordered_set<BaseBackend*> validBases;

// ------------------------------------------------------------
// SAFE POINTER ACCESS MACROS
// ------------------------------------------------------------

#define BASE_OR_RETURNVALUE(basePtr, retValue) \
    BaseBackend* base = reinterpret_cast<BaseBackend*>(basePtr); \
    if (!base || validBases.count(base) == 0) return retValue;

#define BASE_OR_RETURNVOID(basePtr) \
    BaseBackend* base = reinterpret_cast<BaseBackend*>(basePtr); \
    if (!base || validBases.count(base) == 0) return;


// ------------------------------------------------------------
// JNI IMPLEMENTATION
// ------------------------------------------------------------

extern "C" {

// --------------------- CREATE BASE --------------------------
JNIEXPORT jlong JNICALL Java_src_BaseBackendJNI_createBase
  (JNIEnv *env, jobject obj)
{
    BaseBackend *base = new BaseBackend();
    validBases.insert(base);
    return reinterpret_cast<jlong>(base);
}

// --------------------- DESTROY BASE --------------------------
JNIEXPORT void JNICALL Java_src_BaseBackendJNI_destroyBase
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    BaseBackend* base = reinterpret_cast<BaseBackend*>(basePtr);

    if (!base || validBases.count(base) == 0)
        return;

    validBases.erase(base);
    delete base;
}

// --------------------- ADD GOLD ------------------------------
JNIEXPORT void JNICALL Java_src_BaseBackendJNI_addGold
  (JNIEnv *env, jobject obj, jlong basePtr, jint amount)
{
    BASE_OR_RETURNVOID(basePtr);
    base->addGold(amount);
}

// --------------------- BUILD STRUCTURE -----------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_build
  (JNIEnv *env, jobject obj, jlong basePtr, jstring structureName, jint cost)
{
    BASE_OR_RETURNVALUE(basePtr, -1);

    const char *name = env->GetStringUTFChars(structureName, nullptr);
    int result = base->build(name, cost);
    env->ReleaseStringUTFChars(structureName, name);
    return result;
}

// --------------------- GET GOLD ------------------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getGold
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    BASE_OR_RETURNVALUE(basePtr, -1);
    return base->getGold();
}

// --------------------- TOWNHALL LEVEL ------------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getTownhallLevel
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    BASE_OR_RETURNVALUE(basePtr, -1);
    return base->getTownhallLevel();
}

// --------------------- GET STRUCTURE COUNT -------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getStructureCount
  (JNIEnv *env, jobject obj, jlong basePtr, jstring structureName)
{
    BASE_OR_RETURNVALUE(basePtr, -1);

    const char *name = env->GetStringUTFChars(structureName, nullptr);
    int count = base->getStructureCount(name);
    env->ReleaseStringUTFChars(structureName, name);
    return count;
}

// --------------------- HAS TOWNHALL --------------------------
JNIEXPORT jboolean JNICALL Java_src_BaseBackendJNI_hasTownhall
  (JNIEnv *env, jobject obj, jlong basePtr)
{
    BASE_OR_RETURNVALUE(basePtr, JNI_FALSE);
    return base->hasTownhall();
}

// --------------------- GET STRUCTURE COST --------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getStructureCost
  (JNIEnv *env, jobject obj, jlong basePtr, jstring structureName)
{
    BASE_OR_RETURNVALUE(basePtr, -1);

    const char *name = env->GetStringUTFChars(structureName, nullptr);
    int cost = base->getStructureCost(name);
    env->ReleaseStringUTFChars(structureName, name);
    return cost;
}

// --------------------- GET MAX LIMIT -------------------------
JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getMaxLimit
  (JNIEnv *env, jobject obj, jlong basePtr, jstring structureName)
{
    BASE_OR_RETURNVALUE(basePtr, -1);

    const char *name = env->GetStringUTFChars(structureName, nullptr);
    std::string structure(name);
    env->ReleaseStringUTFChars(structureName, name);

    return base->getMaxLimit(structure);
}

} // extern "C"

