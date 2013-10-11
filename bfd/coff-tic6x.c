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

#include "coff/internal.h"
#include "libcoff.h"

#define TI_TARGET_ID 0x99
#define TICOFF_TARGET_ARCH bfd_arch_tic6x
#define OCTETS_PER_BYTE_POWER   2

#include "coff/ti.h"

#define BADMAG(x) COFF2_BADMAG(x)
#define RELOC_PROCESSING(relent,reloc,symbols,abfd,section) \
 tic6x_reloc_processing(relent, reloc, symbols, abfd, section)

static void
tic6x_reloc_processing(
    arelent * const relent,
    struct internal_reloc const * const reloc,
    asymbol ** const symbols,
    bfd * const abfd,
    asection const * const section);

#ifndef bfd_pe_print_pdata
#define bfd_pe_print_pdata  NULL
#endif

#include "coffcode.h"

/* Add to howto to get absolute/sect-relative version.  */
#define HOWTO_BANK      6
static const reloc_howto_type tic6x_howto_table[] =
{
//    HOWTO(R_RELWORD,     0,  2, 16, FALSE, 0, complain_overflow_signed,   tic4x_relocation, "RELWORD",   TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_REL24,   0,  2, 24, FALSE, 0, complain_overflow_bitfield, tic4x_relocation, "REL24",     TRUE, 0x00ffffff, 0x00ffffff, FALSE),
//    HOWTO(R_RELLONG,     0,  2, 32, FALSE, 0, complain_overflow_dont,     tic4x_relocation, "RELLONG",   TRUE, 0xffffffff, 0xffffffff, FALSE),
//    HOWTO(R_PCRWORD,     0,  2, 16, TRUE,  0, complain_overflow_signed,   tic4x_relocation, "PCRWORD",   TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_PCR24,   0,  2, 24, TRUE,  0, complain_overflow_signed,   tic4x_relocation, "PCR24",     TRUE, 0x00ffffff, 0x00ffffff, FALSE),
//    HOWTO(R_PARTLS16,    0,  2, 16, FALSE, 0, complain_overflow_dont,     tic4x_relocation, "PARTLS16",  TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_PARTMS8,    16,  2, 16, FALSE, 0, complain_overflow_dont,     tic4x_relocation, "PARTMS8",   TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_RELWORD,     0,  2, 16, FALSE, 0, complain_overflow_signed,   tic4x_relocation, "ARELWORD",  TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_REL24,   0,  2, 24, FALSE, 0, complain_overflow_signed,   tic4x_relocation, "AREL24",    TRUE, 0x00ffffff, 0x00ffffff, FALSE),
//    HOWTO(R_RELLONG,     0,  2, 32, FALSE, 0, complain_overflow_signed,   tic4x_relocation, "ARELLONG",  TRUE, 0xffffffff, 0xffffffff, FALSE),
//    HOWTO(R_PCRWORD,     0,  2, 16, TRUE,  0, complain_overflow_signed,   tic4x_relocation, "APCRWORD",  TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_PCR24,   0,  2, 24, TRUE,  0, complain_overflow_signed,   tic4x_relocation, "APCR24",    TRUE, 0x00ffffff, 0x00ffffff, FALSE),
//    HOWTO(R_PARTLS16,    0,  2, 16, FALSE, 0, complain_overflow_dont,     tic4x_relocation, "APARTLS16", TRUE, 0x0000ffff, 0x0000ffff, FALSE),
//    HOWTO(R_PARTMS8,    16,  2, 16, FALSE, 0, complain_overflow_dont,     tic4x_relocation, "APARTMS8",  TRUE, 0x0000ffff, 0x0000ffff, FALSE),
};
#define HOWTO_SIZE (sizeof(tic6x_howto_table) / sizeof(tic6x_howto_table[0]))

static void
tic6x_lookup_howto(
    arelent * const internal,
    struct internal_reloc const * const dst)
{
    unsigned int i;
    int bank = (dst->r_symndx == -1) ? HOWTO_BANK : 0;

    for (i = 0; i < HOWTO_SIZE; i++)
    {
        if (tic6x_howto_table[i].type == dst->r_type)
        {
            internal->howto = tic6x_howto_table + i + bank;

            return;
        }
    }

    (*_bfd_error_handler)(_("Unrecognized reloc type 0x%x"),
        (unsigned int) dst->r_type);
    abort();
}

static void
tic6x_reloc_processing(
    arelent * const relent,
    struct internal_reloc const * const reloc,
    asymbol ** const symbols,
    bfd * const abfd,
    asection const * const section)
{
    asymbol *ptr;

    relent->address = reloc->r_vaddr;

    if (reloc->r_symndx != -1)
    {
        if (reloc->r_symndx < 0 || reloc->r_symndx >= obj_conv_table_size (abfd))
        {
            (*_bfd_error_handler)
                (_("%s: warning: illegal symbol index %ld in relocs"),
                    bfd_get_filename (abfd), reloc->r_symndx);
            relent->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
            ptr = NULL;
        }
        else
        {
            relent->sym_ptr_ptr = (symbols
                + obj_convert (abfd)[reloc->r_symndx]);
            ptr = *(relent->sym_ptr_ptr);
        }
    }
    else
    {
        relent->sym_ptr_ptr = section->symbol_ptr_ptr;
        ptr = *(relent->sym_ptr_ptr);
    }

    /* The symbols definitions that we have read in have been relocated
     as if their sections started at 0.  But the offsets refering to
     the symbols in the raw data have not been modified, so we have to
     have a negative addend to compensate.

     Note that symbols which used to be common must be left alone.  */

    /* Calculate any reloc addend by looking at the symbol.  */
    CALC_ADDEND(abfd, ptr, *reloc, relent);

    relent->address -= section->vma;
    /* !!     relent->section = (asection *) NULL;  */

    /* Fill in the relent->howto field from reloc->r_type.  */
    tic6x_lookup_howto(relent, reloc);
}

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
