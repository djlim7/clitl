#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstddef>
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
        DEFAULT = -0x1,
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
            const color& foreground = color::WHITE,
            const color& background = color::BLACK)
            : origin(origin), endpoint(endpoint), str(str),
                foreground(foreground), background(background) {}

        const basic_cli_object<coordT, charT, traits, Alloc>&
            set_origin(const std::pair<coordT, coordT>& coord)
        {
            origin = coord;
            return *this;
        }

        const basic_cli_object<coordT, charT, traits, Alloc>&
            set_endpoint(const std::pair<coordT, coordT>& coord)
        {
            endpoint = coord;
            return *this;
        }

        const basic_cli_object<coordT, charT, traits, Alloc>&
            set_string(const std::basic_string<charT, traits, Alloc>& stri)
        {
            str = stri;
            return *this;
        }

        const basic_cli_object<coordT, charT, traits, Alloc>&
            set_foreground(const color& foreg)
        {
            foreground = foreg;
            return *this;
        }

        const basic_cli_object<coordT, charT, traits, Alloc>&
            set_background(const color& backg)
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

        bool check_cover
            (const basic_cli_object<coordT, charT, traits, Alloc>& bco) const
        {
            if (this->origin.first <= bco.origin.first
                && this->endpoint.first >= bco.endpoint.first
                && this->origin.second <= bco.origin.second
                && this->endpoint.second >= bco.endpoint.second)
                return true;
            return false;
        }

        bool operator&&
            (const basic_cli_object<coordT, charT, traits, Alloc>& bco) const
        {
            if (this->origin.first <= bco.endpoint.first
                && this->endpoint.first >= bco.origin.first
                && this->origin.second <= bco.endpoint.second
                && this->endpoint.second >= bco.origin.second)
                return true;
            return false;
        }

        basic_cli_object<coordT, charT, traits, Alloc> operator+
            (const basic_cli_object<coordT, charT, traits, Alloc>& bco) const
        {
            return basic_cli_object<coordT, charT, traits, Alloc>
                (std::pair<coordT, coordT>
                (this->origin + bco.origin, this->endpoint + bco.endpoint));
        }
    };

    template <typename coordT = coord_t, typename charT = char,
        typename traits = std::char_traits<charT>, typename Alloc = std::allocator<charT> >
    class rect : public basic_cli_object<coordT, charT, traits, Alloc> {
    public:
        explicit rect(
            const std::pair<coordT, coordT>& origin = std::pair<coordT, coordT>(1, 1),
            const std::pair<coordT, coordT>& endpoint = std::pair<coordT, coordT>(1, 1),
            const color& background = color::WHITE)
            : basic_cli_object<coordT, charT, traits, Alloc>(origin, endpoint,
                std::basic_string<charT, traits, Alloc>(" "), color::DEFAULT, background) {}
    };

    template <typename coordT = coord_t, typename charT = char,
        typename traits = std::char_traits<charT>, typename Alloc = std::allocator<charT> >
        class coloredstring : public basic_cli_object<coordT, charT, traits, Alloc> {
    public:
        explicit coloredstring(
            const std::pair<coordT, coordT>& origin = std::pair<coordT, coordT>(1, 1),
            const std::basic_string<charT, traits, Alloc>& str
            = std::basic_string<charT, traits, Alloc>(),
            const color& foreground = clitl::color::WHITE,
            const color& background = clitl::color::BLACK)
            : basic_cli_object<coordT, charT, traits, Alloc>(
                origin, std::pair<coordT, coordT>(origin.first + str.length() - 1, origin.second),
                str, foreground, background) {}
        explicit coloredstring(
            const std::basic_string<charT, traits, Alloc>& str
            = std::basic_string<charT, traits, Alloc>(),
            const color& foreground = clitl::color::WHITE,
            const color& background = clitl::color::BLACK)
            : basic_cli_object<coordT, charT, traits, Alloc>(
                std::pair<coordT, coordT>(1, 1), std::pair<coordT, coordT>(1 + str.length() - 1, 1),
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
    template <typename charT,
        typename traits = std::char_traits<charT>, typename coordT = coord_t >
    class basic_ostream : public std::basic_ostream<charT, traits> {
    public:
#ifdef UNIX
        struct winsize wsize;
#endif
#ifdef WIN32
        HANDLE termout_handle;
        CONSOLE_CURSOR_INFO termout_curinfo;
        CONSOLE_SCREEN_BUFFER_INFO termout_sbufinfo;
        CONSOLE_SCREEN_BUFFER_INFO termout_initial_sbufinfo;
#endif
        explicit basic_ostream(basic_outbuf<charT, traits>* sb)
            : std::basic_ostream<charT, traits>(sb)
        {
#ifdef UNIX
            wsize = { 0 };
#elif WIN32
            termout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            GetConsoleScreenBufferInfo(termout_handle, &termout_initial_sbufinfo);
#endif
        }

        basic_ostream<charT, traits, coordT>& movecursor(coordT xpos, coordT ypos)
        {
#ifdef UNIX
            *this << "\033[" << ypos << ";" << xpos << "f";
#elif WIN32
            COORD pos = { static_cast<SHORT>(xpos) - 1, static_cast<SHORT>(ypos) - 1 };
            SetConsoleCursorPosition(termout_handle, pos);
#endif
            return *this;
        }

        basic_ostream<charT, traits, coordT>& movecursor(const std::pair<coordT, coordT>& coord)
        {
            movecursor(coord.first, coord.second);
            return *this;
        }

        std::pair<coordT, coordT> screensize()
        {
            static coordT column;
            static coordT row;

#ifdef UNIX
            column = wsize.ws_col;
            row = wsize.ws_row;
#elif WIN32
            /* Real code
            GetConsoleScreenBufferInfo(termout_handle, &termout_sbufinfo);
            column = static_cast<coordT>(termout_sbufinfo.dwSize.X);
            row = static_cast<coordT>(termout_sbufinfo.dwSize.Y);
            */

            column = 110;
            row = 28;
#endif

            return std::pair<coordT, coordT>(column, row);
        }

        basic_ostream<charT, traits, coordT>& paintmode(color fgd, color bgd)
        {
#ifdef UNIX
            *this << "\033["
                << 30 + static_cast<int>(fgd)
                << ";"
                << 40 + static_cast<int>(bgd)
                << "m";
#elif WIN32
            if (color::DEFAULT == fgd) {
                fgd = static_cast<color>(termout_initial_sbufinfo.wAttributes % 0x10);
            }
            if (color::DEFAULT == bgd) {
                bgd = static_cast<color>((termout_initial_sbufinfo.wAttributes / 0x10) % 0x10);
            }
            SetConsoleTextAttribute(this->termout_handle,
                static_cast<WORD>(static_cast<int>(fgd)
                    + 0x10 * static_cast<int>(bgd)));
#endif
            return *this;
        }

        basic_ostream<charT, traits, coordT>& paintmode(nullptr_t nullp)
        {
            // Recovery mode
#ifdef UNIX
            os << "\033[0m";
#elif WIN32
            SetConsoleTextAttribute(termout_handle, termout_initial_sbufinfo.wAttributes);
#endif
            return *this;
        }

        basic_ostream<charT, traits, coordT>& draw
            (const basic_cli_object<coordT, charT>& bco)
        {
            this->movecursor(bco.get_origin());
            this->paintmode(bco.get_foreground(), bco.get_background());
            *this << bco.get_string().c_str();

            return *this;
        }

        basic_ostream<charT, traits, coordT>& operator<<
            (basic_ostream<charT, traits, coordT>& (*op)(basic_ostream<charT, traits, coordT>&))
        {
            return (*op)(*this);
        }
    };

    typedef basic_ostream<char> ostream;
    typedef basic_ostream<wchar_t> wostream;

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        alternative_system_screenbuffer(basic_ostream<charT, traits, coordT>& os)
    {
#if UNIX
        os << "\033[?1049h"; // Use alternate screen buffer
#endif
        return os;
    }

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        clear(basic_ostream<charT, traits, coordT>& os)
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

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        hide_cursor(basic_ostream<charT, traits, coordT>& os)
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

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        normal_system_screenbuffer(basic_ostream<charT, traits, coordT>& os)
    {
#if UNIX
        os << "\033[?1049l"; // Use normal screen buffer
#endif
        return os;
    }

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        refresh(basic_ostream<charT, traits, coordT>& os)
    {
        std::fflush(stdout);
        return os;
    }

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        show_cursor(basic_ostream<charT, traits, coordT>& os)
    {
#if UNIX
        os << "\033[?25h"; // Show cursor
#elif WIN32
        GetConsoleCursorInfo(os.termout_handle, &os.termout_curinfo);
        os.termout_curinfo.bVisible = 1;
        SetConsoleCursorInfo(os.termout_handle, &os.termout_curinfo);
#endif
        return os;
    }

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        pre_process(basic_ostream<charT, traits, coordT>& os)
    {
        os << alternative_system_screenbuffer;
        os << hide_cursor;
        os << clear;
        return os;
    }

    template <typename charT, typename traits, typename coordT>
    basic_ostream<charT, traits, coordT>&
        post_process(basic_ostream<charT, traits, coordT>& os)
    {
        os << clear;
        os << show_cursor;
        os.movecursor(1, 1);
        os.paintmode(nullptr);
        os << normal_system_screenbuffer;
        return os;
    }

    template <typename coordT, typename charT, typename traits, typename Alloc>
    basic_ostream<charT, traits, coordT>& operator<<
        (basic_ostream<charT, traits, coordT>& os,
            const coloredstring<coordT, charT, traits, Alloc>& cs)
    {
        os.draw(cs);
        return os;
    }

    template <typename coordT, typename charT, typename traits, typename Alloc>
    basic_ostream<charT, traits, coordT>& operator<<
        (basic_ostream<charT, traits, coordT>& os, const rect<coordT, charT, traits, Alloc>& re)
    {
        static rect<coordT, charT, traits, Alloc> temprec;
        for (int i = re.get_origin().first; i <= re.get_endpoint().first; ++i) {
            for (int j = re.get_origin().second; j <= re.get_endpoint().second; ++j) {
                temprec = re;
                temprec.set_origin(std::pair<coord_t, coord_t>(i, j));
                os.draw(temprec);
            }
        }
        return os;
    }

    /* Input buffer */

    /* Input stream */
    /* Keeping it for further update

    template <typename charT,
        typename traits = std::char_traits<charT>, typename coordT = coord_t >
    class basic_istream : public std::basic_istream<charT, traits, coordT> {
        
    };

    typedef basic_istream<char> istream;
    typedef basic_istream<wchar_t> wistream;
    */
}

#endif
