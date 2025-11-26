package com.example.notecross.components

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.outlined.CheckCircle
import androidx.compose.material.icons.outlined.Delete
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.draw.shadow
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.notecross.ui.theme.ExtraColors
import com.example.notecross.ui.theme.SquadaOne

@Preview
@Composable
fun TaskLayout(){
    Surface(
        modifier = Modifier
            .fillMaxWidth()
            .padding(10.dp)
            .shadow(
                elevation = 4.dp,
                shape = RoundedCornerShape(16.dp),
                clip = false,
                ambientColor = Color.Black.copy(alpha = 1f),
                spotColor = Color.Black.copy(alpha = 1f)

            ),
        shape = RoundedCornerShape(16.dp),
        color = MaterialTheme.colorScheme.secondary,
    )
    {
        Row(
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically,
            modifier = Modifier
                .background(MaterialTheme.colorScheme.secondary)
                .clip(RoundedCornerShape(16.dp))
                .padding(20.dp)
                .fillMaxWidth()
        )
        {
            IconButton(
                onClick = {},
                modifier = Modifier
                    .size(30.dp)
            )
            {
                Icon(
                    imageVector = Icons.Outlined.CheckCircle,
                    tint = ExtraColors.green,
                    contentDescription = "Add Task",
                )
            }
            Column(
                horizontalAlignment = Alignment.CenterHorizontally
            )
            {
                Text(text = "Task Name",
                    color = MaterialTheme.colorScheme.primary,
                    fontSize = 20.sp,
                    fontFamily = SquadaOne
                )
                Text(text = "Task due",
                    color = MaterialTheme.colorScheme.primary,
                    fontSize = 15.sp,
                    fontFamily = SquadaOne
                )
            }
            IconButton(
                onClick = {},
                modifier = Modifier
                    .size(30.dp)
            )
            {
                Icon(
                    imageVector = Icons.Outlined.Delete,
                    tint = ExtraColors.red,
                    contentDescription = "Add Task",
                    modifier = Modifier
                        .size(30.dp)
                )
            }
        }
    }
}