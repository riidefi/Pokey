#pragma once


extern "C"
{
    bool OSDisableInterrupts();
    bool OSEnableInterrupts();
    bool OSRestoreInterrupts(bool stat);
    void OSReport(const char* str, ...);
}

namespace Demo {

void applyProcessBar();
void prologue();

} // namespace Demo