@echo off

g++ test.cpp -o res.exe || goto END

res.exe

:END