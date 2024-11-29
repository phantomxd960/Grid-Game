The setup is completed and a video with clean installation has been attached.

The clean_files is the folder without building anything

Setps to build :
1. Create a bin folder using the below command in the root folder\
`mkdir bin`
2. Generate header files for the JNI files from the *src* directory\
    `cd src`\
    `javac -h ../native BaseBackendJNI.java`\
    `javac -h ../native GridGameBackendJNI.java`
3. Navigate to the `native/` folder and compile the C++ files into shared libraries:\
`cd ../native`\
`g++ -shared -o BaseBackend.dll -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" BaseBackend.cpp`\
`g++ -shared -o GridGameBackend.dll -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" GridGameBackend.cpp`
`g++ -shared -o DragonGameBackend.dll -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" DragonGameBackend.cpp`
4. Navigate to the root folder and compile the java files into class files:\
`javac -d bin src/*.java`

5. navigate to the root folder and run the file with path to native libraries\
`java -Djava.library.path=native -cp bin src.FinalBaseFrontend`




C:\Program Files\Java\jdk-20