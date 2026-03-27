#include <gtest/gtest.h>
#include <jni.h>
#include "../native/src_DragonGameBackendJNI.h"

extern JNIEnv* env;

// Utility for fresh test runs
void resetState() {
    // Knight/Mage reset HP with reviveUnits
    Java_src_DragonGameBackendJNI_reviveUnits(env, nullptr, 0);

    // Force-reset Defence and Saint Tower by manually restoring HP
    // by calling getStructureStatus and reconstructing logic.
    // But since we cannot modify global map directly from tests,
    // we do this trick: we revive twice and then artificially consume nothing.
    // The simplest approach: call reviveUnits multiple times.
    Java_src_DragonGameBackendJNI_reviveUnits(env, nullptr, 0);
}

//
// 1. DRAGON CREATION & MEMORY TESTS
//
TEST(DragonTest, CreateAndDestroyDragon) {
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);
    ASSERT_NE(ptr, 0);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

TEST(DragonTest, DestroyDragonTwiceDoesNotCrash) {
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr); // must not crash

    SUCCEED();
}

//
// 2. HEALTH TESTS
//
TEST(DragonTest, InitialHealthIs4000) {
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    EXPECT_EQ(Java_src_DragonGameBackendJNI_getDragonHealth(env, nullptr, ptr), 4000);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

TEST(DragonTest, AttackKnightCampDeals200To500Damage) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    int dmg = Java_src_DragonGameBackendJNI_attackDragonWithDamage(env, nullptr, 0, ptr, 1);

    EXPECT_GE(dmg, 200);
    EXPECT_LE(dmg, 500);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

TEST(DragonTest, AttackMageTowerDeals500To800Damage) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    int dmg = Java_src_DragonGameBackendJNI_attackDragonWithDamage(env, nullptr, 0, ptr, 2);

    EXPECT_GE(dmg, 500);
    EXPECT_LE(dmg, 800);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

TEST(DragonTest, SaintTowerHealsUnitsNoDragonDamage) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    int dmg = Java_src_DragonGameBackendJNI_attackDragonWithDamage(env, nullptr, 0, ptr, 3);

    EXPECT_EQ(dmg, 0);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

//
// 3. INVALID INPUT TESTS
//
TEST(DragonTest, InvalidMoveReturnsMinusOne) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    EXPECT_EQ(Java_src_DragonGameBackendJNI_attackDragonWithDamage(env, nullptr, 0, ptr, 99), -1);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

TEST(DragonTest, NullDragonPointerReturnsSafeValue) {
    EXPECT_EQ(Java_src_DragonGameBackendJNI_getDragonHealth(env, nullptr, 0), -1);
}

//
// 4. STRUCTURE STATUS & AVAILABILITY
//
TEST(DragonTest, StructureStatusReturnsString) {
    resetState();

    jstring s = Java_src_DragonGameBackendJNI_getStructureStatus(env, nullptr, 0);
    const char* c = env->GetStringUTFChars(s, nullptr);

    EXPECT_STRNE(c, "");

    env->ReleaseStringUTFChars(s, c);
}

TEST(DragonTest, AvailableStructuresListNotEmpty) {
    resetState();

    jstring s = Java_src_DragonGameBackendJNI_getAvailableStructures(env, nullptr, 0);
    const char* c = env->GetStringUTFChars(s, nullptr);

    EXPECT_STRNE(c, "No structures available.");

    env->ReleaseStringUTFChars(s, c);
}

//
// 5. DRAGON RANDOM ATTACK TEST
//
TEST(DragonTest, DragonRandomAttackDoesNotCrash) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    Java_src_DragonGameBackendJNI_dragonAttack(env, nullptr, 0, ptr);

    SUCCEED();
}

//
// 6. ALL STRUCTURES DESTROYED TEST
//
TEST(DragonTest, AllStructuresDestroyedFalseInitially) {
    resetState();

    EXPECT_EQ(Java_src_DragonGameBackendJNI_allStructuresDestroyed(env, nullptr, 0), JNI_FALSE);
}

//
// 7. PLAYER FAILURE COUNTER
//
TEST(DragonTest, PlayerFailureCounterStartsAtZero) {
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    EXPECT_EQ(Java_src_DragonGameBackendJNI_getPlayerFailures(env, nullptr, ptr), 0);

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}

//
// 8. STRESS TEST
//
TEST(DragonTest, StressAttackTest) {
    resetState();
    jlong ptr = Java_src_DragonGameBackendJNI_createDragon(env, nullptr);

    for (int i = 0; i < 200; i++) {
        int dmg = Java_src_DragonGameBackendJNI_attackDragonWithDamage(env, nullptr, 0, ptr, 1);
        EXPECT_GE(dmg, 0);
    }

    Java_src_DragonGameBackendJNI_destroyDragon(env, nullptr, ptr);
}
