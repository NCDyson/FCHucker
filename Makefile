EE_BIN = FCHucker.elf
EE_OBJS = main.o graphics.o UITex.o fonts.o libfchucker.o UITileset.o ui.o blittext.o memcard.o windowstack.o controller.o codex.o inventory.o
EE_INCS = -I$(GSKIT)/ee/dma/include
EE_LIBS_EXTRA = -lmc -lc -lpad

#all: pre-build $(EE_BIN)

#pre-build:
#	python tex2incbin.py xwindo00.png
#	python tex2incbin.py UITex.png

include Makefile.global
include Makefile.pref
