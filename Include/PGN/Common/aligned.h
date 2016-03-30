#pragma once

#ifdef _MSC_VER
#define _aligned(a, x) __declspec(align(a)) x
#endif

#ifdef __GNUC__
#define _aligned(a, x) x __attribute__ ((aligned(a)))
#endif

#ifdef __SNC__
#define _aligned(a, x) x __attribute__ ((aligned(a)))
#endif
