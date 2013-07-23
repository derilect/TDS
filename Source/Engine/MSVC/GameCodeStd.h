#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <crtdbg.h>

#define _VS2005_

#if _MSC_VER < 1400
	#undef _VS2005_
#endif

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <memory>

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

class GCC_noncopyable
{
private:
	GCC_noncopyable(const GCC_noncopyable& x);
	GCC_noncopyable& operator=(const GCC_noncopyable& x);
public:
	GCC_noncopyable() {}
};

#if defined(_DEBUG)
#define GCC_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define GCC_NEW new
#endif

#include <d3dx9tex.h>

#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(x) if(x) delete x; x = NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if(x) delete [] x; x = NULL;
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x = NULL;
#endif