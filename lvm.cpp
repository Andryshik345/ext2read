/**
 * Ext2read
 * File: lvm.c
 **/
/**
 * Copyright (C) 2006 by Manish Regmi   (regmi dot manish at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/

#include <stdlib.h>

#include "ext2read.h"
#include "lvm.h"


LVM::LVM(FileHandle handle, lloff_t offset)
{
//    label = new PV_LABEL_HEADER;

    pv_handle = handle;
    pv_offset = offset;
}

LVM::~LVM()
{
    //delete label;
}

int LVM::scan_pv()
{
    int ret;
    char *config;
    PV_LABEL_HEADER *header;
    PV_LABEL *label;
    char buffer[512];

    ret = read_disk(pv_handle, buffer, pv_offset + 1, 1, 512);

    header = (PV_LABEL_HEADER *) &buffer[0];
    LOG("PV Metadata: %s %UUID=%s offset %d \n",header->pv_name, header->pv_uuid, header->pv_offset_xl);

    read_disk(pv_handle, buffer, (header->pv_labeloffset/512) + 4, 1, 512);
    label = (PV_LABEL *) &buffer[0];

    read_disk(pv_handle, buffer, label->pv_offset_low + label->pv_offset_high, 1, 512);
    LOG("\n%s", buffer);

    return 0;
}

