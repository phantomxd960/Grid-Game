#include <iostream>
#include <unordered_map>
#include <string>
#include "src_BaseBackendJNI.h"

extern "C" {
    class BaseBackend {
    private:
        int goldCoins;
        int townhallLevel;
        std::unordered_map<std::string, int> structureCosts;
        std::unordered_map<std::string, int> structures;


        void initializeStructureCosts() {
            structureCosts = {
                {"Townhall", 200 + townhallLevel * 150},
                {"Knight Camp", 200},
                {"Mage Tower", 300},
                {"Defence Tower", 500},
                {"Saint Tower", 400}
            };
        }

    public:
        int getMaxLimit(const std::string &structureName) const {
            static const int maxLimits[5][4] = {
                {2, 1, 0, 0},
                {4, 2, 1, 1},
                {6, 3, 1, 2},
                {8, 4, 2, 2},
                {10, 5, 2, 3}
            };

            if (structureName == "Knight Camp") return maxLimits[townhallLevel - 1][0];
            if (structureName == "Mage Tower") return maxLimits[townhallLevel - 1][1];
            if (structureName == "Defence Tower") return maxLimits[townhallLevel - 1][2];
            if (structureName == "Saint Tower") return maxLimits[townhallLevel - 1][3];
            return 0;
        
        }
        BaseBackend() : goldCoins(0), townhallLevel(0) {
            initializeStructureCosts();
        }

        void addGold(int amount) {
            goldCoins += amount;
        }

        int build(const char *structureName, int cost) {
            std::string structure(structureName);

            if (structure == "Townhall") {
                if (townhallLevel >= 5) return -2;
                if (goldCoins < cost) return -1;
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

        int getGold() const {
            return goldCoins;
        }

        int getTownhallLevel() const {
            return townhallLevel;
        }

        int getStructureCount(const char *structureName) const {
            std::string structure(structureName);
            if (structures.find(structure) != structures.end()) {
                return structures.at(structure);
            }
            return 0;
        }

        bool hasTownhall() const {
            return townhallLevel > 0;
        }

        int getStructureCost(const std::string &structureName) const {
            auto it = structureCosts.find(structureName);
            return (it != structureCosts.end()) ? it->second : -1;
        }
    };

    JNIEXPORT jlong JNICALL Java_src_BaseBackendJNI_createBase(JNIEnv *env, jobject obj) {
        BaseBackend *base = new BaseBackend();
        return reinterpret_cast<jlong>(base);
    }

    JNIEXPORT void JNICALL Java_src_BaseBackendJNI_destroyBase(JNIEnv *env, jobject obj, jlong basePtr) {
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        delete base;
    }

    JNIEXPORT void JNICALL Java_src_BaseBackendJNI_addGold(JNIEnv *env, jobject obj, jlong basePtr, jint amount) {
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        base->addGold(amount);
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_build(JNIEnv *env, jobject obj, jlong basePtr, jstring structureName, jint cost) {
        const char *structureNameCStr = env->GetStringUTFChars(structureName, nullptr);
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        int result = base->build(structureNameCStr, cost);
        env->ReleaseStringUTFChars(structureName, structureNameCStr);
        return result;
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getGold(JNIEnv *env, jobject obj, jlong basePtr) {
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        return base->getGold();
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getTownhallLevel(JNIEnv *env, jobject obj, jlong basePtr) {
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        return base->getTownhallLevel();
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getStructureCount(JNIEnv *env, jobject obj, jlong basePtr, jstring structureName) {
        const char *structureNameCStr = env->GetStringUTFChars(structureName, nullptr);
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        int count = base->getStructureCount(structureNameCStr);
        env->ReleaseStringUTFChars(structureName, structureNameCStr);
        return count;
    }

    JNIEXPORT jboolean JNICALL Java_src_BaseBackendJNI_hasTownhall(JNIEnv *env, jobject obj, jlong basePtr) {
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        return base->hasTownhall();
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getStructureCost(JNIEnv *env, jobject obj, jlong basePtr, jstring structureName) {
        const char *name = env->GetStringUTFChars(structureName, nullptr);
        BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
        int cost = base->getStructureCost(name);
        env->ReleaseStringUTFChars(structureName, name);
        return cost;
    }

    JNIEXPORT jint JNICALL Java_src_BaseBackendJNI_getMaxLimit(JNIEnv *env, jobject obj, jlong basePtr, jstring structureName) {
    const char *structureNameCStr = env->GetStringUTFChars(structureName, nullptr);
    std::string structure(structureNameCStr);
    env->ReleaseStringUTFChars(structureName, structureNameCStr);

    BaseBackend *base = reinterpret_cast<BaseBackend *>(basePtr);
    return base->getMaxLimit(structure); // Use the getMaxLimit method from the BaseBackend class
    }

}
