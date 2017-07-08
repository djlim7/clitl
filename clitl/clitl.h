#ifndef __CLITL_H__
#define __CLITL_H__

#if WIN32
// For Windows
#ifdef CLITLDLL_EXPORTS
#define CLITLDLL_API __declspec(dllexport)
#else
#define CLITLDLL_API __declspec(dllimport)
#endif

#elif unix
// For Unix
#define CLITLDLL_API
#endif

namespace clitl
{
	class cliout {
	public:
	};

	class cliin {
	public:
	};
}

#endif
