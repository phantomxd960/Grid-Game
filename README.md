# GridHero

The Grid Hero is a strategic pathfinding game where the player must navigate through a
grid, avoiding an enemy that dynamically finds the shortest path to capture them. The
player's goal is to collect gold coins on the grid before being caught by the enemy. These
coins are used to progress through three levels of increasing difficulty and use them in the
final boss battle.Once the two levels are completed, the player engages in a final battle with
a dragon, marking the end of the game.

## Instructions

Instructions to compile and run the code:

**Step 1. Clone the Repository.**

**Step 2. The structure of Repository should like this:**

        GridHero
        |
        |--cli
        |__controller

**Step 3: Compile and Build Backend (C++)**

1. Command: cd controller

2. Command:

g++ -std=c++17 -o game player/Player.cpp enemy/Enemy.cpp enemy/Dragon.cpp Game.cpp main.cpp inventory/Ammo.cpp inventory/Storage.cpp

3. Command:

For linux:

g++ -std=c++17 -shared -fPIC -o libgamebackend.so main.cpp
player/Player.cpp enemy/Enemy.cpp enemy/Dragon.cpp Game.cpp
inventory/Ammo.cpp inventory/Storage.cpp

For macOS:

g++ -std=c++17 -shared -fPIC -o libgamebackend.dylib main.cpp
player/Player.cpp enemy/Enemy.cpp enemy/Dragon.cpp Game.cpp
inventory/Ammo.cpp inventory/Storage.cpp

For Windows:

g++ -std=c++17 -shared -fPIC -o libgamebackend.dll main.cpp
player/Player.cpp enemy/Enemy.cpp enemy/Dragon.cpp Game.cpp
inventory/Ammo.cpp inventory/Storage.cpp

4. Move this file to cli. The path of the file should be: ../GridHero/cli/libgamebackend.dylib/.so/.dll (same as Frontend.java)

**Step 4: Set Up Frontend**

1. Command: cd cli

2. Command: javac Frontend.java

**Step 5: To run the program**

1. cd GridHero/cli

2. java -Djava.library.path=. Frontend

**This will run the program in your terminal.**
