GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

srcdir = src/
bindir = bin/

name = kernel
bin = $(name).bin
iso = $(name).iso
grub = grub.cfg

SRC = $(wildcard $(srcdir)*.s) $(wildcard $(srcdir)*cpp)
OBJ = $(subst $(srcdir), $(bindir), $(patsubst %.s, %.o, $(patsubst %.cpp, %.o, $(SRC))))

$(bindir)%.o : $(srcdir)%.cpp
	g++ $(GPPPARAMS) -o $@ -c $^

$(bindir)%.o : $(srcdir)%.s
	as $(ASPARAMS) -o $@ $^

bin : $(bin)
$(bin) : linker.ld $(OBJ)
	ld $(LDPARAMS) -T $< -o $@ $(OBJ)

iso : $(iso)
$(iso): $(bin)
	mkdir -p iso/boot/grub
	cp $(bin) iso/boot
	cp $(grub) iso/boot/grub
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: $(iso)
	qemu-system-x86_64 $^

clean :
	rm -rf $(bindir)* $(bin) $(iso)
