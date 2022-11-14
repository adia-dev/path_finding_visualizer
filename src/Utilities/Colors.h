#pragma once

namespace se
{

    enum Colors
    {
        Red,
        Green,
        Blue,
        Yellow,
        White,
        Black,
        Grey,
        Orange,
        Purple,
        Cyan,
        Magenta,
        Brown,
        DarkRed,
        DarkGreen,
        DarkBlue,
        DarkYellow,
        DarkWhite,
        DarkBlack,
        DarkGrey,
        DarkOrange,
        DarkPurple,
        DarkCyan,
        DarkMagenta,
        DarkBrown,
        LightRed,
        LightGreen,
        LightBlue,
        LightYellow,
        LightWhite,
        LightBlack,
        LightGrey,
        LightOrange,
        LightPurple,
        LightCyan,
        LightMagenta,
        LightBrown,
        Transparent,
        Reset
    };

    static const char *COLORS_CODE[] = {
        "[31m", // Red
        "[32m", // Green
        "[34m", // Blue
        "[33m", // Yellow
        "[37m", // White
        "[30m", // Black
        "[90m", // Grey
        "[91m", // Orange
        "[95m", // Purple
        "[96m", // Cyan
        "[35m", // Magenta
        "[33m", // Brown
        "[31m", // DarkRed
        "[32m", // DarkGreen
        "[34m", // DarkBlue
        "[33m", // DarkYellow
        "[37m", // DarkWhite
        "[30m", // DarkBlack
        "[90m", // DarkGrey
        "[91m", // DarkOrange
        "[95m", // DarkPurple
        "[96m", // DarkCyan
        "[35m", // DarkMagenta
        "[33m", // DarkBrown
        "[91m", // LightRed
        "[92m", // LightGreen
        "[94m", // LightBlue
        "[93m", // LightYellow
        "[97m", // LightWhite
        "[90m", // LightBlack
        "[90m", // LightGrey
        "[91m", // LightOrange
        "[95m", // LightPurple
        "[96m", // LightCyan
        "[35m", // LightMagenta
        "[33m", // LightBrown
        "[0m"   // Transparent,
        "[0m"   // Reset
    };

#define RESET "[0m"

} // namespace se
