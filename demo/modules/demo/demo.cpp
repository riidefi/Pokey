#include <libpokey\debug.hpp>

namespace Demo {

void prologue()
{
	// Disable multitasking while recording the prior interrupt state.
	bool iState = OSDisableInterrupts();

	DebugReport("Applying debug bar...\n");
	
	// Modify control flow to enable the bar.
	applyProcessBar();
	
	DebugReport("...Applied!\n");
	
	// Restore our previous interrupt state.
	OSRestoreInterrupts(iState);
}


/*-------------------------------------------------------

NOTE:
- Patches are only done this way for demonstration.
- Using kamek macros would be preferable!

-------------------------------------------------------*/



inline void Write32(void* addr, u32 val)
{ __asm {
	stw r4, 0(r3); // store value

	// update cache
	dcbf 0, r3;
	sync;
	icbi 0, r3;
	isync;
} }

void applyProcessBar()
{
	/*
	$Process Bar (Riidefi) (PAL)
	04238F14 4E800020
	04009600 48239100
	0424271C 4BDC6EF0
	0400979C 4823912C
	042428E4 4BDC6EC4
	*/
	Write32((void*)0x80009600, 0x48239100);
	Write32((void*)0x8024271C, 0x4BDC6EF0);
	Write32((void*)0x8000979C, 0x4823912C);
	Write32((void*)0x802428E4, 0x4BDC6EC4);
}

} // namespace Demo