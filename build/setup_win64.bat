@echo off

mkdir pongViper-win64
cd pongViper-win64
cmake -G "Visual Studio 16 2019" ../../src/ga1-core
cd ..