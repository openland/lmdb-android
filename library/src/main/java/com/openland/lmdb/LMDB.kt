package com.openland.lmdb

import android.content.Context
import com.getkeepsafe.relinker.ReLinker

class LMDBException(message: String, val id: Int) : Exception(message)

object LMDB {
    private val jni by lazy {
        LMDBJNI()
    }

    fun loadLibrary(context: Context) {
        ReLinker.loadLibrary(context, "lmdbjni")
    }

    fun testString(): String {
        return jni.testString()
    }

    fun createEnvironment(path: String): LMDBEnvironment {
        val id = jni.createEnv()
        jni.openEnv(id, path)
        return LMDBEnvironment(id, jni)
    }
}

class LMDBEnvironment(val id: Long, private val jni: LMDBJNI) {

    fun startTransaction(readonly: Boolean = false): LMDBTransaction {
        return LMDBTransaction(jni.beginTx(this.id, readonly), jni)
    }

    fun close() {
        jni.closeEnv(this.id)
    }
}

class LMDBDatabase(val id: Long, private val jni: LMDBJNI) {

    fun put(tx: LMDBTransaction, key: String, value: String) {
        jni.put(tx.id, this.id, key, value)
    }

    fun get(tx: LMDBTransaction, key: String): String? {
        return jni.get(tx.id, this.id, key)
    }
}

class LMDBTransaction(val id: Long, private val jni: LMDBJNI) {

    fun openDatabase(name: String): LMDBDatabase {
        return LMDBDatabase(jni.openDatabase(this.id, name), jni)
    }

    fun commit() {
        jni.commitTx(this.id)
    }

    fun abort() {
        jni.abortTx(this.id)
    }
}

class LMDBJNI {

    // Environment

    external fun createEnv(): Long
    external fun openEnv(envId: Long, path: String)
    external fun closeEnv(envId: Long)

    // Transactions

    external fun beginTx(envId: Long, readonly: Boolean): Long
    external fun commitTx(txId: Long)
    external fun abortTx(txId: Long)

    // Database

    external fun openDatabase(txId: Long, name: String): Long

    // Operations

    external fun put(txId: Long, dbId: Long, key: String, value: String)
    external fun clear(txId: Long, dbId: Long, key: String)
    external fun get(txId: Long, dbID: Long, key: String): String?

    external fun testString(): String
}