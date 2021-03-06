/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifndef _NUCLEO_BSD_SOCKET_H_
#define _NUCLEO_BSD_SOCKET_H_

#include <unint.h>
#include <bsd/inet.h>
#include <bsd/struct_hostent.h>
#include <platforms/default/bsd/sockdef.h>
#include <platforms/default/bsd/sockdecl.h>

# define htons _htons
# define htonl _htonl

#endif // _NUCLEO_BSD_SOCKET_H_
