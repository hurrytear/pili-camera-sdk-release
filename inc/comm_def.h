#ifndef __PILI_COMM_DEF__
#define __PILI_COMM_DEF__

#if defined(_WIN32)

#if defined(PILI_CAMERA_SDK_IMPLEMENTATION)
#define PILI_CAMERA_SDK_EXPORT __declspec(dllexport)
#else
#define PILI_CAMERA_SDK_EXPORT __declspec(dllimport)
#endif

#else  /* defined(_WIN32) */

#if defined(PILI_CAMERA_SDK_IMPLEMENTATION)
#define PILI_CAMERA_SDK_EXPORT __attribute__((visibility("default")))
#else
#define PILI_CAMERA_SDK_EXPORT
#endif

#endif  /* defined(_WIN32) */

#endif // __PILI_COMM_DEF__