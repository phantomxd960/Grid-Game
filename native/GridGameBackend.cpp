#include <vector>
#include <queue>
#include <utility>
#include "src_GridGameBackendJNI.h"

#define GRID_SIZE 10
#define INF 1000000

bool isValidPosition(int x, int y, std::vector<std::vector<char>> &grid) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[x][y] != 'x';
}

extern "C" JNIEXPORT jintArray JNICALL Java_src_GridGameGUI_calculateEnemyMove(
    JNIEnv *env,
    jobject obj,
    jobjectArray jGrid,
    jintArray jPlayerPos,
    jintArray jEnemyPos) {
    // Process the Java grid into a C++ grid
    std::vector<std::vector<char>> grid(GRID_SIZE, std::vector<char>(GRID_SIZE));
    for (int i = 0; i < GRID_SIZE; i++) {
        jcharArray row = (jcharArray)env->GetObjectArrayElement(jGrid, i);
        jchar *rowData = env->GetCharArrayElements(row, nullptr);
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = (char)rowData[j];
        }
        env->ReleaseCharArrayElements(row, rowData, JNI_ABORT);
    }

    // Process positions
    jint *playerPos = env->GetIntArrayElements(jPlayerPos, nullptr);
    jint *enemyPos = env->GetIntArrayElements(jEnemyPos, nullptr);

    // Calculate enemy move
    std::vector<std::vector<int>> dist(GRID_SIZE, std::vector<int>(GRID_SIZE, INF));
    std::queue<std::pair<int, int>> queue;

    queue.push({playerPos[0], playerPos[1]});
    dist[playerPos[0]][playerPos[1]] = 0;

    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    while (!queue.empty()) {
        auto [cx, cy] = queue.front();
        queue.pop();
        for (auto &dir : directions) {
            int nx = cx + dir[0], ny = cy + dir[1];
            if (isValidPosition(nx, ny, grid) && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[cx][cy] + 1;
                queue.push({nx, ny});
            }
        }
    }

    int bestX = enemyPos[0], bestY = enemyPos[1], shortestDist = INF;
    for (auto &dir : directions) {
        int nx = enemyPos[0] + dir[0], ny = enemyPos[1] + dir[1];
        if (isValidPosition(nx, ny, grid) && dist[nx][ny] < shortestDist) {
            bestX = nx;
            bestY = ny;
            shortestDist = dist[nx][ny];
        }
    }

    // Return new enemy position
    jintArray result = env->NewIntArray(2);
    jint outPos[2] = {bestX, bestY};
    env->SetIntArrayRegion(result, 0, 2, outPos);

    env->ReleaseIntArrayElements(jPlayerPos, playerPos, JNI_ABORT);
    env->ReleaseIntArrayElements(jEnemyPos, enemyPos, JNI_ABORT);
    return result;
}