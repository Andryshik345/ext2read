/**
 * Ext2read
 * File: print.c
 **/
/**
 * Copyright (C) 2005 by Manish Regmi   (regmi dot manish at gmail.com)
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

#include <stdio.h>

#include "platform.h"
#include "ext2read.h"
#include "parttypes.h"

extern struct sys sys;

void print_ext2()
{
	int i, j;
	char ch[2], num[3];
	struct disk *dsk;
	struct partition *part;

	for(i = 0; i < sys.ndisks; i++)
	{
		dsk = &sys.dsk[i];
		ch[0] = (i + 'a');
		ch[1] = '\0';
		printf("Disk %d: /dev/hd%c \n", i, ch[0]);
		for(j = 0; j < dsk->totalpart; j++)
		{
			part = &dsk->part[j];
			strcpy(part->linux_name, "/dev/hd");
			strcat(part->linux_name, ch);
			sprintf(num, "%d", j + 1);
			strcat(part->linux_name, num);
			printf("  Partition %d: %s  type: %s\n", \
					j, part->linux_name, part_type(part->systemId));
		}
	}
}

void get_choice(char *partstr, int *disk, int *part)
{
	char num[10];

	if(!strcmp(partstr, "exit"))
		exit(0);

	*disk = (int)(partstr[7] - 'a');
	strcpy(num, &partstr[8]);
	*part = atoi(num) - 1;
}

struct partition *ask_for_choice()
{
	char partstr[20];
	int disk_idx, part_idx, max;
	struct partition *p;
again:
	print_ext2();
	printf("Please enter your choice in the form (/dev/hda5) or exit to end\n");

	memset(partstr, 0, 20);
	scanf("%s", partstr);

	get_choice(partstr, &disk_idx, &part_idx);
	printf("Choices are %d %d\n", disk_idx, part_idx);
	
	if((disk_idx < 0) || (disk_idx >= sys.ndisks))
	{
		printf("invalid choice. please choose again\n");
		goto again;
	}

	max = sys.dsk[disk_idx].totalpart;
	if((part_idx < 0) || (part_idx > max))
	{
		printf("invalid choice. please choose again\n");
		goto again;
	}

	p = &sys.dsk[disk_idx].part[part_idx];
	if(p->systemId != EXT2)
	{
		printf("Not an Ext2 Partition. \n"
			"you can only select Ext2 Partitions");
		goto again;
	}
	
	sys.current_disk = &sys.dsk[disk_idx];
	return p;
}
