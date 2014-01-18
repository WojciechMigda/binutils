/* TI C6X assembler.
 *  Copyright 2010-2013 Free Software Foundation, Inc.
 *  Contributed by Wojciech Migda <wojtek.golf@interia.pl>
 *
 *  This file is part of GAS, the GNU Assembler.
 *
 *  GAS is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  GAS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GAS; see the file COPYING.  If not, write to the Free
 *  Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
 *  02110-1301, USA.  */

#include <string.h>

char *
s_tic6x_coff_c6xabi_attribute(char * const input_line_p)
{
  char *        work_p = input_line_p;

  work_p = strchrnul(work_p, 0x0a) + 1;

  return work_p;
}
