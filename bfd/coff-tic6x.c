/* BFD back-end for TMS320C6x coff binaries.
   Copyright 2013
   Free Software Foundation, Inc.
   Contributed by Wojciech Migda

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "libcoff.h"

/* customize ?? */
#include "coffcode.h"

const bfd_target bfd_coff2_tic6x_coff_le_vec =
{
    "coff2-c6x",    /* name */
    bfd_target_coff_flavour,
    BFD_ENDIAN_BIG,		/* data byte order is big */
    BFD_ENDIAN_LITTLE,		/* header byte order is little */

    (HAS_RELOC | EXEC_P |		/* object flags */
      HAS_LINENO | HAS_DEBUG |
      HAS_SYMS | HAS_LOCALS | WP_TEXT),

    (SEC_HAS_CONTENTS | SEC_ALLOC | SEC_LOAD | SEC_RELOC), /* section flags */
    '_',				/* leading symbol underscore */
    '/',				/* ar_pad_char */
    15,				/* ar_max_namelen */
    0,				/* match priority.  */
    bfd_getb64, bfd_getb_signed_64, bfd_putb64,
    bfd_getb32, bfd_getb_signed_32, bfd_putb32,
    bfd_getb16, bfd_getb_signed_16, bfd_putb16,	/* data */
    bfd_getl64, bfd_getl_signed_64, bfd_putl64,
    bfd_getl32, bfd_getl_signed_32, bfd_putl32,
    bfd_getl16, bfd_getl_signed_16, bfd_putl16,	/* hdrs */

    {
        _bfd_dummy_target, coff_object_p,	/* bfd_check_format */
        bfd_generic_archive_p, _bfd_dummy_target
    },
    {
        bfd_false, coff_mkobject, _bfd_generic_mkarchive,	/* bfd_set_format */
        bfd_false
    },
    {
        bfd_false, coff_write_object_contents,	/* bfd_write_contents */
        _bfd_write_archive_contents, bfd_false
    },

    BFD_JUMP_TABLE_GENERIC (coff),
    BFD_JUMP_TABLE_COPY (coff),
    BFD_JUMP_TABLE_CORE (_bfd_nocore),
    BFD_JUMP_TABLE_ARCHIVE (_bfd_archive_coff),
    BFD_JUMP_TABLE_SYMBOLS (coff),
    BFD_JUMP_TABLE_RELOCS (coff),
    BFD_JUMP_TABLE_WRITE (coff),
    BFD_JUMP_TABLE_LINK (coff),
    BFD_JUMP_TABLE_DYNAMIC (_bfd_nodynamic),

    NULL,

    COFF_SWAP_TABLE
};
