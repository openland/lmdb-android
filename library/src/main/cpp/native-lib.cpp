#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_openland_lmdb_LMDB_testString(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from LMDB";
    return env->NewStringUTF(hello.c_str());
}
