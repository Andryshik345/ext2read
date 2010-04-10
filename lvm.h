/**
 * Ext2read
 * File: lvm.h
 **/
/**
 * Copyright (C) 2006 2010 by Manish Regmi   (regmi dot manish at gmail.com)
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

#ifndef __LVM_H
#define __LVM_H

#include <stdint.h>
#include <QString>

#include "ext2read.h"

#define LVM_SIGLEN	8
#define LVM_MAGIC_LEN	8
#define UUID_LEN	32

/* Structure to hold Physical Volumes (PV) label*/
typedef struct pv_label_header {
        char        pv_name[LVM_SIGLEN];   // Physical volume signature
        uint64_t    pv_sector_xl;          // sector number of this label
        uint32_t    pv_crc;                // CRC value
        uint32_t    pv_offset_xl;          // Offset from start of struct to contents
        char        pv_vermagic[LVM_MAGIC_LEN]; // Physical Volume version "LVM2 001"
        char        pv_uuid[UUID_LEN];
        uint64_t    pv_unknown1[5];             // documentation lacks for lvm
        uint64_t    pv_labeloffset;             // location of the label
} __attribute__ ((__packed__)) PV_LABEL_HEADER;

typedef struct pv_label {
    uint32_t        pv_magic;
    char            pv_sig[4];          // signature
    uint64_t        unknown1[2];
    uint64_t        pv_offset_low;
    uint64_t        unknown2;
    uint64_t        pv_offset_high;
    uint64_t        pv_length;
} __attribute__ ((__packed__)) PV_LABEL;

class VolumeGroup;

class LVM {
private:
    FileHandle pv_handle;
    lloff_t pv_offset;
    QString pv_metadata;
    Ext2Read *ext2read;
public:
    LVM(FileHandle handle, lloff_t offset, Ext2Read *rd);
    ~LVM();

    int scan_pv();
    int parse_metadata();
    VolumeGroup *find_volgroup();
};


class PhysicalVolume {
    char uuid[UUID_LEN];
    lloff_t dev_size;
    uint32_t pe_start, pe_count;
};

class lv_segment {
    uint32_t start_extent;
    uint32_t extent_count;
    std::list <PhysicalVolume *> pvolumes;
};

class LogicalVolume {
    char uuid[UUID_LEN];
    int segment_count;
    VolumeGroup *this_group;
};

class VolumeGroup {
private:
    QString volgroup;
    int extent_size;
    int seqno;
    int max_lv, max_pv;
    std::list <PhysicalVolume *> pvolumes;
    std::list <LogicalVolume *> lvolumes;

public:
    VolumeGroup();
    ~VolumeGroup();
};

#ifdef __cplusplus
extern "C"{
#endif
int scan_lvm(FileHandle handle, lloff_t offset);

#ifdef __cplusplus
}
#endif

#endif // __LVM_H
