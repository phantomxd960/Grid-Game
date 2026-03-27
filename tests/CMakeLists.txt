cmake_minimum_required(VERSION 3.10)
project(JavaCppBackendTests)

# ----------------------------
# 1. GoogleTest
# ----------------------------
add_subdirectory(googletest)

# ----------------------------
# 2. Java + JNI
# ----------------------------
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include_directories(
    ${JNI_INCLUDE_DIRS}
    ../native
)

# ----------------------------
# 3. Test executable
# ----------------------------
add_executable(runTests
    test_main.cpp            # IMPORTANT (contains JVM creation)
    test_basebackend.cpp
    test_dragongamebackend.cpp
    test_gridgamebackend.cpp
    jni_env.cpp
    ../native/BaseBackend.cpp
    ../native/DragonGameBackend.cpp
    ../native/GridGameBackend.cpp
)

# ----------------------------
# 4. Link libraries
# ----------------------------
target_link_libraries(runTests
    gtest
    gtest_main
    ${JNI_LIBRARIES}          # This links libjvm.so
)

# ----------------------------
# 5. Testing
# ----------------------------
enable_testing()
add_test(NAME runTests COMMAND runTests)
