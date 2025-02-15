#pragma once

// Platform detection using predefined macros
#if defined(_WIN32) && !defined(_WIN64)
	/* Windows x86 */
	#define VX_32BIT
	#error "x86 Builds are not supported!"
#elif defined(_WIN64)
	/* Windows x64  */
	#define VX_PLATFORM_WINDOWS
	#define VX_64BIT
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	* so we must check all of them (in this order)
	* to ensure that we're running on MAC
	* and not some other Apple platform */
	#if (TARGET_IPHONE_SIMULATOR == 1)
		#define SP_PLATFORM_IPHONE_SIMULATOR
		#error "IOS simulator is not supported!"
	#elif (TARGET_OS_IPHONE == 1)
		#define SP_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif (TARGET_OS_MAC == 1)
		#define SP_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define SP_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define SP_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform is not supported!"
#endif // End of platform detection