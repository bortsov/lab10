rem @echo off
rem Скрипт для запуска утилиты обмена по DCC используя JLink/LPC-Link2/ST-Link

set device=RM48L952_LPCLINK2
set speed=3000


set DCC_EXE=JLinkDCC_x64.exe
set jlinkDCC=..\utils\JLink\%DCC_EXE%
set jlinkfile="%TEMP%\runDCC.jlink"
set jlinkopt=-device %device% -if JTAG -speed %speed% -BufSize 256 -Timeout 10

echo term >%jlinkfile%

taskkill /IM %DCC_EXE%

%jlinkDCC% %jlinkopt% -CommanderScript %jlinkfile%

del %jlinkfile%
