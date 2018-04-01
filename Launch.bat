@echo off
setlocal EnableDelayedExpansion
 
server.exe
echo Server Done!

set t0=!time!
core.exe 0 ./../
set t1=!time!
call :difftime
 
pause & exit /B
 
:difftime
  for /F "tokens=1-8 delims=:.," %%a in ("!t0: =0!:!t1: =0!") do set /a "a=(((1%%e-1%%a)*60)+1%%f-1%%b)*6000+1%%g%%h-1%%c%%d, a+=(a>>31) & 8640000"
  echo !a! cs
goto :eof