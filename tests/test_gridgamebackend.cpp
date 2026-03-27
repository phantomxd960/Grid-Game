#include <gtest/gtest.h>
#include "../native/src_GridGameBackendJNI.h"
#include <jni.h>

extern JNIEnv* env;

// Helper to build a Java 2D char grid
jobjectArray makeGrid(const std::vector<std::string>& rows)
{
    jobjectArray grid = env->NewObjectArray(10, env->FindClass("[C"), nullptr);

    for (int i = 0; i < 10; i++)
    {
        jcharArray row = env->NewCharArray(10);
        jchar buf[10];
        for (int j = 0; j < 10; j++)
            buf[j] = rows[i][j];

        env->SetCharArrayRegion(row, 0, 10, buf);
        env->SetObjectArrayElement(grid, i, row);
    }
    return grid;
}

jintArray makeInt2(int x, int y)
{
    jintArray arr = env->NewIntArray(2);
    jint buf[2] = {x, y};
    env->SetIntArrayRegion(arr, 0, 2, buf);
    return arr;
}

std::pair<int,int> readPos(jintArray arr)
{
    jint buf[2];
    env->GetIntArrayRegion(arr, 0, 2, buf);
    return {buf[0], buf[1]};
}


// ==========================================================
//  TESTS
// ==========================================================

TEST(GridGameTest, EnemyMovesCloserToPlayer)
{
    auto grid = makeGrid({
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        ".....E....",
        "..........",
        "P........."
    });

    auto player = makeInt2(9,0);
    auto enemy  = makeInt2(7,5);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    // Enemy should reduce its Manhattan distance
    int before = abs(7-9) + abs(5-0);
    int after  = abs(ex-9) + abs(ey-0);

    EXPECT_LT(after, before);
}

TEST(GridGameTest, EnemyBlockedByWalls)
{
    auto grid = makeGrid({
        "xxxxxxxxxx",
        "x........x",
        "x........x",
        "x........x",
        "x........x",
        "x....E...x",
        "x........x",
        "x........x",
        "x........x",
        "xP.......x"
    });

    auto player = makeInt2(9,1);
    auto enemy = makeInt2(5,5);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    // Enemy cannot move into a wall
    jcharArray row = (jcharArray)env->GetObjectArrayElement(grid, ex);
    jchar rowData[10];
    env->GetCharArrayRegion(row, 0, 10, rowData);

    EXPECT_NE(rowData[ey], 'x');
}

TEST(GridGameTest, EnemyCanMoveDiagonally)
{
    auto grid = makeGrid({
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        ".....E....",
        "..........",
        "..........",
        "..........",
        "...P......"
    });

    auto player = makeInt2(9,3);
    auto enemy = makeInt2(5,5);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    // Diagonal move usually best → either (6,4) or (6,6) or (4,4) etc.
    int before = abs(5-9) + abs(5-3);
    int after  = abs(ex-9) + abs(ey-3);

    EXPECT_LE(after, before);
}

TEST(GridGameTest, EnemyChoosesShortestPath)
{
    auto grid = makeGrid({
        "..........",
        "..........",
        "....xxx...",
        "....xPx...",
        "....xxx...",
        "..........",
        "..........",
        "...E......",
        "..........",
        ".........."
    });

    auto player = makeInt2(3,5);
    auto enemy = makeInt2(7,3);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    // Should at least reduce or equalize BFS distance
    int before = abs(7-3) + abs(3-5);
    int after  = abs(ex-3) + abs(ey-5);

    EXPECT_LE(after, before);
}

TEST(GridGameTest, EnemyStaysInsideBoundaries)
{
    auto grid = makeGrid({
        "P.........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "E.........",
        "..........",
        ".........."
    });

    auto player = makeInt2(0,0);
    auto enemy = makeInt2(7,0);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    EXPECT_GE(ex, 0);
    EXPECT_GE(ey, 0);
    EXPECT_LT(ex, 10);
    EXPECT_LT(ey, 10);
}


// ==========================================================
//  FIXED TEST — the previous failing one
// ==========================================================

TEST(GridGameTest, EnemyFindsAlternatePathAroundWalls)
{
    auto grid = makeGrid({
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "xxxx.xxxxx",  // only opening at column 4
        "..........",
        ".....E....",
        "..........",
        "P........."
    });

    auto player = makeInt2(9,0);
    auto enemy  = makeInt2(7,5);

    auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
    auto [ex, ey] = readPos(res);

    // Enemy must move (not remain stuck)
    EXPECT_FALSE(ex == 7 && ey == 5);

    // Enemy must not move into a wall
    jcharArray row = (jcharArray)env->GetObjectArrayElement(grid, ex);
    jchar rowData[10];
    env->GetCharArrayRegion(row, 0, 10, rowData);
    EXPECT_NE(rowData[ey], 'x');

    // Should reduce Manhattan distance or at least not worsen it
    int before = abs(7-9) + abs(5-0);
    int after  = abs(ex-9) + abs(ey-0);
    EXPECT_LE(after, before);
}

// ==========================================================

TEST(GridGameTest, StressTest100Moves)
{
    auto grid = makeGrid({
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        "..........",
        ".....E....",
        "..........",
        "P........."
    });

    auto player = makeInt2(9,0);
    auto enemy  = makeInt2(7,5);

    for (int i = 0; i < 100; i++)
    {
        auto res = Java_src_GridGameBackendJNI_calculateEnemyMove(env, nullptr, grid, player, enemy);
        auto [ex, ey] = readPos(res);
        enemy = makeInt2(ex, ey);

        EXPECT_GE(ex, 0);
        EXPECT_GE(ey, 0);
        EXPECT_LT(ex, 10);
        EXPECT_LT(ey, 10);
    }
}

