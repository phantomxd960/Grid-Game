🔧 Project Setup Instructions
1. Install Dependencies

Make sure you have the following installed:

g++ (C++ compiler)

JDK 23 (both java and javac)

Swing (included in standard JDK)

Linux (project tested on Ubuntu)

Check installations:

java -version
javac -version
g++ --version


Both java and javac should show version 23.0.x.

2. Clone the Repository
git clone <YOUR_REPO_LINK>
cd final_java_cpp_project

3. Set JAVA_HOME
export JAVA_HOME=/usr/lib/jvm/jdk-23.0.1-oracle-x64

4. Compile Java Sources
rm -rf bin
mkdir bin
javac -h native -d bin src/*.java


This:

Compiles .java files

Generates JNI header files inside native/

5. Compile Native C++ Code (.so file)

Move into the native folder:

cd native
g++ -fPIC -shared -o libGridGameBackend.so \
    -I"$JAVA_HOME/include" \
    -I"$JAVA_HOME/include/linux" \
    GridGameBackend.cpp
cd ..

6. Run the Project
export LD_LIBRARY_PATH=$PWD/native
java -cp bin src.FinalBaseFrontend


The GUI will launch automatically.

🧪 Testing Instructions
sudo apt install cmake

Commands to Run Tests:

sudo apt update
sudo apt install g++ cmake git

cd tests
cmake --build build
cd build
./runTests
This executes all GoogleTest suites included in the project.




Enemy moves closer every turn (BFS sh
