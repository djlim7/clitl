#ifndef __CLITL_H__
#define __CLITL_H__

#include <cstdio>
#include <locale>
#include <streambuf>
#include "stdafx.h"

#if WIN32
#ifdef CLITLDLL_EXPORTS
#define CLITLDLL_API __declspec(dllexport)
#else
#define CLITLDLL_API __declspec(dllimport)
#endif
#elif unix
#define CLITLDLL_API
#endif

namespace clitl
{
	template <typename charT,
		typename traits = std::char_traits<charT> >
		class CLITLDLL_API basic_outbuf : public std::basic_streambuf<charT, traits>
	{
	protected:
		void test(void);
	};

	typedef basic_outbuf<char> outbuf;
	typedef basic_outbuf<wchar_t> woutbuf;
}

void CLITLDLL_API test(void);

#endif
