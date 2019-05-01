#pragma once

#include <Common/rk_types.h>

// deprecated patch history
#if 0
//! @brief What was at the address before it was patched. internally array of 30 entries
struct patchEntry
{
	void* addr;
	u32 oldVal;
};
#define NUM_PATCH_HISTORY 30
patchEntry* getPatchEntries();
int getPatchCursor();
#endif

namespace Prologue {

//! @brief apply the patches
void applyPatches(u32* block);

} // namespace Prologue