package com.example.notecross.components;

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

@Composable
fun AddTaskButton( onClick: () -> Unit)
{
    Row(
        modifier = Modifier
            .fillMaxSize()
            .padding(bottom = 60.dp),
        horizontalArrangement = Arrangement.Center,
        verticalAlignment = Alignment.Bottom
    )
    {
        FloatingActionButton(
            onClick = onClick,
            shape = CircleShape,
            containerColor = MaterialTheme.colorScheme.secondary,
            modifier = Modifier
                .size(60.dp)
        ) {
            Icon(
                imageVector = Icons.Default.Add,
                tint = MaterialTheme.colorScheme.tertiary,
                contentDescription = "Add Task",
                modifier = Modifier
                    .size(50.dp)
            )
        }
    }
}