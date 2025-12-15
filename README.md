

---


## **Project Title**

**Grid Game with Java Swing UI and C++ Backend via JNI**

---

## 📖 **Project Overview**

This project implements a **grid-based game** where:

* The **UI is developed in Java Swing**
* The **backend logic (enemy movement)** is implemented in **C++**
* Java and C++ communicate using **JNI (Java Native Interface)**

### **Key Features**

* Dijkstra Algo based enemy movement algorithm written in C++
* Java Swing GUI for real-time interaction
* Random wall generation with guaranteed path availability
* Detection of repetitive player movement patterns
* Modular, clean C++ backend logic
* GoogleTest-based unit testing for core game logic.

---

## **Project Structure**

```
final_java_cpp_project/
│
├── src/                     # Java sources
│   ├── GridGameGUI.java
│   ├── FinalBaseFrontend.java
│   └── ...
│
├── native/                  # C++ code + JNI headers + .so file
│   ├── GridGameBackend.cpp
│   ├── src_GridGameGUI.h
│   ├── libGridGameBackend.so
│   └── ...
│
├── tests/                   # GoogleTest-based testing framework
│   ├── CMakeLists.txt
│   ├── test_main.cpp
│   ├── test_basebackend.cpp
│   ├── test_gridgame.cpp
│   └── ...
│
├── images/                  # Icons for the game GUI
│
└── README.md
```

---

#**Project Setup Instructions**

## **1. Install Dependencies**

```bash
sudo apt update
sudo apt install g++ cmake git
```

Install JDK 23:

```bash
java -version
javac -version
```

Both must show **23.x**.

---

## **2. Clone the Repository**

```bash
git clone <REPO_LINK>
cd final_java_cpp_project
```

---

## **3. Set JAVA_HOME**

```bash
export JAVA_HOME=/usr/lib/jvm/jdk-23.0.1-oracle-x64
```

---

## **4. Compile Java & Generate JNI Headers**

```bash
rm -rf bin
mkdir bin
javac -h native -d bin src/*.java
```

This generates:

* `.class` files → `bin/`
* `.h` JNI headers → `native/`

---

## **5. Compile Native C++ Backend (.so file)**

```bash
cd native

g++ -fPIC -shared -o libGridGameBackend.so \
    -I"$JAVA_HOME/include" \
    -I"$JAVA_HOME/include/linux" \
    GridGameBackend.cpp
do it for all native files
cd ..
```

This creates:

```
native/libGridGameBackend.so
native/libDragonGameBackend.so
native/libBaseBackend.so
```

---

## **6. Run the Project**

```bash
export LD_LIBRARY_PATH=$PWD/native
java -cp bin src.FinalBaseFrontend
```

---

# **How the Game Works**

* **Player** moves using the UI buttons (Up/Down/Left/Right)
* **Enemy** chases the player using **Dijsktra Algorithm for shortest path** (in C++)
* Random **walls** are generated each game
* Game ends when the enemy reaches the player
* depending on the number of moves you get coins and build structures to reach various town hall levels.
* on reaching town hall level 5 you fight the dragon battle.

---

# **Testing Instructions (GoogleTest)**

## **1. Install CMake**

```bash
sudo apt install cmake
```

---

## **2. Build and Run Tests**

```bash
cd tests
mkdir build
cmake -S . -B build
cmake --build build
cd build
./runTests
```

