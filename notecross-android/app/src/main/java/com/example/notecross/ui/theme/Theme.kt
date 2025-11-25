package com.example.notecross.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color

private val DarkColorScheme = darkColorScheme(
    primary = Color(0xFFECE0C8),
    secondary = Color(0xFF5E5547),
    tertiary = Color(0xFFD7B072),
    background = Color(0xFF2C2923)
)
@Composable
fun NotecrossTheme(
    content: @Composable () -> Unit
) {
    MaterialTheme(
        colorScheme = DarkColorScheme,
        typography = Typography,
        content = content
    )
}