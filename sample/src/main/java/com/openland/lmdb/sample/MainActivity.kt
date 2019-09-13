package com.openland.lmdb.sample

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.openland.lmdb.LMDB
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        LMDB.loadLibrary(this)

        val env = LMDB.createEnvironment(filesDir.absolutePath + "/app.db")

        val tx = env.startTransaction()
        val db = tx.openDatabase("database")
        tx.commit()

        env.close()


        // Example of a call to a native method
        sample_text.text = LMDB.testString()
    }
}
