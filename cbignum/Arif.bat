@echo off
if (%1==( %0 Arif
set r=
if (%PROCESSOR_ARCHITECTURE%==(AMD64 set r=64
Arifexp%r% -high -size -check -v %1 >%1.wrk
if errorlevel 1 pause
fc %1.res %1.wrk
pause
