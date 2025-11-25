package com.example.notecross

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.sp
import com.example.notecross.ui.theme.NotecrossTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        //enableEdgeToEdge()
        setContent {
            NotecrossTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                )
                {
                    Row(
                    ){
                        Title("Notecross")
                    }
                }
            }
        }
    }
}

@Composable
fun Title(title: String)
{
    Text( text = title,
        color = MaterialTheme.colorScheme.primary,
        fontSize = 50.sp)
}

@Preview(showBackground = true)
@Composable
fun ShowTitle(){
    Title("Notecross")
}