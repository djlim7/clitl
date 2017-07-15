#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <cstdio>
#include <iosfwd>
#include <locale>
#include <utility>

namespace clitl {
	/* Basic definitions and classes */
	typedef int scord_t;

	class parametric_manipulator {
		// Empty
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
	};

	typedef basic_ostream<char> ostream;
	typedef basic_ostream<wchar_t> wostream;

	class moveto : public parametric_manipulator {
		std::pair<scord_t, scord_t> pr;
	public:
		explicit moveto(std::pair<scord_t, scord_t> pr) : pr(pr)
		{
			// Empty
		}

		template <typename charT, typename traits = std::char_traits<charT> >
		friend std::basic_ostream<charT, traits>&
			operator<<(std::basic_ostream<charT, traits>& os, const moveto& mvto);
	};

	template <typename charT, typename traits>
	std::basic_ostream<charT, traits>&
		operator<<(std::basic_ostream<charT, traits>& os, const moveto& mvto)
	{
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
