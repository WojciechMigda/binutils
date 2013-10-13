/* BFD back-end for TMS320C6X coff binaries.
   Copyright 1996, 1997, 1998, 1999, 2000, 2002, 2003, 2005, 2007,
   2008, 2012, 2013  Free Software Foundation, Inc.

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
#include "bfdlink.h"
#include "coff/tic6x.h"
#include "coff/internal.h"
#include "libcoff.h"

#undef  F_LSYMS
#define	F_LSYMS		F_LSYMS_TICOFF

static reloc_howto_type *
coff_tic6x_rtype_to_howto(
    bfd *,
    asection *,
    struct internal_reloc *,
    struct coff_link_hash_entry *,
    struct internal_syment *,
    bfd_vma *);

static void
tic6x_reloc_processing(
    arelent *,
    struct internal_reloc *,
    asymbol **,
    bfd *,
    asection *);

/* Replace the stock _bfd_coff_is_local_label_name to recognize TI COFF local
 * labels.  */
static bfd_boolean
ticoff_bfd_is_local_label_name(
    bfd *abfd ATTRIBUTE_UNUSED,
    const char *name)
{
    if (TICOFF_LOCAL_LABEL_P(name))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#define coff_bfd_is_local_label_name ticoff_bfd_is_local_label_name

#define RELOC_PROCESSING(RELENT,RELOC,SYMS,ABFD,SECT)\
 tic6x_reloc_processing (RELENT,RELOC,SYMS,ABFD,SECT)

/* Customize coffcode.h; the default coff_ functions are set up to use
 * COFF2; coff_bad_format_hook uses BADMAG, so set that for COFF2.
 * The COFF1 and COFF0 vectors use custom _bad_format_hook procs
 * instead of setting BADMAG.  */
#define BADMAG(x) COFF2_BADMAG(x)

#undef  coff_rtype_to_howto
#define coff_rtype_to_howto	coff_tic6x_rtype_to_howto

#ifndef bfd_pe_print_pdata
#define bfd_pe_print_pdata	NULL
#endif

#include "coffcode.h"

static bfd_reloc_status_type
tic6x_relocation(
    bfd *abfd ATTRIBUTE_UNUSED,
    arelent *reloc_entry,
    asymbol *symbol ATTRIBUTE_UNUSED,
    void * data ATTRIBUTE_UNUSED,
    asection *input_section,
    bfd *output_bfd,
    char **error_message ATTRIBUTE_UNUSED)
{
    if (output_bfd != (bfd *)NULL)
    {
        /* This is a partial relocation, and we want to apply the
 	     * relocation to the reloc entry rather than the raw data.
 	     * Modify the reloc inplace to reflect what we now know.  */
        reloc_entry->address += input_section->output_offset;
        return bfd_reloc_ok;
    }
    else
    {
        return bfd_reloc_continue;
    }
}

reloc_howto_type tic6x_howto_table[] =
{
    HOWTO(R_RELBYTE,     0,  2,  8, FALSE, 0, complain_overflow_signed,   tic6x_relocation, "R_RELBYTE",   TRUE, 0x0000ffff, 0x000000ff, FALSE),
    HOWTO(R_RELWORD,	 0,  2, 16, FALSE, 0, complain_overflow_signed,   tic6x_relocation, "R_RELWORD",   TRUE, 0x0000ffff, 0x0000ffff, FALSE),
    HOWTO(R_RELLONG,	 0,  2, 32, FALSE, 0, complain_overflow_dont,     tic6x_relocation, "R_RELLONG",   TRUE, 0xffffffff, 0xffffffff, FALSE),
    /* taken from http://mailman.tu-chemnitz.de/pipermail/gcc-c6x/attachments/20090105/d15f1447/attachment.pl */
    /* Elf: R_C6000_SBR_U15_B */
    HOWTO(R_C60BASE,     0,  2, 15, FALSE,  8, complain_overflow_unsigned, tic6x_relocation, "R_C60BASE",   TRUE, 0x007fff00, 0x007fff00, FALSE),
    /* Elf: R_C6000_ABS_L16 */
//    HOWTO(R_C60DIR15,    0,  2, 16, FALSE,  7, complain_overflow_dont,     tic6x_relocation, "C60DIR15",  TRUE, 0x003fffc0, 0x003fffc0, FALSE), // ???
    /* Elf: R_C6000_PCR_S21 */
    HOWTO(R_C60PCR21,    2,  2, 21, TRUE,   7, complain_overflow_signed,   tic6x_relocation, "R_C60PCR21",  TRUE, 0x0fffff80, 0x0fffff80, FALSE),
//    HOWTO( R_C60PCR10,   2,  2, 10, TRUE,  13, complain_overflow_signed,   tic6x_relocation, "C60PCR10",  TRUE, 0x007fe000, 0x007fe000, FALSE),
    /* Elf: R_C6000_ABS_L16 */
    HOWTO(R_C60LO16,     0,  2, 16, FALSE,  7, complain_overflow_dont,     tic6x_relocation, "R_C60LO16",   TRUE, 0x007fff80, 0x007fff80, FALSE),
    /* Elf: R_C6000_ABS_H16 */
    HOWTO(R_C60HI16,    16,  2, 16, FALSE,  7, complain_overflow_dont,     tic6x_relocation, "R_C60HI16",   TRUE, 0x007fff80, 0x007fff80, FALSE),
//    HOWTO( R_C60SECT,    0,  2, 16, FALSE,  7, complain_overflow_dont,     tic6x_relocation, "C60SECT",   TRUE, 0x003fffc0, 0x003fffc0, FALSE), // ???
    /* Elf: R_C6000_ABS_S16 */
    HOWTO(R_C60S16,      0,  2, 16, FALSE,  7, complain_overflow_signed,   tic6x_relocation, "R_C60S16",    TRUE, 0x007fff80, 0x007fff80, FALSE),
//    HOWTO( R_C60PCR7,    2,  2,  7, TRUE,  16, complain_overflow_signed,   tic6x_relocation, "C60PCR7",   TRUE, 0x007f0000, 0x007f0000, FALSE),
//    HOWTO( R_C60PCR12,   2,  2, 12, TRUE,  16, complain_overflow_signed,   tic6x_relocation, "C60PCR12",  TRUE, 0x0fff0000, 0x0fff0000, FALSE),
    HOWTO(R_TI_ADD,    0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_ADD",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_SR,     0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_SR",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_AND,    0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_AND",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_USTFLD, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_USTFLD",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_PUSH,   0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_PUSH",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_PUSHUK, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_PUSHUK",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(R_TI_XSTFLD, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RE_XSTFLD",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(0x5000, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RM_RANGE",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(0x5001, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RM_OBJ",  TRUE, 0x0, 0x0, FALSE),
    HOWTO(0x5002, 0,  0,  0, FALSE,  0, complain_overflow_dont,     tic6x_relocation, "RM_DWARF_LENGTH",  TRUE, 0x0, 0x0, FALSE),
};
#define HOWTO_SIZE (sizeof(tic6x_howto_table) / sizeof(tic6x_howto_table[0]))

#undef coff_bfd_reloc_type_lookup
#define coff_bfd_reloc_type_lookup tic6x_coff_reloc_type_lookup
#undef coff_bfd_reloc_name_lookup
#define coff_bfd_reloc_name_lookup tic6x_coff_reloc_name_lookup

/* For the case statement use the code values used tc_gen_reloc (defined in
 * bfd/reloc.c) to map to the howto table entries.  */

static reloc_howto_type *
tic6x_coff_reloc_type_lookup(
    bfd *abfd ATTRIBUTE_UNUSED,
    bfd_reloc_code_real_type code)
{
    unsigned int type;
    unsigned int i;

    switch (code)
    {
        case BFD_RELOC_32:		type = R_RELLONG; break;
//        case BFD_RELOC_24:		type = R_REL24; break;
        case BFD_RELOC_16:		type = R_RELWORD; break;
//        case BFD_RELOC_24_PCREL:	type = R_PCR24; break;
//        case BFD_RELOC_16_PCREL:	type = R_PCRWORD; break;
//        case BFD_RELOC_HI16:	type = R_PARTMS8; break;
//        case BFD_RELOC_LO16:	type = R_PARTLS16; break;
        default:
            return NULL;
    }

    for (i = 0; i < HOWTO_SIZE; i++)
    {
        if (tic6x_howto_table[i].type == type)
        {
	        return tic6x_howto_table + i;
	    }
    }

    return NULL;
}

static reloc_howto_type *
tic6x_coff_reloc_name_lookup(
    bfd *abfd ATTRIBUTE_UNUSED,
    const char *r_name)
{
    unsigned int i;

    for (i = 0; i < HOWTO_SIZE; i++)
    {
        if (tic6x_howto_table[i].name != NULL
	        && strcasecmp (tic6x_howto_table[i].name, r_name) == 0)
	    {
            return &tic6x_howto_table[i];
        }
    }

    return NULL;
}

/* Code to turn a r_type into a howto ptr, uses the above howto table.
 * Called after some initial checking by the tic6x_rtype_to_howto fn
 * below.  */
static void
tic6x_lookup_howto(
    arelent *internal,
    struct internal_reloc *dst)
{
    unsigned int i;
    int bank = (dst->r_symndx == -1) ? HOWTO_BANK : 0;

#if 0
    if (dst->r_symndx == 65535)
    {
        (*_bfd_error_handler)(_("tic6x_lookup_howto: Internal reloc 65535 for type 0x%x vaddr 0x%x"),
            (unsigned int)dst->r_type, dst->r_vaddr);
    }
#endif

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

static reloc_howto_type *
coff_tic6x_rtype_to_howto(
    bfd *abfd ATTRIBUTE_UNUSED,
    asection *sec,
    struct internal_reloc *rel,
    struct coff_link_hash_entry *h ATTRIBUTE_UNUSED,
    struct internal_syment *sym ATTRIBUTE_UNUSED,
    bfd_vma *addendp)
{
    arelent genrel;

    if (rel->r_symndx == -1 && addendp != NULL)
    {
        /* This is a TI "internal relocation", which means that the relocation
         * amount is the amount by which the current section is being relocated
         * in the output section.  */
        *addendp = (sec->output_section->vma + sec->output_offset) - sec->vma;
    }

    tic6x_lookup_howto(&genrel, rel);

    return genrel.howto;
}


static void
tic6x_reloc_processing(
    arelent *relent,
    struct internal_reloc *reloc,
    asymbol **symbols,
    bfd *abfd,
    asection *section)
{
    asymbol *ptr;

    relent->address = reloc->r_vaddr;

#if 0
    if (reloc->r_type & 0x4000)
    {
        /* relocation expression */
        (*_bfd_error_handler)
            (_("tic6x_reloc_processing: Relocation expression:\n  vaddr: 0x%08X symndx: 0x%04X type: 0x%04X"),
                reloc->r_vaddr, reloc->r_symndx, reloc->r_type);
    }
#endif

    if (reloc->r_symndx != -1)
    {
        if (reloc->r_symndx < 0 || reloc->r_symndx >= obj_conv_table_size (abfd))
        {
            (*_bfd_error_handler)
                (_("%s: warning: illegal symbol index %lX in relocs"),
                bfd_get_filename (abfd), reloc->r_symndx);
            relent->sym_ptr_ptr = bfd_abs_section_ptr->symbol_ptr_ptr;
            ptr = NULL;
        }
        else
        {
            relent->sym_ptr_ptr = (symbols + obj_convert (abfd)[reloc->r_symndx]);
            ptr = *(relent->sym_ptr_ptr);
        }
    }
    else
    {
        relent->sym_ptr_ptr = section->symbol_ptr_ptr;
        ptr = *(relent->sym_ptr_ptr);
    }

    /* The symbols definitions that we have read in have been relocated
     * as if their sections started at 0.  But the offsets refering to
     * the symbols in the raw data have not been modified, so we have to
     * have a negative addend to compensate.
     *
     * Note that symbols which used to be common must be left alone.  */

    /* Calculate any reloc addend by looking at the symbol.  */
    CALC_ADDEND(abfd, ptr, *reloc, relent);

    relent->address -= section->vma;
    /* !!     relent->section = (asection *) NULL;  */

    /* Fill in the relent->howto field from reloc->r_type.  */
    tic6x_lookup_howto(relent, reloc);
}


/* TI COFF v0, DOS tools (little-endian headers).  */
CREATE_LITTLE_COFF_TARGET_VEC(bfd_coff0_tic6x_coff_le_vec, "bfd_coff0_tic6x_coff_le_vec", HAS_LOAD_PAGE, 0, '_', NULL, &ticoff0_swap_table);

/* TI COFF v0, SPARC tools (big-endian headers).  */
CREATE_BIGHDR_COFF_TARGET_VEC(bfd_coff0_tic6x_coff_be_vec, "bfd_coff0_tic6x_coff_be_vec", HAS_LOAD_PAGE, 0, '_', &bfd_coff0_tic6x_coff_le_vec, &ticoff0_swap_table);

/* TI COFF v1, DOS tools (little-endian headers).  */
CREATE_LITTLE_COFF_TARGET_VEC(bfd_coff1_tic6x_coff_le_vec, "bfd_coff1_tic6x_coff_le_vec", HAS_LOAD_PAGE, 0, '_', &bfd_coff0_tic6x_coff_be_vec, &ticoff1_swap_table);

/* TI COFF v1, SPARC tools (big-endian headers).  */
CREATE_BIGHDR_COFF_TARGET_VEC(bfd_coff1_tic6x_coff_be_vec, "bfd_coff1_tic6x_coff_be_vec", HAS_LOAD_PAGE, 0, '_', &bfd_coff1_tic6x_coff_le_vec, &ticoff1_swap_table);

/* TI COFF v2, TI DOS tools output (little-endian headers).  */
CREATE_LITTLE_COFF_TARGET_VEC(bfd_coff2_tic6x_coff_le_vec, "bfd_coff2_tic6x_coff_le_vec", HAS_LOAD_PAGE, 0, '_', &bfd_coff1_tic6x_coff_be_vec, COFF_SWAP_TABLE);

/* TI COFF v2, TI SPARC tools output (big-endian headers).  */
CREATE_BIGHDR_COFF_TARGET_VEC(bfd_coff2_tic6x_coff_be_vec, "bfd_coff2_tic6x_coff_be_vec", HAS_LOAD_PAGE, 0, '_', &bfd_coff2_tic6x_coff_le_vec, COFF_SWAP_TABLE);

