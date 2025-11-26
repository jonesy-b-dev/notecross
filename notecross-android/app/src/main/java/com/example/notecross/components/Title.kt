package com.example.notecross.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.notecross.ui.theme.SquadaOne

@Composable
fun Title(title: String)
{
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.Center,
    )
    {
        Text(
            text = title,
            color = MaterialTheme.colorScheme.primary,
            fontSize = 50.sp,
            fontFamily = SquadaOne,
            modifier = Modifier
                .padding(top = 80.dp, bottom = 30.dp)
        )
    }
}

@Preview(showBackground = true)
@Composable
fun ShowTitle(){
    Title("Notecross")
}