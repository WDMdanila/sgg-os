GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	obj/std.o \
	obj/gdt.o \
	obj/port.o \
	obj/interrupts.o \
	obj/interruptstubs.o \
	obj/kernel.o

.PHONY: all
all: kernel.iso

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	mv $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SGG Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso/

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

obj/%.o: src/%.cpp
	mkdir -p $(@D) || true
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D) || true
	as $(ASPARAMS) -o $@ $<

.PHONY: clean
clean:
	rm -rf obj || true
	rm -rf iso || true