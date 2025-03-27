# Compiler and tools
NASM = nasm
LD = ld
MKISOFS = xorriso
GRUB_CFG = build/iso/boot/grub/grub.cfg
BOOTLOADER_SRC = bootloader/bootloader.asm
BOOTLOADER_BIN = build/bootloader.bin
ISO_DIR = build/iso

# Output files
ISO_FILE = build/Kytora.iso

# Targets
all: build/Kytora.iso

# Clean build directory
clean:
	rm -rf build/*

# Create bootloader binary
build/bootloader.bin: bootloader/bootloader.asm
	$(NASM) -f bin $< -o $@

# Create Loading binary (compile Loading.cpp)
build/loading.bin: src/Loading.cpp
	g++ -o build/loading.bin src/Loading.cpp -lgdiplus

# Create ISO file
build/Kytora.iso: build/bootloader.bin build/loading.bin
	mkdir -p $(ISO_DIR)/boot/grub
	cp build/bootloader.bin $(ISO_DIR)/boot/bootloader.bin
	cp build/loading.bin $(ISO_DIR)/boot/loading.bin  # ����� ��� Loading
	echo 'set timeout=0\nset default=0\n\nmenuentry "Kytora" {\n  multiboot /boot/bootloader.bin\n}' > $(GRUB_CFG)
	echo 'menuentry "Loading" {\n  multiboot /boot/loading.bin\n}' >> $(GRUB_CFG)  # ����� ���� �� Loading
	$(MKISOFS) -as mkisofs -o $(ISO_FILE) -b boot/bootloader.bin -c boot/boot.catalog -no-emul-boot -boot-load-size 4 -boot-info-table $(ISO_DIR)

# Build everything (default target)
build:
	make clean
	make all
