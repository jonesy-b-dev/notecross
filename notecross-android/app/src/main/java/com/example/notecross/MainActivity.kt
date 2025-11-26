package com.example.notecross

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.example.notecross.components.AddTaskButton
import com.example.notecross.components.TaskLayout
import com.example.notecross.components.Title
import com.example.notecross.ui.theme.NotecrossTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            NotecrossTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                )
                {
                    Column(
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        Title("Notecross")
                        Column(
                            modifier = Modifier
                                .padding(30.dp)
                        ) {
                            TaskLayout()
                            TaskLayout()
                            TaskLayout()
                        }
                        AddTaskButton(onClick = { })
                    }
                }
            }
        }
    }
}