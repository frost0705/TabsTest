@echo off
if (%1==( %0 Arifr
set r=
if (%PROCESSOR_ARCHITECTURE%==(AMD64 set r=64
Arifexp%r% -hexo -par 2 %1 >%1.wrk
for %%f in (0 1 2 3 4 5 6 7) do find /C "%%f" %1.wrk
pause
for %%f in (8 9 a b c d e f) do find /C "%%f" %1.wrk
pause
