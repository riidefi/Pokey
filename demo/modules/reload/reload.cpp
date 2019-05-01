#include "reload.hpp"
#include <libpokey/hooks.h>

#include <libpokey/debug.h>
#include <modules/prologue/prologue.hpp>

#include <revolution/pad.h>
#include <EGG/core/eggExpHeap.hpp>

namespace Reload {
#ifdef DEBUG

void reload()
{
	BOOL iState = OSDisableInterrupts();

	DVDFileInfo fileInfo;
	bool success = false;
	u8* block = (u8*)0x809c4fa0;//getBlock();

	PokeyDebugReport("block: %p\n", block);




	PokeyVerboseReport("---\nRELOADING\n---\n");

	if (DVDOpen(PATH_PATCH_BIN, &fileInfo))
	{
		u32 fileLen = fileInfo.length;
		PokeyDebugReport("File len: %u\n", fileLen);
		u32 fileLen32 = OSRoundUp32B(fileLen);
		PokeyDebugReport("File len (rounded): %u\n", fileLen32);
		u32 amountRead = DVDRead(&fileInfo, block, fileLen32, 0);
		DVDClose(&fileInfo);
		if (fileLen <= amountRead)
		{
			PokeyDebugReport("Loaded file!\n");
			success = true;
		}
	}
	if (success)
	{
#if 0
		patchEntry* entry = getPatchEntries();

		PokeyDebugReport("--- Rollback %u patches ---\n", getPatchCursor());
		for (int i = 0; i < getPatchCursor(); i++)
		{
			PokeyDebugReport("-> %p was 0x%08x now 0x%08x. reverting.\n", entry->addr, entry->oldVal, *(u32*)entry->addr);
			doPatch32((void*)entry);
			entry++;
		}
#endif
	}
	else
	{
		PokeyDebugReport("Reload Failed!\n");
	}

	
	PokeyDebugReport("Calling prologue: %p\n", block);


	// Call the prologue, again. this will load patches
	((u32(*)(void))block)();

	PokeyDebugReport("Reload Success!\n");
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
PokeyBranch(0x80009824, loopAdditions);
#endif


#endif
} // namespace Reload