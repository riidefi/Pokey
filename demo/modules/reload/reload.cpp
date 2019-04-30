#include "reload.hpp"
#include <EGG/core/eggDvdRipper.hpp>
#include <EGG/core/eggExpHeap.hpp>
#include <lib/debug/debug.hpp>
#include "../Prologue/prologue.hpp"
#include <revolution/pad.h>
#include <kamek.h>


#ifdef DEBUG

void reload()
{
	BOOL iState = OSDisableInterrupts();

	DVDFileInfo fileInfo;
	bool success = false;
	u8* block = (u8*)0x809c4fa0;//getBlock();

	DebugReport("block: %p\n", block);




	DebugReport("---\nRELOADING\n---\n");

	if (DVDOpen("Race/Course/draw_demo.szs", &fileInfo))
	{
		u32 fileLen = fileInfo.length;
		DebugReport("File len: %u\n", fileLen);
		u32 fileLen32 = OSRoundUp32B(fileLen);
		DebugReport("File len (rounded): %u\n", fileLen32);
		u32 amountRead = DVDRead(&fileInfo, block, fileLen32, 0);
		DVDClose(&fileInfo);
		if (fileLen <= amountRead)
		{
			DebugReport("Loaded file!\n");
			success = true;
		}
	}
	if (success)
	{
#if 0
		patchEntry* entry = getPatchEntries();

		DebugReport("--- Rollback %u patches ---\n", getPatchCursor());
		for (int i = 0; i < getPatchCursor(); i++)
		{
			DebugReport("-> %p was 0x%08x now 0x%08x. reverting.\n", entry->addr, entry->oldVal, *(u32*)entry->addr);
			doPatch32((void*)entry);
			entry++;
		}
#endif
	}
	else
	{
		DebugReport("Reload Failed!!\n");
	}

	
	DebugReport("Calling prologue: %p\n", block);


	// Call the prologue, again. this will load patches
	((u32(*)(void))block)();

	DebugReport("Reload Success!\n");
	OSRestoreInterrupts(iState);
}

#if 1
void loopAdditions()
{
	PADStatus padStatusArr[PAD_MAX_CONTROLLERS];

	PADRead(padStatusArr);
	if (padStatusArr[0].button & PAD_BUTTON_X)
	{
		reload();
	}

}
kmBranch(0x80009824, loopAdditions);
#endif


#endif