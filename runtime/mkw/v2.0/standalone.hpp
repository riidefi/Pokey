#pragma once


extern "C" void OSReport(const char* str, ...);

typedef unsigned long u32;
typedef unsigned char u8;
#define NULL 0
#define kmCall PokeyCall
#ifdef DEBUG
// https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define DebugReport(...) OSReport("["__FILE__ ":" LINE_STRING "] " __VA_ARGS__ )
#else
#define DebugReport(...) ((void) 0)
#endif


// points to OSFatal with colors passed by structure (EABI calls to pass as pointer to callee stack)
extern "C" void Fatal(u32* fg, u32* bg, const char* msg);

namespace EGG
{
struct Heap
{
	virtual ~Heap();
	virtual int getHeapKind() const = 0;
	virtual void initAllocator(void* allocator, int align) = 0;
	virtual void* alloc(u32 size, int align) = 0;
	virtual void free(void* block) = 0;
	virtual void destroy() = 0;
	virtual u32 resizeForMBlock(void* block, u32 size) = 0;
	virtual u32 getAllocatableSize(int align) = 0;
	virtual u32 adjust() = 0;
	static void* alloc(u32 size, int align, Heap* heap);
};

struct DvdRipper {

	//! @brief Describes the direction of allocating new blocks in a free memory region.
	//!
	enum EAllocDirection
	{
		ALLOC_DIR_PAD,    // Unseen/unhandled so far
		ALLOC_DIR_TOP,	  //!< [1] Negative alignment; allocate block from top of free block.
		ALLOC_DIR_BOTTOM  //!< [2] Positive alignment; allocate block from bottom of free block.
	};
	static u8* loadToMainRAM(const char* path, u8* dst, Heap* pHeap, EAllocDirection allocDirection, u32 offset, u32* pAmountRead, u32* pFileSize);
};
}
namespace System
{
	struct ModuleLinker
	{
		bool callModule(int moduleID);
	};
	struct SystemManager
	{
		static u8* ripFromDisc(const char path[64], EGG::Heap* heap, bool allocTop, u32* fsizeOutput);
	};
}
extern "C" int strcmp(
	char const* _Str1,
	char const* _Str2
);
struct DVDFileInfo
{
	u8 _[0x3C];
};
extern "C" int DVDOpen(const char* path, DVDFileInfo* fInfo);
extern "C" void DVDClose(DVDFileInfo* fInfo);
extern "C" int DVDReadPrio(DVDFileInfo* fInfo, void* addr, int len, int offs, int prio);
