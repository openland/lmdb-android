#include <jni.h>
#include <string>
#include "lmdb/lmdb.h"

void throwLMDBError(JNIEnv *env, jint code) {
    if (code != MDB_SUCCESS) {
        const char *className = "com/openland/lmdb/LMDBException";
        char *message = mdb_strerror(code);
        jclass myExceptionClass = env->FindClass(className);

        jmethodID ctorMethod = env->GetMethodID(myExceptionClass, "<init>", "(Ljava/lang/String;I)V");
        jobject myExceptionObject = env->NewObject(myExceptionClass, ctorMethod, env->NewStringUTF(message), code);

        env->Throw((jthrowable) myExceptionObject);
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_openland_lmdb_LMDBJNI_testString(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from LMDB";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_openland_lmdb_LMDBJNI_createEnv(JNIEnv *env, jobject /* this */) {
    MDB_env *mdbEnv;
    if (const int c = mdb_env_create(&mdbEnv)) {
        throwLMDBError(env, c);
    }
    if (const int c = mdb_env_set_maxdbs(mdbEnv, 50)) {
        throwLMDBError(env, c);
    }
    return (long) mdbEnv;
}

extern "C" JNIEXPORT void JNICALL
Java_com_openland_lmdb_LMDBJNI_closeEnv(JNIEnv *env, jobject /* this */, jlong mdbEnv) {
    mdb_env_close((MDB_env *) mdbEnv);
}


extern "C" JNIEXPORT void JNICALL
Java_com_openland_lmdb_LMDBJNI_openEnv(JNIEnv *env, jobject /* this */, jlong mdbEnvRaw, jstring pathRaw) {
    MDB_env *mdbEnv = (MDB_env *) mdbEnvRaw;
    const char *path = env->GetStringUTFChars(pathRaw, nullptr);
    if (const int c = mdb_env_open(mdbEnv, path, MDB_NOSUBDIR /*|MDB_NOSYNC*/, 0664)) {
        throwLMDBError(env, c);
    }
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_openland_lmdb_LMDBJNI_beginTx(JNIEnv *env, jobject /* this */, jlong mdbEnvRaw, jboolean readonly) {
    MDB_env *mdbEnv = (MDB_env *) mdbEnvRaw;
    MDB_txn *transaction;
    if (const int c = mdb_txn_begin(mdbEnv, nullptr, readonly ? MDB_RDONLY : 0, &transaction)) {
        throwLMDBError(env, c);
    }
    return (long) transaction;
}

extern "C" JNIEXPORT void JNICALL
Java_com_openland_lmdb_LMDBJNI_commitTx(JNIEnv *env, jobject /* this */, jlong txnRaw) {
    if (const int c = mdb_txn_commit((MDB_txn *) txnRaw)) {
        throwLMDBError(env, c);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_openland_lmdb_LMDBJNI_abortTx(JNIEnv *env, jobject /* this */, jlong txnRaw) {
    mdb_txn_abort((MDB_txn *) txnRaw);
}

extern "C" JNIEXPORT void JNICALL
Java_com_openland_lmdb_LMDBJNI_openDatabase(JNIEnv *env, jobject /* this */, jlong txnRaw, jstring nameRaw) {
    MDB_dbi dbi;
    const char *name = env->GetStringUTFChars(nameRaw, nullptr);
    if (const int c = mdb_dbi_open((MDB_txn *) txnRaw, name, MDB_CREATE, &dbi)) {
        throwLMDBError(env, c);
    }
}
