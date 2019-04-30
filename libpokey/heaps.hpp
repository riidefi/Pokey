#pragma once

#include <EGG/core/eggExpHeap.hpp>

inline
EGG::ExpHeap* getMem1Heap()
{
	__asm {
		lwz r3, -0x6C38(r13);
		lwz r3, 0x18(r3);
	}
}