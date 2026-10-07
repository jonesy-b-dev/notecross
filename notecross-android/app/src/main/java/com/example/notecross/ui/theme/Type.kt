package com.example.notecross.ui.theme

import androidx.compose.material3.Typography
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.sp
import com.example.notecross.R

val SquadaOne = FontFamily(
    Font(
        resId = R.font.squada_one_regular,
        weight = FontWeight.Normal
    )
)

val Typography = Typography(
    titleLarge = TextStyle(
        fontFamily = SquadaOne,
        fontWeight = FontWeight.Normal,
        fontSize = 50.sp
    ),
    titleMedium = TextStyle(
        fontFamily = SquadaOne,
        fontWeight = FontWeight.Normal,
        fontSize = 20.sp
    ),
    bodyMedium = TextStyle(
        fontFamily = SquadaOne,
        fontWeight = FontWeight.Normal,
        fontSize = 15.sp
    ),
    bodyLarge = TextStyle(
        fontFamily = SquadaOne,
        fontWeight = FontWeight.Normal,
        fontSize = 16.sp,
        lineHeight = 24.sp,
        letterSpacing = 0.5.sp
    )
)
