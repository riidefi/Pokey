# Work In Progress
<p align="center">
  <img src="https://github.com/riidefi/Pokey/blob/master/banner.png">
</p>

## How does it work?
Pokey aims to seamlessly integrate custom C and C++ code into existing games. Custom code is loaded by a game-specific runtime, which can be injected directly into the dol natively or used as a code.

## Extending Game Classes
Pokey supports extending game classes with proper multiple inheritance and virtual function support. Programmers can effortlessly write C++ classes for new game elements as they were intended to be written.

### Runtime
The runtime must load the compiled code file. Runtime 2.0 also comes with Universal Bootstrap, allowing a PAL dol to boot on other region discs. A demo project that implements the patch files and much more has been included (see section below).

## Project Format

###Project Configuration File
The project configuration file (`project.json`) ties everything together, including: the module registration list, global includes, the path to external symbols, and the static base to link code at.

###The Library (`lib`) Folder
The library folder houses module-independent code and external precompiled object files which will be compiled and linked when building. The library `build.json` file directs this process.

###Modules
Pokey projects are divided into modules. Each module must have a `module.json` file, which enumerates source files to build and  (optional) additional includes.

## Usage
To build a project:
```
pokey_frontend.py <project_path> [debug|release]
```
### Hooks
In any source file, to insert a call (bl) to a defined function:
```
PokeyCall(dest, symbol);
```
To branch (b):
```
PokeyBranch(dest, symbol);
```
To write a 32 bit value:
```
PokeyWrite32(dest, value);
PokeyWritePointer(dest, value);
```
More information can be found on <a href="https://github.com/Treeki/Kamek/blob/master/preproc_demo.cpp">Kamek's page</a>.

## Demo
An easy to follow, C demo project has been provided. This project is for the PAL release of Mario Kart Wii. A more sophisticated, C++ project using Pokey can be found <a href="https://github.com/riidefi/Comet">here</a>.

## Dependencies
- Pokey runs on Python 3
- Code is linked with <a href="https://github.com/Treeki/Kamek">Kamek</a> by Treeki
- Code is compiled with Freescale <a href="http://www.freescale.com/webapp/sps/site/overview.jsp?code=CW_SPECIALEDITIONS">
    CodeWarrior Special Edition</a> for MPC55xx/MPC56xx v2.10

## Credits
- <a href="https://github.com/Treeki">Treeki</a> for his Kamek linker without which this would not be possible
- <a href="https://twitter.com/Zachruff">Zachruff</a> for the lovely banner
