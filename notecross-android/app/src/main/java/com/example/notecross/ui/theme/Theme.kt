package com.example.notecross.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color

private val DarkColorScheme = darkColorScheme(
    primary = Color(0xFFF0E9DB),
    secondary = Color(0xFF5A534D),
    tertiary = Color(0xFFE6C28D),
    background = Color(0xFF302E2B)
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