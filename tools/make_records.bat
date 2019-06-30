@echo off
cd ../tmp/
echo %date% >> records.txt
cloc --quiet ../ >> records.txt
echo --- >> records.txt