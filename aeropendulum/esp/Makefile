PROGRAM=http_server
PROGRAM_INC_DIR=./include ./lib/json/inc/ ./tests/inc/ ./lib/log.c/src/
PROGRAM_SRC_DIR=. ./lib/json/src/ ./tests/src/ ./tasks/ ./lib/log.c/src/

EXTRA_CFLAGS=-DLWIP_HTTPD_CGI=1 -DLWIP_HTTPD_SSI=1 -I./fsdata -DLOG_USE_COLOR

#Enable debugging
#EXTRA_CFLAGS+=-DLWIP_DEBUG=1 -DHTTPD_DEBUG=LWIP_DBG_ON

EXTRA_COMPONENTS=extras/mbedtls extras/httpd
EXTRA_COMPONENTS += extras/jsmn

include ../../common.mk

flash_after_build:
	$(if will_flash, $(call will_flash, "flash"))
	$(ESPTOOL) -p $(ESPPORT) --baud $(ESPBAUD) write_flash $(ESPTOOL_ARGS) \
		$(RBOOT_ARGS) 0x2000 $(CI_PROJECT_DIR)/*.bin $(SPIFFS_ESPTOOL_ARGS)
	$(if did_flash, $(call did_flash, "flash"))
