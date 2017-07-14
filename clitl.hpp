#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstdio>
#include <iosfwd>
#include <locale>

namespace clitl {
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

	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_ostream : public std::basic_ostream<charT, traits> {
	public:
		explicit basic_ostream(basic_outbuf<charT, traits>* sb)
			: std::basic_ostream<charT, traits>(sb)
		{
			// empty
		}
	};

	typedef basic_ostream<char> ostream;
	typedef basic_ostream<wchar_t> wostream;

	template <typename charT, typename traits = std::char_traits<charT> >
	class basic_istream : public std::basic_istream<charT, traits> {
		
	};

	typedef basic_istream<char> istream;
	typedef basic_istream<wchar_t> wistream;
}

#endif
