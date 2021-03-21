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
 /** \file ssi_utils.h */
#ifndef EXAMPLES_PROJECT_LIB_CGI_INC_SSI_UTILS_H_
#define EXAMPLES_PROJECT_LIB_CGI_INC_SSI_UTILS_H_

#include <stddef.h>
#include <stdint.h>

#include <retval.h>

/**
 * \brief   Replaces SSI tag with the content of global string ssi_string.
 *
 *  Every time that the processing library finds a defined SSI tag in a
 *  file that will be used as a response, it will call ssi_handler, with
 *  it's specified parameters.
 *
 * \note    THIS IS AN EXTERNAL FUNCTION, FROM HTTPD.
 * \param   iIndex: Specifies which number of SSI tag is being handled.
 * \param   *pcInsert: Position in memory where the string to be inserted
 *          should begin.
 * \param   iInsertLen: Max length (chars) of the string that will be inserted.
 */
int32_t ssi_handler(int32_t iIndex, char *pcInsert, int32_t iInsertLen);


/**
 * \brief   Loads an string into the global ssi_string.
 *
 *  Should be called before issuing any HTTP response with a file that
 *  contains a SSI tag, that we expect to replace.
 *
 * \param   *str: A pointer to the string we expect to use as replacement.
 * \param   str_len: The length of the string that begins at *str.
 */
retval_t load_ssi_data(const char *str, size_t str_len);
#endif  /* EXAMPLES_PROJECT_LIB_CGI_INC_SSI_UTILS_H_ */
