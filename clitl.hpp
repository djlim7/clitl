#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstdio>
#include <iosfwd>
#include <locale>
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
    /* Basic definitions and containers */
    typedef int coord_t;
    typedef int colornum_t;

    template <typename T>
    struct rect {
        std::pair<T, T> origin;
        std::pair<T, T> endpoint;
    };

    /* Color class */
    class color {
        colornum_t colornum;
    public:
        enum {
#if UNIX
            VOIDSPACE = -1,
            DEFAULT = 0,
            BLACK = 30,
            RED = 31,
            GREEN = 32,
            BROWN = 33,
            BLUE = 34,
            MAGENTA = 35,
            CYAN = 36,
#elif WIN32
            VOIDSPACE = -1,
            DEFAULT = 7,
            BLACK = 0,
            RED = 4,
            GREEN = 2,
            BROWN = 7, // Worthless
            BLUE = 1,
            MAGENTA = 7, // Worthless
            CYAN = 9,
#endif
        };
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
#ifdef UNIX
        static struct winsize wsize;
#endif
#ifdef WIN32
        static HANDLE termout_handle;
#endif
    public:
        explicit basic_ostream(basic_outbuf<charT, traits>* sb)
            : std::basic_ostream<charT, traits>(sb)
        {
            // Empty
        }

        basic_ostream<charT, traits>& moveto(const std::pair<coord_t, coord_t>& cord)
        {
            return *this;
        }

        std::pair<coord_t, coord_t> ssize()
        {
            return std::pair<coord_t, coord_t>(0, 0);
        }

        basic_ostream<charT, traits>& operator<<
            (basic_ostream<charT, traits>& (*op)(basic_ostream<charT, traits>&))
        {
            return (*op)(*this);
        }
    };

#ifdef UNIX
    template <typename charT, typename traits = std::char_traits<charT> >
    struct winsize basic_ostring<charT, traits>::wsize = { 0 };
#endif
#ifdef WIN32
    template <typename charT, typename traits = std::char_traits<charT> >
    HANDLE basic_ostream<charT, traits>::termout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& pre_process(basic_ostream<charT, traits>& os)
    {
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& post_process(basic_ostream<charT, traits>& os)
    {
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& clear(basic_ostream<charT, traits>& os)
    {
        return os;
    }

    template <typename charT, typename traits>
    basic_ostream<charT, traits>& refresh(basic_ostream<charT, traits>& os)
    {
        return os;
    }

    typedef basic_ostream<char> ostream;
    typedef basic_ostream<wchar_t> wostream;

    /* Input buffer */

    /* Input stream */
    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_istream : public std::basic_istream<charT, traits> {
        
    };

    typedef basic_istream<char> istream;
    typedef basic_istream<wchar_t> wistream;
}

#endif
