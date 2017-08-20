#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstdio>
#include <locale>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>

#ifdef UNIX
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#elif WIN32
#include <conio.h>
#include <Windows.h>
#endif

namespace clitl {
    /* Color class */
    enum class color {
#ifdef UNIX
        DEFAULT = 0,
        BLACK = 0,
        RED = 1,
        GREEN = 2,
        BROWN = 3,
        BLUE = 4,
        MAGENTA = 5,
        CYAN = 6,
        WHITE = 7,
#elif WIN32
        DEFAULT = 0x7,
        BLACK = 0x0,
        RED = 0x4,
        GREEN = 0x2,
        BROWN = 0x7, // Worthless
        BLUE = 0x1,
        MAGENTA = 0x7, // Worthless
        CYAN = 0x9,
        WHITE = 0x7,
#endif
    };

    /* Basic definitions and containers */
    typedef int coord_t;
    //typedef int colornum_t;

    template <typename coordT, typename charT,
        typename traits = std::char_traits<charT>, typename Alloc = std::allocator<charT> >
    class basic_cli_object {
        std::pair<coordT, coordT> origin;
        std::pair<coordT, coordT> endpoint;
        std::basic_string<charT, traits, Alloc> str;
        color foreground;
        color background;
    public:
        explicit basic_cli_object(
            const std::pair<coordT, coordT>& origin = std::pair<coordT, coordT>(0, 0),
            const std::pair<coordT, coordT>& endpoint = std::pair<coordT, coordT>(0, 0),
            const std::basic_string<charT, traits, Alloc>& str
                                            = std::basic_string<charT, traits, Alloc>(),
            const color& foreground = clitl::color::WHITE,
            const color& background = clitl::color::BLACK)
            : origin(origin), endpoint(endpoint), str(str),
                foreground(foreground), background(background) {}

        const basic_cli_object<coordT, charT>& set_origin(const std::pair<coordT, coordT>& coord)
        {
            origin = coord;
            return *this;
        }

        const basic_cli_object<coordT, charT>& set_endpoint(const std::pair<coordT, coordT>& coord)
        {
            endpoint = coord;
            return *this;
        }

        const basic_cli_object<coordT, charT>& set_string(const std::basic_string<charT, traits, Alloc>& stri)
        {
            str = stri;
            return *this
        }

        const basic_cli_object<coordT, charT>& set_foreground(const color& foreg)
        {
            foreground = foreg;
            return *this;
        }

        const basic_cli_object<coordT, charT>& set_background(const color& backg)
        {
            background = backg;
            return *this;
        }

        const std::pair<coordT, coordT>& get_origin() const
        {
            return origin;
        }

        const std::pair<coordT, coordT>& get_endpoint() const
        {
            return endpoint;
        }

        const std::basic_string<charT, traits, Alloc>& get_string() const
        {
            return str;
        }

        const color& get_foreground() const
        {
            return foreground;
        }

        const color& get_background() const
        {
            return background;
        }
    };

    template <typename coordT, typename charT = char,
        typename traits = std::char_traits<charT>, typename Alloc = std::allocator<charT> >
    class rect : public basic_cli_object<coordT, charT, traits, Alloc> {
    public:
        explicit rect(
            const std::pair<coordT, coordT>& origin = std::pair<coordT, coordT>(0, 0),
            const std::pair<coordT, coordT>& endpoint = std::pair<coordT, coordT>(0, 0),
            const color& background = color::WHITE)
            : basic_cli_object<coordT, charT, traits, Alloc>(origin, endpoint,
                std::basic_string<charT, traits, Alloc>(" "), color::DEFAULT, background) {}
    };

    template <typename coordT, typename charT,
        typename traits = std::char_traits<charT>, typename Alloc = std::allocator<charT> >
        class coloredstring : public basic_cli_object<coordT, charT, traits, Alloc> {
    public:
        explicit coloredstring(
            const std::pair<coordT, coordT>& origin = std::pair<coordT, coordT>(0, 0),
            const std::pair<coordT, coordT>& endpoint = std::pair<coordT, coordT>(0, 0),
            const std::basic_string<charT, traits, Alloc>& str
            = std::basic_string<charT, traits, Alloc>(),
            const color& foreground = clitl::color::WHITE,
            const color& background = clitl::color::BLACK)
            : basic_cli_object<coordT, charT, traits, Alloc>(
                origin, endpoint, str, foreground, background) {}
        explicit coloredstring(
            const std::basic_string<charT, traits, Alloc>& str
            = std::basic_string<charT, traits, Alloc>(),
            const color& foreground = clitl::color::WHITE,
            const color& background = clitl::color::BLACK)
            : basic_cli_object<coordT, charT, traits, Alloc>(
                std::pair<coordT, coordT>(0, 0), std::pair<coordT, coordT>(0, 0),
                str, foreground, background) {}
    };

    /* Output buffer */
    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_outbuf : public std::basic_streambuf<charT, traits> {
    protected:
        virtual typename traits::int_type
            overflow(typename traits::int_type c)
        {
            if (std::putchar(c) == EOF) {
                return traits::eof();
            }
            return traits::not_eof(c);
        }
    };

    typedef basic_outbuf<char> outbuf;
    typedef basic_outbuf<wchar_t> woutbuf;

