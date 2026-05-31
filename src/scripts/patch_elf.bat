@echo off
echo -------------------------------------------------------------------------------
echo PAtching ROM
echo -------------------------------------------------------------------------------
C:/HAM/tools/win32/gbafix.exe m4g.elf -tm4g
echo ROM fixed!
echo -------------------------------------------------------------------------------
echo Appending datafile
echo -------------------------------------------------------------------------------
move m4g.elf m4g.bin
tools\padbin.exe 256 m4g.bin
copy /b m4g.bin+data.gbfs m4g.elf	