#pragma once
#include <revolution/os.h>


#ifdef DEBUG

	//#define POKEY_ENABLE_VERBOSE_REPORT
	#define POKEY_ENABLE_DEBUG_REPORT
	// Critical problems that are not entirely unlikely to happen
	#define POKEY_ENABLE_CRITICAL_ASSERT
	// Unlikely critical problems
	#define POKEY_ENABLE_UNLIKELY
	// Nonfatal problems
	#define POKEY_ENABLE_CHECK


	// Display asserts on-screen
	// Useful for debugging on retail wiis
	#define POKEY_VISUAL_FAIL


	#define POKEY_BG_COLOR (GXColor) {00, 0x1b, 0x58  }
	#define POKEY_FG_COLOR (GXColor) { 0xff,0xff,0xff, 0xff }

	#define POKEY_FATAL_HEADER ""
	#define POKEY_FATAL_FOOTER "\n\n\nPokey v1.0 by Riidefi\n"
#endif

// backwards compat
#define DebugAssert PokeyCriticalAssert
#define DebugReport PokeyDebugReport

// https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

// todo: clean up
#ifndef POKEY_VISUAL_FAIL
	#define PokeyAssert(exp) (void) ((exp) || (PokeyFailAssert(__FILE__, __LINE__, "Failed Assert: " #exp), 0))
	#define PokeyFailAssert(file, line, msg) OSPanic(file, line, msg)
#else
	#define PokeyAssert(exp) (void) ((exp) || (PokeyFailAssert(__FILE__, LINE_STRING, "Failed Assert: " #exp), 0))
	#define PokeyFailAssert(file, line, msg) OSFatal(POKEY_FG_COLOR, POKEY_BG_COLOR, POKEY_FATAL_HEADER msg "\nin " file ":" line POKEY_FATAL_FOOTER )
#endif


#define PokeyPass(...)   ((void) 0)


#ifdef POKEY_ENABLE_DEBUG_REPORT
	#define PokeyDebugReport(...) OSReport("["__FILE__ ":" LINE_STRING "] " __VA_ARGS__ )
#else
	#define PokeyDebugReport PokeyPass
#endif

#ifdef POKEY_ENABLE_VERBOSE_REPORT
	#define PokeyVerboseReport PokeyDebugReport
#else
	#define PokeyVerboseReport PokeyPass
#endif

#ifdef POKEY_ENABLE_CRITICAL_ASSERT
	#define PokeyCriticalAssert PokeyAssert
#else
	#define PokeyCriticalAssert PokeyPass
#endif
#ifdef POKEY_ENABLE_UNLIKELY
	#define PokeyUnlikelyAssert PokeyAssert
#else
	#define PokeyUnlikelyAssert PokeyPass
#endif
#ifdef POKEY_ENABLE_CHECK
	#define PokeyCheck PokeyAssert
#else
	#define PokeyCheck PokeyPass
#endif