COMPILER = GCC
_CORE_FILES = fcc.c fparse.c
_CORE_FLAGS = -Wall

_CCFILES = flex.c ftokens.c
_CCFLAGS = -o fcc

_GRFILES = grlex.c grtokens.c
_GRFLAGS = -o fgr

_CC      = gcc

ifeq ($(COMPILER), GCC)
	_CORE_FLAGS += --define GCC -g
endif
ifeq ($(COMPILER), FCC)
	_CORE_FILES += fstd.c
	_CC = fcc
endif

fgr: $(_CORE_FILES) $(_GRFILES) makefile
	$(_CC) $(_CORE_FILES) $(_GRFILES) $(_CORE_FLAGS) $(_GRFLAGS)

fcc: $(_CORE_FILES) $(_CCFILES) makefile
	$(_CC) $(_CORE_FILES) $(_CCFILES) $(_CORE_FLAGS) $(_CCFLAGS) 
