
#!/bin/bash
cd "$(dirname "$0")"

# generate new obfuscated cpp & code
python obfuscator.py

# compile and move to RELEASE folder
clang++ interpreter_obfuscated.cpp -o interpreter_obfuscated -O0 -static
rm interpreter_obfuscated.cpp
mv interpreter_obfuscated ../RELEASE/interpreter
mv code_obfuscated.txt ../RELEASE/code.txt

# do some testing... output of 255 means everything fine
printf 'wgmy{10e3b9a7cb5a88391afb2a1c56c5ac98}' | ../RELEASE/interpreter ../RELEASE/code.txt
echo $?
