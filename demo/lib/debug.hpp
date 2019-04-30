#pragma once
#include <revolution/os.h>


#ifdef DEBUG
	//#define RM_ENABLE_VERBOSE_REPORT
	#define RM_ENABLE_DEBUG_REPORT
	// Critical problems that are not entirely unlikely to happen
	#define RM_ENABLE_CRITICAL_ASSERT
	// Unlikely critical problems
	#define RM_ENABLE_UNLIKELY
	// Nonfatal problems
	#define RM_ENABLE_CHECK


	// Display asserts on-screen
	// Useful for debugging on retail wiis
	#define RM_VISUAL_FAIL


	#define RM_BG_COLOR (GXColor) {00, 0x1b, 0x58  }
	#define RM_FG_COLOR (GXColor) { 0xff,0xff,0xff, 0xff }

	#define RIICLIENT_FATAL_HEADER ""
	#define RIICLIENT_FATAL_FOOTER "\n\n\nPokey v1.0 by Riidefi\n"

#endif

// backwards compat
#define DebugAssert RMCriticalAssert
#define DebugReport RMDebugReport

// https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

// todo: clean up
#ifndef RM_VISUAL_FAIL
	#define RMAssert(exp) (void) ((exp) || (RMFailAssert(__FILE__, __LINE__, "Failed Assert: " #exp), 0))
	#define RMFailAssert(file, line, msg) OSPanic(file, line, msg)
#else
	#define RMAssert(exp) (void) ((exp) || (RMFailAssert(__FILE__, LINE_STRING, "Failed Assert: " #exp), 0))
	#define RMFailAssert(file, line, msg) OSFatal(RM_FG_COLOR, RM_BG_COLOR, RIICLIENT_FATAL_HEADER msg "\nin " file ":" line RIICLIENT_FATAL_FOOTER )
#endif


#define RMPass(...)   ((void) 0)


#ifdef RM_ENABLE_DEBUG_REPORT
	#define RMDebugReport(...) OSReport("["__FILE__ ":" LINE_STRING "] " __VA_ARGS__ )
#else
	#define RMDebugReport RMPass
#endif

#ifdef RM_ENABLE_VERBOSE_REPORT
	#define RMVerboseReport RMDebugReport
#else
	#define RMVerboseReport RMPass
#endif

#ifdef RM_ENABLE_CRITICAL_ASSERT
	#define RMCriticalAssert RMAssert
#else
	#define RMCriticalAssert RMPass
#endif
#ifdef RM_ENABLE_UNLIKELY
	#define RMUnlikelyAssert RMAssert
#else
	#define RMUnlikelyAssert RMPass
#endif
#ifdef RM_ENABLE_CHECK
	#define RMCheck RMAssert
#else
	#define RMCheck RMPass
#endif