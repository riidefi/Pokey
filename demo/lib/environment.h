#pragma once

#define DATA_HACK


// TODO: Network load

#ifdef DATA_HACK
#define PATH_CODE_BIN "Race/Course/draw_demo.szs"
#define PATH_PATCH_BIN "Race/Course/draw_demo_d.szs"

#else
#define PATH_CODE_BIN "CODE.bin"
#define PATH_PATCH_BIN "PATCH.bin"
#endif
