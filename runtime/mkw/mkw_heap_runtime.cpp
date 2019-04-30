#define PAL
#include <kamek.h>

#include <lib/debug.hpp>
#include <lib/heaps.hpp>
#include <lib/environment.h>

#include <Static/ModuleLinker.hpp>
#include <EGG/core/eggExpHeap.hpp>
#include <EGG/core/eggDvdRipper.hpp>


bool myCallModule(System::ModuleLinker* linker, int moduleID)
{
	u8* block;
	// Before we call the module, let's add our code
	EGG::ExpHeap* pHeap = getMem1Heap();


	if (!pHeap)
		DebugReport("Could not acquire MEM1 heap!");
	else if (!(block = EGG::DvdRipper::loadToMainRAM(PATH_CODE_BIN, NULL, pHeap, EGG::DvdRipper::ALLOC_DIR_TOP, 0, NULL, NULL)))
		DebugReport("Couldn't rip file!");
	else if ((u32)block != 0x809c4fa0)
	{
		DebugReport("Block allocated at unexpected location (%p)!\n", block);
		pHeap->free(block);
	}
	else
	{
		DebugReport("Block successfully allocated at expected location!\n");
		((u32(*)(void))block)();		
	}
	
	// Call down to the original module
	linker->callModule(moduleID);
}

kmCall(0x80007B5C, myCallModule);