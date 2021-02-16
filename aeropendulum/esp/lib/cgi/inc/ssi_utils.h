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
#ifndef EXAMPLES_PROJECT_LIB_CGI_INC_SSI_UTILS_H_
#define EXAMPLES_PROJECT_LIB_CGI_INC_SSI_UTILS_H_

#include <stddef.h>
#include <stdint.h>

typedef enum ssiUtilsRvType {
    SSI_OK,
    STRING_TOO_LARGE,
} ssiUtilsRvType;


int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen);

ssiUtilsRvType load_ssi_data(const char *str, size_t str_len);
#endif /* EXAMPLES_PROJECT_LIB_GCI_INC_SSI_UTILS_H_ */
