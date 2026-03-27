#include <jni.h>
#include <gtest/gtest.h>
#include <iostream>

JavaVM* jvm;
JNIEnv* env;

int main(int argc, char** argv) {

    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    // Set classpath to your compiled Java classes:
    options[0].optionString = (char*) "-Djava.class.path=./bin";

    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    if (res != JNI_OK) {
        std::cerr << "ERROR: Unable to launch JVM" << std::endl;
        return 1;
    }

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    jvm->DestroyJavaVM();
    return result;
}
