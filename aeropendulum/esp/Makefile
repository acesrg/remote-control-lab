PROGRAM=http_server
PROGRAM_INC_DIR=./include ./lib/json/inc/ ./callbacks/inc/ ./third_party/log.c/src/ ./lib/driver/inc/ ./lib/cgi/inc/ ./lib/propeller/inc/
PROGRAM_SRC_DIR=. ./lib/json/src/ ./callbacks/src/ ./tasks/ ./third_party/log.c/src/ ./lib/driver/src/ ./lib/cgi/src/ ./lib/propeller/src/

EXTRA_CFLAGS=-DLWIP_HTTPD_CGI=1 -DLWIP_HTTPD_SSI=1 -I./fsdata -DLOG_USE_COLOR -DLWIP_HTTPD_SSI_INCLUDE_TAG=0

#Enable debugging
#EXTRA_CFLAGS+=-DLWIP_DEBUG=1 -DHTTPD_DEBUG=LWIP_DBG_ON

EXTRA_COMPONENTS=extras/mbedtls extras/httpd
EXTRA_COMPONENTS += extras/jsmn
EXTRA_COMPONENTS += extras/pwm

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
	docker run --rm -it -v $(PWD)/../tests:/home/tests marcotti/remote-lab-debian-host python3 -m unittest discover -v -s /home/tests

lint:
	$(LINT_TOOL) $(LINT_FLAGS) *

monitor:
	$(FILTEROUTPUT) --port $(ESPPORT) --baud 115200 --elf $(PROGRAM_OUT)

flash_after_build:
	$(if will_flash, $(call will_flash, "flash"))
	$(ESPTOOL) -p $(ESPPORT) --baud $(ESPBAUD) write_flash $(ESPTOOL_ARGS) \
		$(RBOOT_ARGS) 0x2000 $(CI_PROJECT_DIR)/*.bin $(SPIFFS_ESPTOOL_ARGS)
	$(if did_flash, $(call did_flash, "flash"))
