#!/usr/bin/make -f
# Makefile for DPF #
# ---------------- #
# Created by falkTX
#

include dpf/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

ifneq ($(CROSS_COMPILING),true)
CAN_GENERATE_TTL = true
else ifneq ($(EXE_WRAPPER),)
CAN_GENERATE_TTL = true
endif

dgl:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl opengl
endif

plugins: dgl
	$(MAKE) all -C plugins/ScaleSpace

ifeq ($(CAN_GENERATE_TTL),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

tests: dgl
	$(MAKE) -C tests

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C plugins/ScaleSpace
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	rm -f plugins/ScaleSpace/lib/DPFDearImGuiWidgets/opengl/*.d
	rm -f plugins/ScaleSpace/lib/DPFDearImGuiWidgets/opengl/*.o
	rm -rf bin build

# --------------------------------------------------------------

.PHONY: dgl plugins tests
