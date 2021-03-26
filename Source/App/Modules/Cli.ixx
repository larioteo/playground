
module;

#include <iomanip>
#include <iostream>
#include <ostream>

export module Cli;

// Disable:
#pragma warning(push)
#pragma warning(disable: 26812)

/**
* @brief CLI Modifiers: Extends output stream with easy support for colors and styles.
*/
export namespace app { namespace cli {


enum class Backgrounds {
    Default         = 49,

    Black           = 40,
    Red             = 41,
    Green           = 42,
    Yellow          = 43,
    Blue            = 44,
    Magenta         = 45,
    Cyan            = 46,
    LightGray       = 47,

    Gray            = 100,
    LightRed        = 101,
    LightGreen      = 102,
    LightYellow     = 103,
    LightBlue       = 104,
    LightMagenta    = 105,
    LightCyan       = 106,
    White           = 107,
};

enum class Colors {
    Default         = 39,

    Black           = 30,
    Red             = 31,
    Green           = 32,
    Yellow          = 33,
    Blue            = 34,
    Magenta         = 35,
    Cyan            = 36,
    LightGray       = 37,

    Gray            = 90,
    LightRed        = 91,
    LightGreen      = 92,
    LightYellow     = 93,
    LightBlue       = 94,
    LightMagenta    = 95,
    LightCyan       = 96,
    White           = 97,
};

enum class Styles {
    Normal          = 0,
    Bold            = 1,
    Dim             = 2,
    Italic          = 3,
    Underline       = 4,
    Blink           = 5,
    RapidBlink      = 6,
    Reverse         = 7,
    Conceal         = 8,
    Strike          = 9,

    NBold           = 21,
    NDim            = 22,
    NotUnderlined   = 24,
    NotBlinking     = 25,
    NotReversed     = 27,
    Reveal          = 28,
    NotStriked      = 29,

    Reset           = Normal,
};


template <typename T>
concept CliModifier =
    std::is_same_v<Backgrounds, T> ||
    std::is_same_v<Colors, T> ||
    std::is_same_v<Styles, T>;

template <CliModifier T>
inline std::ostream &operator<<(std::ostream &os, T t) {
    return os << "\x1b[" << static_cast<int>(t) << "m";
}


void Test() {
    const Backgrounds backgrounds[] = {
        Backgrounds::Black,      Backgrounds::Red,            Backgrounds::Green,      Backgrounds::Yellow,
        Backgrounds::Blue,       Backgrounds::Magenta,        Backgrounds::Cyan,       Backgrounds::LightGray,
        Backgrounds::Gray,       Backgrounds::LightRed,       Backgrounds::LightGreen, Backgrounds::LightYellow,
        Backgrounds::LightBlue,  Backgrounds::LightMagenta,   Backgrounds::LightCyan,  Backgrounds::White
    };
    const Colors colors[] = {
        Colors::Black,      Colors::Red,            Colors::Green,      Colors::Yellow,
        Colors::Blue,       Colors::Magenta,        Colors::Cyan,       Colors::LightGray,
        Colors::Gray,       Colors::LightRed,       Colors::LightGreen, Colors::LightYellow,
        Colors::LightBlue,  Colors::LightMagenta,   Colors::LightCyan,  Colors::White
    };
    
    std::cout << "\nSupported backgrounds and colors:\n";
    int counter = 0;
    for (const auto &color : colors) {
        for (const auto &background : backgrounds) {
            std::cout << background << color << " " << std::setfill('0') << std::setw(3) << counter++ << " ";
        }
        std::cout << Styles::Reset << "\n";
    }
    std::cout << Styles::Reset;

    std::cout << "\nSupported styles:\n";
    std::cout << Styles::Bold       << "Bold       " << Styles::Reset           << std::endl;
    std::cout << Styles::Dim        << "Dim        " << Styles::NDim            << std::endl;
    std::cout << Styles::Italic     << "Italic     " << Styles::Reset           << std::endl;
    std::cout << Styles::Underline  << "Underline  " << Styles::NotUnderlined   << std::endl;
    std::cout << Styles::Blink      << "Blink      " << Styles::NotBlinking     << std::endl;
    std::cout << Styles::RapidBlink << "RapidBlink " << Styles::NotBlinking     << std::endl;
    std::cout << Styles::Reverse    << "Reverse    " << Styles::NotReversed     << std::endl;
    std::cout << Styles::Conceal    << "Concealed  " << Styles::Reveal          << std::endl;
    std::cout << Styles::Strike     << "Strike     " << Styles::NotStriked      << std::endl;

    std::cout << "\x1b[" << 11 << "m"  << "Test" << std::endl;
    
    std::cout << Styles::Reset;
}

}}

#pragma warning(pop)
