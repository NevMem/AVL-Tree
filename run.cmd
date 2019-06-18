@echo off

g++ avl.cpp test.cpp -o res.exe || goto END

res.exe

:END