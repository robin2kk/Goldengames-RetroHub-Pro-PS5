ifndef PS5_PAYLOAD_SDK
$(error Set PS5_PAYLOAD_SDK to the public payload SDK v0.42)
endif
include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
all: return-chord.elf
return-chord.elf: main.c chord.h
	$(CC) -std=c11 -O2 -Wall -Wextra -Werror -o $@ main.c -lScePad -lSceUserService -lSceSysmodule
