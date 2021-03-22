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
/** \file logger_cgi.h */
#ifndef EXAMPLES_PROJECT_CALLBACKS_INC_LOGGER_CGI_H_
#define EXAMPLES_PROJECT_CALLBACKS_INC_LOGGER_CGI_H_

/**
 * \brief   An HTTP handler to adjust the system logging level.
 * \remark  Endpoint is http://$HOST_IP/logger/level
 * \param   iIndex: Unused.
 * \param   iNumParams: Quantity of variables in the query.
 * \param   *pcParam[]: Pointer to first char of first string in query-names array.
 * \param   *pcValue[]: Pointer to first char of first string in query-values array.
 * \return  A file name, which will contain an HTTP return code.
 */
const char *logger_level_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

#endif /* EXAMPLES_PROJECT_CALLBACKS_INC_LOGGER_CGI_H_ */
