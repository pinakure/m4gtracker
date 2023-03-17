@echo off
cls
color 2a
echo -------------------------------------------------------------------------------
echo BUILDING M4GTRACKER
echo -------------------------------------------------------------------------------
update_timestamp > timestamp.h
call clean
call compile