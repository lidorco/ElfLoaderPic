cmake . -Bbuild-output
cd build-output;
make

echo run this to check shellcode:
echo ./build-output/tests/pic_runner.elf ./build-output/shellcode.pic