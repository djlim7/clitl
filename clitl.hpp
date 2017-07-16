#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstdio>
#include <iosfwd>
#include <locale>
#include <utility>

namespace clitl {
    /* Basic definitions and classes */
    typedef int coord_t;

    template <typename T>
    class rect {
        std::pair<T, T> origin;
        std::pair<T, T> endpoint;
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
    };

    template <typename charT, typename traits = std::char_traits<charT> >
    basic_ostream<charT, traits> clear(const basic_ostream<charT, traits>& os)
    {

    }

    template <typename charT, typename traits = std::char_traits<charT> >
    basic_ostream<charT, traits> refresh(const basic_ostream<charT, traits>& os)
    {

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
