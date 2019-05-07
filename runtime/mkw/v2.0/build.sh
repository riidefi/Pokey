# Based on kamek loader build script

CW_PATH=../../tool/
CPPFILES="bootstrap loader"

CC=$CW_PATH/mwcceppc
CFLAGS="-i . -I- -i ../../ -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0"

for i in $CPPFILES
do
	echo Compiling $i.cpp...
	$CC $CFLAGS -c -o $i.o $i.cpp
done

echo Linking...

../../tool/Kamek.exe bootstrap.o loader.o -static=0x80218FF0 -output-gecko=loader.txt -externals="../externals/RMCP01.txt" -input-dol="../../tool/main.dol" -output-dol=comet.dol