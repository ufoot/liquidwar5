@echo off

rem This is a sample windows server starting script.
rem Simply click on it in explorer, and it should launch a server
rem which will popup a window each time someone connects on it.
rem You'd better run LW in windowed mode if you use this BTW.
rem You might also add the "-public" option if you don't want
rem to be prompted with it all the time. Basically replace
rem "lwwinsrv.exe -6" by "lwwinsrv.exe -public -6".
rem Also, we use -nobeep since the popup makes the default
rem beep pretymuch useless.
rem Last point: you'll need to have windows scripting host installed.

..\lwwinsrv.exe -6 -nobeep -callback "cscript lwpopup.js"


