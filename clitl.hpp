#ifndef __CLITL_HPP__
#define __CLITL_HPP__

#include <iosfwd>

namespace clitl {
	template <typename charT, typename traits = char_traits<charT> >
	class ostream : public std::basic_ostream<charT, traits> {
		// empty
	};

	template <typename charT, typename traits = char_traits<charT> >
	class istream : public std::basic_istream<charT, traits> {
		//empty
	};
}

#endif
