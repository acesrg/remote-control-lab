PROGRAM=main
PROGRAM_INC_DIR= ./include ./third_party/log.c/src/
PROGRAM_SRC_DIR= . ./third_party/log.c/src/

EXTRA_CFLAGS=-DLOG_USE_COLOR
EXTRA_COMPONENTS = extras/pwm

LINT_TOOL = cpplint
LINT_FLAGS = --recursive --quiet

ifdef YOUARECONTAINERIZEDBUDDY
include ../../common.mk
endif

dockeride:
ifndef YOUARECONTAINERIZEDBUDDY
	../../tools/dockeride
else
	echo "Alredy dockerized, pal!"
endif

unittest:
	echo "no tests right now"

lint:
	$(LINT_TOOL) $(LINT_FLAGS) *

monitor:
	$(FILTEROUTPUT) --port $(ESPPORT) --baud 115200 --elf $(PROGRAM_OUT)

flash_after_build:
	$(if will_flash, $(call will_flash, "flash"))
	$(ESPTOOL) -p $(ESPPORT) --baud $(ESPBAUD) write_flash $(ESPTOOL_ARGS) \
		$(RBOOT_ARGS) 0x2000 $(CI_PROJECT_DIR)/*.bin $(SPIFFS_ESPTOOL_ARGS)
	$(if did_flash, $(call did_flash, "flash"))
