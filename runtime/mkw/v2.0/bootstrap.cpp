#include "standalone.hpp"
#include <libpokey/hooks.h>
// filepaths from all regions combined
static const char* BootStrapFilePaths[] = {
	"/Boot/Strap/jp/jp.szs", // 0
	"/Boot/Strap/eu/English.szs", // 1
	"/Boot/Strap/eu/German.szs", // 2
	"/Boot/Strap/eu/French.szs", // 3
	// "/Boot/Strap/eu/Spanish_US.szs"
	"/Boot/Strap/eu/Spanish_EU.szs", // 4
	"/Boot/Strap/eu/Italian.szs", // 5
	"/Boot/Strap/eu/Dutch.szs" // 6
};

const char** getBootStrapFilePaths()
{
	return BootStrapFilePaths;
}

u8* bootStrapLoad(System::SystemManager* pManager, char* path, EGG::Heap* heap, bool allocTop, u32* fsizeOutput)
{
	DVDFileInfo fInfo;


	if (DVDOpen(path, &fInfo))
		goto out;

	// Support for US Spanish
	if (!strcmp(path, BootStrapFilePaths[4]))
	{
		path = (char*)"/Boot/Strap/us/Spanish_US.szs";
		if (DVDOpen(path, &fInfo))
			goto out;
	}
	// Support for US French
	if (!strcmp(path, BootStrapFilePaths[3]))
	{
		path = (char*)"/Boot/Strap/us/French.szs";
		if (DVDOpen(path, &fInfo))
			goto out;
	}

	path = (char*)BootStrapFilePaths[1]; // Default to EU English
	if (DVDOpen(path, &fInfo))
		goto out;

	path = (char*)"/Boot/Strap/us/English.szs"; // US english
	if (DVDOpen(path, &fInfo))
		goto out;
	
	path = (char*)BootStrapFilePaths[0]; // Japan
	if (DVDOpen(path, &fInfo))
		goto out;
	
	// Fatal: file doesn't exist
	
	{
		u32 fg = 0x000000ff, bg = 0xffffffff;
		Fatal(&fg, &bg, "Failed to load bootstrap file!\n");
		return 0;
	}
	
	
out:
	DVDClose(&fInfo);
	return pManager->ripFromDisc((const char*)path, heap, allocTop, fsizeOutput);
}
// Overwrite load to game table.. needs to load to r25 not r3
//PokeyCall(0x80007500, getBootStrapFilePaths);

// Overwrite call to rip file
PokeyCall(0x80007528, bootStrapLoad);
