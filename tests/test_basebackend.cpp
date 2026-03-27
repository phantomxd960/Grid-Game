#include <gtest/gtest.h>
#include <jni.h>
#include "../native/src_BaseBackendJNI.h"

extern JNIEnv* env;

// Utility: convert C++ string → jstring
jstring JStr(const std::string &s)
{
    return env->NewStringUTF(s.c_str());
}

//
// 1. BASIC LIFECYCLE TESTS
//
TEST(BaseBackendTest, CreateAndDestroyBase)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);
    ASSERT_NE(base, 0);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

TEST(BaseBackendTest, BaseStartsWithZeroGoldAndZeroTownhall)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    EXPECT_EQ(Java_src_BaseBackendJNI_getGold(env, nullptr, base), 0);
    EXPECT_EQ(Java_src_BaseBackendJNI_getTownhallLevel(env, nullptr, base), 0);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

//
// 2. GOLD TESTS
//
TEST(BaseBackendTest, AddGoldIncreasesCorrectly)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 500);
    EXPECT_EQ(Java_src_BaseBackendJNI_getGold(env, nullptr, base), 500);

    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 300);
    EXPECT_EQ(Java_src_BaseBackendJNI_getGold(env, nullptr, base), 800);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

TEST(BaseBackendTest, GetGoldOnNullBaseReturnsMinusOne)
{
    EXPECT_EQ(Java_src_BaseBackendJNI_getGold(env, nullptr, 0), -1);
}

//
// 3. TOWNHALL BUILDING TESTS
//
TEST(BaseBackendTest, CannotBuildTownhallWithoutGold)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    int cost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Townhall"));
    int result = Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Townhall"), cost);

    EXPECT_EQ(result, -1);  // insufficient gold

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

TEST(BaseBackendTest, BuildTownhallSuccessfully)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 10000);

    int cost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Townhall"));
    int result = Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Townhall"), cost);

    EXPECT_EQ(result, 1);
    EXPECT_EQ(Java_src_BaseBackendJNI_getTownhallLevel(env, nullptr, base), 1);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

TEST(BaseBackendTest, TownhallCannotExceedLevel5)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);
    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 100000);

    for (int i = 0; i < 5; i++)
    {
        int cost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Townhall"));
        Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Townhall"), cost);
    }

    // Attempt to build 6th Townhall
    int cost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Townhall"));
    int result = Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Townhall"), cost);

    EXPECT_EQ(result, -2);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

//
// 4. STRUCTURE BUILDING TESTS
//
TEST(BaseBackendTest, CannotBuildStructuresBeforeTownhall)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);
    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 1000);

    int cost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Knight Camp"));
    int result = Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Knight Camp"), cost);

    EXPECT_EQ(result, -2);  // townhall not built

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

TEST(BaseBackendTest, BuildKnightCampWithinLimit)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    Java_src_BaseBackendJNI_addGold(env, nullptr, base, 10000);

    // Build Townhall first
    int thc = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Townhall"));
    Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Townhall"), thc);

    int knightCost = Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Knight Camp"));
    int maxLimit = Java_src_BaseBackendJNI_getMaxLimit(env, nullptr, base, JStr("Knight Camp"));

    for (int i = 0; i < maxLimit; i++)
    {
        EXPECT_EQ(Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Knight Camp"), knightCost), 1);
    }

    EXPECT_EQ(Java_src_BaseBackendJNI_getStructureCount(env, nullptr, base, JStr("Knight Camp")), maxLimit);

    // Exceeding limit
    EXPECT_EQ(Java_src_BaseBackendJNI_build(env, nullptr, base, JStr("Knight Camp"), knightCost), -2);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}

//
// 5. SAFETY TESTS
//
TEST(BaseBackendTest, UsingDestroyedBaseShouldReturnError)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);
    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);

    EXPECT_EQ(Java_src_BaseBackendJNI_getGold(env, nullptr, base), -1);
}

TEST(BaseBackendTest, DoubleDestroyIsSafe)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);

    SUCCEED();
}

//
// 6. COST TESTS
//
TEST(BaseBackendTest, StructureCostsArePositive)
{
    jlong base = Java_src_BaseBackendJNI_createBase(env, nullptr);

    EXPECT_GT(Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Knight Camp")), 0);
    EXPECT_GT(Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Mage Tower")), 0);
    EXPECT_GT(Java_src_BaseBackendJNI_getStructureCost(env, nullptr, base, JStr("Saint Tower")), 0);

    Java_src_BaseBackendJNI_destroyBase(env, nullptr, base);
}
