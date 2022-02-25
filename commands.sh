echo Boot:
nasm "Bootloader/boot.asm" -f bin -o "WeeBins/boot.bin" -i Bootloader
nasm "Kernel/empty_end.asm" -f bin -o "WeeBins/empty_end.bin"
../../linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/11.2.0/bin/x86_64-elf-gcc -ffreestanding -m32 -c "Kernel/kernel.cpp" -o "WeeBins/kernel.o"

nasm "Kernel/kernel_entry.asm" -f elf -o "WeeBins/kernel_entry.o"

../../linuxbrew/.linuxbrew/Cellar/x86_64-elf-binutils/2.38/bin/x86_64-elf-ld -m elf_i386 -s -o "WeeBins/kernel.bin" -Ttext 0x7e00 "WeeBins/kernel_entry.o" "WeeBins/kernel.o" --oformat binary

cat "WeeBins/boot.bin" "WeeBins/kernel.bin" > "WeeBins/short.bin"
cat "WeeBins/short.bin" "WeeBins/empty_end.bin" > os_image.bin


