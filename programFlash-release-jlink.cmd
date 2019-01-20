@echo off
rem Скрипт для загрузки программы в Flash и её запуска используя JLink/LPC-Link2
rem Используются команды "r0" и "r1" для управления контактом RESET; LPC-Link2 имеет ошибку в ПО, которая приводит к невозможности управления контактом "RESET" с помощью команды "r"
rem После программирования Flash осуществляется рестарт МК

set hexfile=objs\release\motorHw.hex
set device=RM48L952_LPCLINK2
set jlinkcmd=..\utils\JLink\JLink.exe
set speed=3000

set jlinkopt=-device %device% -if JTAG -jtagconf -1,-1 -speed %speed% -Autoconnect 1 -ExitOnError 1
set jlinkfile="%TEMP%\loadToHerculesFlash.jlink"

echo connect >%jlinkfile%
echo r0 >>%jlinkfile%
echo r1 >>%jlinkfile%
echo r >>%jlinkfile%
echo loadfile %hexfile% >>%jlinkfile%
echo r0 >>%jlinkfile%
echo r1 >>%jlinkfile%
echo r >>%jlinkfile%
echo g >>%jlinkfile%
echo q >>%jlinkfile%

%jlinkcmd% %jlinkopt% -CommandFile %jlinkfile%

del %jlinkfile%

