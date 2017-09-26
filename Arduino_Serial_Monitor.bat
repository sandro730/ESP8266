

@echo off
set TERATERM_PATH=D:\Programmi_Home\Arduino\teraterm-4.96
set TERATERM_EXE=D:\Programmi_Home\Arduino\teraterm-4.96\ttermpro.exe
set TERATERM_TITLE=Tera Term - *
set TERATERM_PORT=5 COM5
set TERATERM_BAUD=19200
tasklist /FI "WINDOWTITLE eq %TERATERM_TITLE%"
TASKKILL /F /FI "WINDOWTITLE eq %TERATERM_TITLE%"
%TERATERM_EXE% /C=%TERATERM_PORT% /BAUD=%TERATERM_BAUD%
