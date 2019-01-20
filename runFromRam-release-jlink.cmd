@echo off
rem Скрипт для загрузки программы в ОЗУ и её запуска используя JLink/LPC-Link2/ST-Link
rem Используются команды "r0" и "r1" для управления контактом RESET; LPC-Link2 имеет ошибку в ПО, которая приводит к невозможности управления контактом "RESET" с помощью команды "r"
rem После загрузки программы в ОЗУ управление передаётся на адрес 0x08000000 (начало ОЗУ)

set hexfile=objs\release\motorHw.srec
set device=RM48L952_LPCLINK2
set jlinkcmd=..\utils\JLink\JLink.exe
set speed=3000

set jlinkopt=-device %device% -if JTAG -jtagconf -1,-1 -speed %speed% -Autoconnect 1 -ExitOnError 1
set jlinkfile="%TEMP%\loadToHerculesRam.jlink"

echo connect >%jlinkfile%
echo r0 >>%jlinkfile%
echo r1 >>%jlinkfile%
echo r >>%jlinkfile%
echo loadfile %hexfile% >>%jlinkfile%
echo SetPC 0x08000000 >>%jlinkfile%
echo g >>%jlinkfile%
echo q >>%jlinkfile%

%jlinkcmd% %jlinkopt% -CommandFile %jlinkfile%

del %jlinkfile%

