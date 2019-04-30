#include "prologue.hpp"

#include <EGG/core/eggDvdRipper.hpp>
#include <EGG/core/eggExpHeap.hpp>

#include <Common/rk_types.h>

#include <lib/debug/debug.hpp>
#include <lib/heaps.hpp>
#include <lib/environment.h>

// for ctors
#include <modules/demo/demo.hpp>


void applyPatches(u32* block);
void doPatch32(void* patch);

void ctor_top();

void* __constructors[] =
{
	ctor_head,

	// Debug module init function
	Demo::prologue,

	ctor_tail,
	0 // NULL-terminated
};


// The first function will be the entry point.
void prologue()
{
	u32 patch_block[30 * 2 + (8)];
	DVDFileInfo fileInfo;
	BOOL iState = OSDisableInterrupts();
	u32 amountRead;
	void* buf = (void*)OSRoundUp32B((u32)&patch_block);

	// Read patches from disc
	if (DVDOpen(PATH_PATCH_BIN, &fileInfo))
	{
		u32 fileLen = fileInfo.length;

		u32 fileLen32 = OSRoundUp32B(fileLen);

		if (fileLen32 > 30 * 2 * 4)
		{
			DebugReport("Too many patches (max 30)!\n");
			goto fail;
		}

		amountRead = DVDRead(&fileInfo, buf, fileLen32, 0);
		DVDClose(&fileInfo);
		if (fileLen32 > amountRead)
		{
			fail:
			DebugReport("Failed to load PATCH.bin!\n");
		}
		else
		{
			// NULL terminated
			applyPatches((u32*)buf);
			DebugReport("Success!\n");
		}
	}

	// call prologue functions
	for (u32* f = (u32*)__constructors; *f; f++)
		((void(*)(void))*f)();
	DebugReport("DONE\n");

	RustTest();


	OSRestoreInterrupts(iState);
}
void ctor_head()
{
	DebugReport("Calling constructors...\n");
}
void ctor_tail()
{
	DebugReport("...done!\n");
}

#if 0
// ensure enough space for all patches

patchEntry patchRecord[NUM_PATCH_HISTORY];
int patchCursor = 0;

patchEntry* getPatchEntries()
{
	return &patchRecord[0];
}
int getPatchCursor()
{
	return patchCursor;
}

#endif
inline void applyPatches(u32* block)
{
	int i;
	for (i = 0; block[i]; i += 2)
	{
		u32* dst = (u32*)block[i];
		u32  val = block[i + 1];
#if 0
		patchRecord[patchCursor].addr = dst;
		patchRecord[patchCursor].oldVal = *dst;
		patchCursor++;
#endif
		DebugReport("Patching 32: %p = 0x%08x\n", dst, val);
		doPatch32(&block[i]);
	}
	DebugReport("Made %u patches!\n", i / 2);
}
inline void doPatch32(void* patch)
{

	__asm
	{
		lwz r4, 4(r3) // value
		lwz r3, 0(r3) // address to write to
		stw r4, 0(r3)

		// cache
		dcbf 0, r3
		sync
		icbi 0, r3
		isync
	}
}