# Work In Progress
<p align="center">
  <img src="https://github.com/riidefi/Pokey/blob/master/banner.png">
</p>

## How does it work?
C and C++ code is compiled and linked into a static object. A patch file is generated from the special Kamek object file section populated from hook macros.

### Hooks
TODO

### Runtime
which a runtime must load. A demo project that implements the patch files (and even reloading) has been included.
### Demo
TODO

## Usage
To build a project:
```
pokey_frontend.py <project_path> [debug|release]
```
TODO: Add information about project files

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
