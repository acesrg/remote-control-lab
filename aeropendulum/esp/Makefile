PROGRAM=http_server

EXTRA_CFLAGS=-DLWIP_HTTPD_CGI=1 -DLWIP_HTTPD_SSI=1 -I./fsdata

#Enable debugging
#EXTRA_CFLAGS+=-DLWIP_DEBUG=1 -DHTTPD_DEBUG=LWIP_DBG_ON

EXTRA_COMPONENTS=extras/mbedtls extras/httpd

include ../../common.mk

html:
	@echo "Generating fsdata.."
	cd fsdata && ./makefsdata

flash_after_build:
	$(if will_flash, $(call will_flash, "flash"))
	$(ESPTOOL) -p $(ESPPORT) --baud $(ESPBAUD) write_flash $(ESPTOOL_ARGS) \
		$(RBOOT_ARGS) 0x2000 $(CI_PROJECT_DIR)/*.bin $(SPIFFS_ESPTOOL_ARGS)
	$(if did_flash, $(call did_flash, "flash"))
