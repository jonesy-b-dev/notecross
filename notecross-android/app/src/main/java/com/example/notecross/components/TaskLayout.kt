package com.example.notecross.components

import androidx.compose.foundation.clickable
import androidx.compose.foundation.interaction.MutableInteractionSource
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.outlined.CheckCircle
import androidx.compose.material.icons.outlined.Delete
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.ripple
import androidx.compose.runtime.Composable
import androidx.compose.runtime.Immutable
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.drawBehind
import androidx.compose.ui.geometry.CornerRadius
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.notecross.ui.theme.ExtraColors

private val CheckIcon = Icons.Outlined.CheckCircle
private val DeleteIcon = Icons.Outlined.Delete
private val BorderColor = Color(0xFF302A22)

@Immutable
data class TaskItem(
    val id: Int,
    val name: String,
    val due: String
)

@Composable
fun TaskLayout(
    modifier: Modifier = Modifier,
    task: TaskItem = TaskItem(0, "Task Name", "Task due")
) {
    val cardColor = MaterialTheme.colorScheme.secondary
    val textColor = MaterialTheme.colorScheme.primary
    val titleStyle = MaterialTheme.typography.titleMedium
    val bodyStyle = MaterialTheme.typography.bodyMedium

    Row(
        horizontalArrangement = Arrangement.SpaceBetween,
        verticalAlignment = Alignment.CenterVertically,
        modifier = modifier
            .fillMaxWidth()
            .padding(10.dp)
            .drawBehind {
                val cornerRadius = CornerRadius(16.dp.toPx())
                drawRoundRect(
                    color = cardColor,
                    cornerRadius = cornerRadius
                )
                drawRoundRect(
                    color = BorderColor,
                    cornerRadius = cornerRadius,
                    style = Stroke(width = 1.dp.toPx())
                )
            }
            .padding(20.dp)
    ) {
        Icon(
            imageVector = CheckIcon,
            tint = ExtraColors.green,
            contentDescription = "Complete Task",
            modifier = Modifier
                .size(40.dp)
                .clickable(
                    interactionSource = remember { MutableInteractionSource() },
                    indication = ripple(bounded = false, radius = 24.dp),
                    onClick = {}
                )
                .padding(8.dp)
        )
        Column(
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(
                text = task.name,
                color = textColor,
                style = titleStyle
            )
            Text(
                text = task.due,
                color = textColor,
                style = bodyStyle
            )
        }

        Icon(
            imageVector = DeleteIcon,
            tint = ExtraColors.red,
            contentDescription = "Delete Task",
            modifier = Modifier
                .size(40.dp)
                .clickable(
                    interactionSource = remember { MutableInteractionSource() },
                    indication = ripple(bounded = false, radius = 24.dp),
                    onClick = {}
                )
                .padding(8.dp)
        )
    }
}

@Preview
@Composable
fun TaskLayoutPreview() {
    TaskLayout()
}
