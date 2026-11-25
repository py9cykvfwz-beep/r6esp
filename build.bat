@echo off
cl /EHsc /MD /I C:\path\to\gdiplus\include src\main.cpp src\overlay.cpp /link /LIBPATH:C:\path\to\gdiplus\lib gdiplus.lib
move main.obj build\
move overlay.obj build\
link /OUT:R6ESP.exe /SUBSYSTEM:WINDOWS build\main.obj build\overlay.obj gdiplus.lib
del build\main.obj
del build\overlay.obj
