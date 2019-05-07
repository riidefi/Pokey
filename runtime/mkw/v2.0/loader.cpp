#define DEBUG

#include "standalone.hpp"
#include <libpokey/hooks.h>
#include <libpokey/mkw/environment.h>



inline
EGG::Heap* getMem1Heap()
{
	__asm {
		lwz r3, -0x6C38(r13);
		lwz r3, 0x18(r3);
	}
}

kmCallDefCpp(0x80007B5C, bool, System::ModuleLinker* linker, int moduleID)
{
	void* block;
	DVDFileInfo fileInfo;
	u32 fileLenRounded;
	// Before we call the module, let's add our code
	EGG::Heap* pHeap = getMem1Heap();

	if (pHeap)
	{
		DebugReport("Loaded heap!\n");
	}
	else
	{
		DebugReport("Could not acquire MEM1 heap!\n");
		goto out;
	}
	if (!DVDOpen(PATH_CODE_BIN, &fileInfo))
	{
		DebugReport("Failed to open code file!\n");
		goto out;
	}
	
	fileLenRounded = (((u32*)&fileInfo)[13] + 31) & ~31;
	DebugReport("Expecting %u bytes...\n", fileLenRounded);
	
	block = EGG::Heap::alloc(fileLenRounded + 2048, 32, pHeap);

	if ((u32)block != 0x809c4fa0)
	{
		DebugReport("Block allocated at unexpected location (%p)!\n", block);

		u32 dif = 0x809c4fa0 - (u32)block;

		if (dif <= 2048)
		{
			DebugReport("Off by %u.. within 2048 room for error\n", dif);
			block = (void*)0x809c4fa0;
			goto call;
		}

		pHeap->free(block);
		goto out;
	}
	else
	{
		call:
		DebugReport("Block successfully allocated at expected location!\n");
		DVDReadPrio(&fileInfo, block, fileLenRounded, 0, 2);
		DVDClose(&fileInfo);
		((u32(*)(void))block)();
	}
	
	
	

out:
	// Call down to the original module
	return linker->callModule(moduleID);
}