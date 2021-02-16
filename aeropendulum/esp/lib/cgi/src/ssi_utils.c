/*
 * Copyright 2021 Marco Miretti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <string.h>

#include <httpd/httpd.h>

#include <ssi_utils.h>
#include <log.h>


static char ssi_string[LWIP_HTTPD_MAX_TAG_INSERT_LEN];

int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen) {
    snprintf(pcInsert, iInsertLen, ssi_string);
    log_trace("String replaced in ssi file");
    return (strlen(pcInsert));
}

ssiUtilsRvType load_ssi_data(const char *str, size_t str_len) {
    if (str_len > LWIP_HTTPD_MAX_TAG_INSERT_LEN) {
        log_error(
            "SSI too large! Max len is: %d, given string len is %d",
            LWIP_HTTPD_MAX_TAG_INSERT_LEN,
            str_len
        );
        return STRING_TOO_LARGE;
    }
    log_trace("String written to global variable");
    snprintf(ssi_string, str_len, str);
    return SSI_OK;
}
