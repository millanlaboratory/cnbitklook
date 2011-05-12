/*
    Copyright (C) 2009 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libtransport library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libtransport/tr_names.h>
#include <libtransport/tr_types.h>
#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_NAME "[resolver.c]"

int main(void) { 
	char address[tr_getaddrlen()];
	tr_resolve("menace", address);
	printf("%s IPv4 is %s\n", EXAMPLE_NAME, address);
	return 0;
}
