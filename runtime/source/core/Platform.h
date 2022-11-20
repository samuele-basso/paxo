#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#ifdef _WIN64
#define RT_WINDOWS_64
#else
#error 32-bit Windows is not supported
#endif

#elif __APPLE__

#error Apple computer is not supported
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#elif TARGET_OS_MACCATALYST
#elif TARGET_OS_IPHONE
#elif TARGET_OS_MAC
#else
#error "Unknown Apple platform"
#endif

#elif __linux__
#define RT_LINUX

#elif __unix__
#error Unix is not supported

#elif defined(_POSIX_VERSION)
#error Posix is not supported

#else
#error "Unknown compiler"

#endif


#ifdef RT_ACTIVE_API_DIRECTX

#if RT_WINDOWS_64
#error DirectX not supported
#else
#error DirectX is Win64 only
#endif

#elif RT_ACTIVE_API_VULKAN
#error Vulkan not supported

#else
#define RT_ACTIVE_API_DEFAULT_OPENGL

#endif