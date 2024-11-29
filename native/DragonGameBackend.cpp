#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "src_DragonGameBackendJNI.h"

extern "C" {
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
            if (health < 0) {
                health = 0;
            }
        }

        void incrementFailures() {
            playerFailures++;
        }

        int getPlayerFailures() const {
            return playerFailures;
        }
    };

    static std::unordered_map<std::string, std::pair<int, int>> structures = {
        {"Knight Camp", {200, 1}}, // {Health, Count}
        {"Mage Tower", {500, 1}},
        {"Defence Tower", {200, 1}},
        {"Saint Tower", {300, 1}}
    };

    int randomDamage(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    bool useDefenseTower() {
        for (auto& structure : structures) {
            if (structure.first == "Defence Tower" && structure.second.first > 0) {
                structure.second.first = 0; // Mark as used
                return true;
            }
        }
        return false;
    }

    void dragonRandomAttack() {
        if (useDefenseTower()) {
            std::cout << "Dragon's attack was blocked by a Defence Tower!" << std::endl;
            return;
        }

        std::vector<std::string> existingStructures;
        for (const auto& structure : structures) {
            if (structure.second.first > 0 && structure.second.second > 0) { // Health > 0 and Count > 0
                existingStructures.push_back(structure.first);
            }
        }

        if (!existingStructures.empty()) {
            int targetIndex = rand() % existingStructures.size();
            std::string target = existingStructures[targetIndex];

            int damage = randomDamage(50, 150);
            structures[target].first -= damage;
            if (structures[target].first < 0) {
                structures[target].first = 0;
            }

            std::cout << "Dragon attacked " << target << " dealing " << damage << " damage!" << std::endl;
        } else {
            std::cout << "No existing structures to attack!" << std::endl;
        }
    }

    JNIEXPORT jlong JNICALL Java_src_DragonGameBackendJNI_createDragon(JNIEnv *env, jobject obj) {
        Dragon* dragon = new Dragon();
        return reinterpret_cast<jlong>(dragon);
    }

    JNIEXPORT void JNICALL Java_src_DragonGameBackendJNI_destroyDragon(JNIEnv *env, jobject obj, jlong dragonPtr) {
        Dragon* dragon = reinterpret_cast<Dragon*>(dragonPtr);
        delete dragon;
    }

    JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_getDragonHealth(JNIEnv *env, jobject obj, jlong dragonPtr) {
        Dragon* dragon = reinterpret_cast<Dragon*>(dragonPtr);
        return dragon->getHealth();
    }

    JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_attackDragonWithDamage(JNIEnv *env, jobject obj, jlong basePtr, jlong dragonPtr, jint move) {
        Dragon* dragon = reinterpret_cast<Dragon*>(dragonPtr);
        auto it = structures.begin();
        std::advance(it, move - 1);

        if (it == structures.end() || it->second.first <= 0 || it->second.second <= 0) {
            return -1;
        }

        const std::string& structure = it->first;
        int damage = 0;

        if (structure == "Knight Camp") {
            damage = randomDamage(200, 500);
        } else if (structure == "Mage Tower") {
            damage = randomDamage(500, 800);
        } else if (structure == "Saint Tower") {
            structures["Knight Camp"].first = std::min(500, structures["Knight Camp"].first + 100);
            structures["Mage Tower"].first = std::min(800, structures["Mage Tower"].first + 200);
        }

        dragon->reduceHealth(damage);
        return damage;
    }

    JNIEXPORT void JNICALL Java_src_DragonGameBackendJNI_dragonAttack(JNIEnv *env, jobject obj, jlong basePtr, jlong dragonPtr) {
        dragonRandomAttack();
    }

    JNIEXPORT jboolean JNICALL Java_src_DragonGameBackendJNI_allStructuresDestroyed(JNIEnv *env, jobject obj, jlong basePtr) {
        for (const auto& structure : structures) {
            if (structure.second.first > 0 && structure.second.second > 0) {
                return JNI_FALSE; // At least one structure is still standing
            }
        }
        return JNI_TRUE; // All structures are destroyed
    }

    JNIEXPORT jstring JNICALL Java_src_DragonGameBackendJNI_getStructureStatus(JNIEnv *env, jobject obj, jlong basePtr) {
        std::string status;
        for (const auto& structure : structures) {
            if (structure.second.second > 0) { // Include only structures with Count > 0
                status += structure.first + ": " + std::to_string(structure.second.first) + " HP, Count: " + std::to_string(structure.second.second) + "\n";
            }
        }
        return env->NewStringUTF(status.empty() ? "No structures remaining." : status.c_str());
    }

    JNIEXPORT jstring JNICALL Java_src_DragonGameBackendJNI_getAvailableStructures(JNIEnv *env, jobject obj, jlong basePtr) {
        std::string available;
        int id = 1; // Assign numeric IDs for player input
        for (const auto& structure : structures) {
            if (structure.second.first > 0 && structure.second.second > 0) { // Health > 0 and Count > 0
                available += std::to_string(id++) + ". " + structure.first + " (" + std::to_string(structure.second.first) + " HP)\n";
            }
        }
        return env->NewStringUTF(available.empty() ? "No structures available." : available.c_str());
    }

    JNIEXPORT jboolean JNICALL Java_src_DragonGameBackendJNI_reviveUnits(JNIEnv *env, jobject obj, jlong basePtr) {
        structures["Knight Camp"].first = std::min(500, structures["Knight Camp"].first + 100);
        structures["Mage Tower"].first = std::min(800, structures["Mage Tower"].first + 200);
        return JNI_TRUE;
    }
    JNIEXPORT jint JNICALL Java_src_DragonGameBackendJNI_getPlayerFailures(JNIEnv *env, jobject obj, jlong dragonPtr) {
        Dragon* dragon = reinterpret_cast<Dragon*>(dragonPtr);
        return dragon->getPlayerFailures();
    }
}
