@echo off
gcc -Wall -O2 gbfs.c djbasename.c -o gbfs.exe
gcc -Wall -O2 insgbfs.c -o insgbfs.exe
gcc -Wall -O2 lsgbfs.c -o lsgbfs.exe
gcc -Wall -O2 ungbfs.c -o ungbfs.exe
gcc -Wall -O2 padbin.c -o padbin.exe
gcc -Wall -O2 bin2s.c -o bin2s.exe