    /* Output stream */
    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_ostream : public std::basic_ostream<charT, traits> {
    public:
#ifdef UNIX
        struct winsize wsize;
#endif
#ifdef WIN32
        HANDLE termout_handle;
        CONSOLE_CURSOR_INFO termout_curinfo;
        CONSOLE_SCREEN_BUFFER_INFO termout_sbufinfo;
#endif
        explicit basic_ostream(basic_outbuf<charT, traits>* sb)
            : std::basic_ostream<charT, traits>(sb)
        {
#ifdef UNIX
            wsize = { 0 };
#elif WIN32
            termout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
        }

        basic_ostream<charT, traits>& moveto(const std::pair<coord_t, coord_t>& coord)
        {
#ifdef UNIX
            *this << "\033[" << coord.second << ";" << coord.first << "f";
#elif WIN32
            COORD pos = { static_cast<SHORT>(coord.first) - 1, static_cast<SHORT>(coord.second) - 1 };
            SetConsoleCursorPosition(termout_handle, pos);
#endif
            return *this;
        }

        std::pair<coord_t, coord_t> screensize()
        {
            static coord_t column;
            static coord_t row;

#ifdef UNIX
            column = wsize.ws_col;
            row = wsize.ws_row;
#elif WIN32
            GetConsoleScreenBufferInfo(termout_handle, &termout_sbufinfo);
            column = static_cast<coord_t>(termout_sbufinfo.dwSize.X);
            row = static_cast<coord_t>(termout_sbufinfo.dwSize.Y);
#endif

            return std::pair<coord_t, coord_t>(column, row);
        }

        basic_ostream<charT, traits>& paintmode(color fgd, color bgd)
        {
#ifdef UNIX
            os << "\033["
                << static_cast<int>(30 + fgd)
                << ";"
                << static_cast<int>(40 + bwd)
                << "m" << ;
#elif WIN32
            SetConsoleTextAttribute(this->termout_handle,
                static_cast<WORD>(static_cast<int>(fgd)
                    + 0x10 * static_cast<int>(bgd)));
#endif
            return *this;
        }

        basic_ostream<charT, traits>& operator<<
            (basic_ostream<charT, traits>& (*op)(basic_ostream<charT, traits>&))
        {
            return (*op)(*this);
        }
    };

    typedef basic_ostream<char> ostream;
    typedef basic_ostream<wchar_t> wostream;

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& alternative_system_screenbuffer(basic_ostream<charT, traits>& os)
    {
#if UNIX
        os << "\033[?1049h"; // Use alternate screen buffer
#endif
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& clear(basic_ostream<charT, traits>& os)
    {
#ifdef UNIX
        os << "\033[2J";
#elif WIN32
        COORD startpoint = { 0, 0 };
        DWORD dw;

        GetConsoleScreenBufferInfo(os.termout_handle, &os.termout_sbufinfo);
        FillConsoleOutputCharacterA(os.termout_handle, ' ',
            os.termout_sbufinfo.dwSize.X * os.termout_sbufinfo.dwSize.Y,
            startpoint, &dw);
        FillConsoleOutputAttribute(os.termout_handle,
            FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
            os.termout_sbufinfo.dwSize.X * os.termout_sbufinfo.dwSize.Y,
            startpoint, &dw);
#endif
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& hide_cursor(basic_ostream<charT, traits>& os)
    {
#ifdef UNIX
        os << "\033[?25l"; // Hide cursor
#elif WIN32
    GetConsoleCursorInfo(os.termout_handle, &os.termout_curinfo);
    os.termout_curinfo.bVisible = 0;
    SetConsoleCursorInfo(os.termout_handle, &os.termout_curinfo);
#endif
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& normal_system_screenbuffer(basic_ostream<charT, traits>& os)
    {
#if UNIX
        os << "\033[?1049l"; // Use normal screen buffer
#endif
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& refresh(basic_ostream<charT, traits>& os)
    {
        std::fflush(stdout);
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& show_cursor(basic_ostream<charT, traits>& os)
    {
#if UNIX
        os << "\033[?25h"; // Show cursor
#elif WIN32
    CONSOLE_CURSOR_INFO termout_curinfo;
    GetConsoleCursorInfo(os.termout_handle, &termout_curinfo);
    termout_curinfo.bVisible = 1;
    SetConsoleCursorInfo(os.termout_handle, &termout_curinfo);
#endif
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& pre_process(basic_ostream<charT, traits>& os)
    {
        os << alternative_system_screenbuffer;
        os << hide_cursor;
        os << clear;
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& post_process(basic_ostream<charT, traits>& os)
    {
        os << clear;
        os.moveto(std::pair<coord_t, coord_t>(1, 1));
        os.paintmode(color::WHITE, color::BLACK);
        os << normal_system_screenbuffer;
        os << show_cursor;
        return os;
    }

    template <typename coordT, typename charT, typename traits, typename Alloc>
    basic_ostream<charT, traits>& operator<<
        (basic_ostream<charT, traits>& os,
            const coloredstring<coordT, charT, traits, Alloc>& cs)
    {
        os.paintmode(cs.get_foreground(), cs.get_background());
        os << cs.get_string().c_str();
        return os;
    }

    template <typename coordT, typename charT, typename traits, typename Alloc>
    basic_ostream<charT, traits>& operator<<
        (basic_ostream<charT, traits>& os, const rect<coordT, charT, traits, Alloc>& re)
    {
        for (int i = re.get_origin().first; i <= re.get_endpoint().first; ++i) {
            for (int j = re.get_origin().second; j <= re.get_endpoint().second; ++j) {
                os.moveto(std::pair<coord_t, coord_t>(i, j));
                os << coloredstring<coordT, charT, traits, Alloc>(
                    re.get_string(), re.get_foreground(), re.get_background());
            }
        }
        return os;
    }

    /* Input buffer */

    /* Input stream */
    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_istream : public std::basic_istream<charT, traits> {
        
    };

    typedef basic_istream<char> istream;
    typedef basic_istream<wchar_t> wistream;
}

#endif
