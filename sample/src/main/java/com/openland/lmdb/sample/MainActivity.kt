package com.openland.lmdb.sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.openland.lmdb.LMDB
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        LMDB.loadLibrary(this)

        val env = LMDB.createEnvironment(filesDir.absolutePath + "/app.db")

        var tx = env.startTransaction()
        val db = tx.openDatabase("database")
        db.put(tx, "key1", "value2")
        tx.commit()

        tx = env.startTransaction(true)
        val v = db.get(tx, "key1")
        Log.d("LMDB", "V: $v")
        tx.abort()

        env.close()


        // Example of a call to a native method
        sample_text.text = LMDB.testString()
    }
}
