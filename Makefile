GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -I $(srcdir) -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

srcdir = src/
bindir = bin/

name = kernel
bin = $(name).bin
iso = $(name).iso
grub = grub.cfg

SRC = $(shell find $(srcdir) -regex ".*\.s\|.*\.cpp")
OBJ = $(subst $(srcdir), $(bindir), $(patsubst %.s, %.o, $(patsubst %.cpp, %.o, $(SRC))))

$(bindir)%.o : $(srcdir)%.cpp $(srcdir)%.h
	mkdir -p $(dir $@)
	g++ $(GPPPARAMS) -o $@ -c $<

$(bindir)%.o : $(srcdir)%.cpp
	mkdir -p $(dir $@)
	g++ $(GPPPARAMS) -o $@ -c $<

$(bindir)%.o : $(srcdir)%.s
	mkdir -p $(dir $@)
	as $(ASPARAMS) -o $@ $^

bin : $(bin)
$(bin) : linker.ld $(OBJ)
	echo $(SRC)
	ld $(LDPARAMS) -T $< -o $@ $(OBJ)

iso : $(iso)
$(iso) : $(bin)
	mkdir -p iso/boot/grub
	cp $(bin) iso/boot
	cp $(grub) iso/boot/grub
	grub-mkrescue --output=$@ iso
	rm -rf iso

run : $(iso)
	qemu-system-x86_64 $^

box : $(iso)
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "My kernel"  &

clean :
	rm -rf $(bindir) $(bin) $(iso)
