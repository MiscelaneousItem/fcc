COMPILER = GCC
_CCFILES = fcc.c flex.c fparse.c ftokens.c
_CCFLAGS = -Wall -o fcc
_CC      = gcc

ifeq ($(COMPILER), GCC)
	_CCFLAGS += --define GCC -g
endif
ifeq ($(COMPILER), FCC)
	_CCFILES += fstd.c
	_CC = fcc
endif

main:
	$(_CC) $(_CCFILES) $(_CCFLAGS)
