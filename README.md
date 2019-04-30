# Work In Progress
<p align="center">
  <img src="https://github.com/riidefi/Pokey/blob/master/banner.png">
</p>

## How does it work?
C and C++ code is compiled and linked into a static object as well as a supplemntary patch file containing precomputed hooks.

### Hooks
In any source file, to insert a call (bl) to a defined function:
```c++
kmCall(dest, symbol);
```
To branch (b):
```c++
kmBranch(dest, symbol);
```
To write a 32 bit value:
```C++
kmWrite32
```
More information can be found on <a href="https://github.com/Treeki/Kamek/blob/master/preproc_demo.cpp">Kamek's page</a>.

### Runtime
The runtime must load the patch file. Support for 16 and 8 bit writes is planned. A demo project that implements the patch files and much more has been included (see section below).
#### Patch format
Patches are currently stored as a null terminated sequence of 32 bit address-value pairs.

## Project Format
TODO

## Usage
To build a project:
```
pokey_frontend.py <project_path> [debug|release]
```

## Demo
A demo project has been provided. This project is for the PAL release of Mario Kart Wii.

## Dependencies
- Pokey runs on Python 3
- Code is linked with <a href="https://github.com/Treeki/Kamek">Kamek</a> by Treeki
- Code is compiled with Freescale <a href="http://www.freescale.com/webapp/sps/site/overview.jsp?code=CW_SPECIALEDITIONS">
    CodeWarrior Special Edition</a> for MPC55xx/MPC56xx v2.10

## Credits
- <a href="https://github.com/Treeki">Treeki</a> for his Kamek linker without which this would not be possible
- <a href="https://twitter.com/Zachruff">Zachruff</a> for the lovely banner
