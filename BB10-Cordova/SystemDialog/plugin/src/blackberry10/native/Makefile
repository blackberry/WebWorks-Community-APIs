QMAKE_TARGET = SysDialog
PROJECT_DIR := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

QMAKE = $(QNX_HOST)/usr/bin/qmake

ifdef NUMBER_OF_PROCESSORS
  MAKEFLAGS += -j${NUMBER_OF_PROCESSORS}
endif

default: Device-Release

.PHONY: FORCE

Makefile-all: device/Makefile simulator/Makefile

device/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p device
	cd device && $(QMAKE) -spec blackberry-armv7le-qcc ../$(QMAKE_TARGET).pro CONFIG+=release CONFIG+=device

simulator/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p simulator
	cd simulator && $(QMAKE) -spec blackberry-x86-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug CONFIG+=simulator

Device-Release: device/Makefile FORCE
	$(MAKE) -C ./device -f Makefile

Simulator-Debug: simulator/Makefile FORCE
	$(MAKE) -C ./simulator -f Makefile

clean: FORCE
	rm -rf device simulator

