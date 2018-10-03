
#include "fs.h"
#include "disk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024


struct fs_superblock {
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};

struct fs_inode {
	int isvalid;
	int size;
	int direct[POINTERS_PER_INODE];
	int indirect;
};

union fs_block {
	struct fs_superblock super;
	struct fs_inode inode[INODES_PER_BLOCK];
	int pointers[POINTERS_PER_BLOCK];
	char data[DISK_BLOCK_SIZE];
};



int fs_format()
{
	
	
	return 0;
}
/*
void fs_debug()
{
//fdkslna.knasl
//dakhadsfuhdafk
	union fs_block block,iblock;
	int i,j,k,,l,ninodeb;
	disk_read(0,block.data);
	ninodeb=block.super.ninodeblocks;


	printf("superblock:\n");
	printf("    %d blocks\n",block.super.nblocks);
	printf("    %d inode blocks\n",block.super.ninodeblocks);
	printf("    %d inodes\n",block.super.ninodes);


	for(k=0;k<ninodeb;k++)
	{
		disk_read(k,block.data);
		for(i=1;i<INODES_PER_BLOCK;i++)
		{
			//disk_read(i,block.data);
			if(block.inode[i].isvalid == 0)
			{
				printf("inode %d\n",i );
				printf(" size : %d\n",block.inode[i].size);
				printf("direct block" );
				for( j = 0 ; j < POINTERS_PER_INODE ; j++ )
				{
					printf("%d ",block.inode[i].direct[j]);
				}
				printf("\n");

				printf("indirect block %d\n",block.inode[i].indirect);
				disk_read(block.inode[i].indirect,iblock.data);
				printf("indirect data blocks: ")
				for(l=0;l<POINTERS_PER_BLOCK;l++)
				{
					printf("%d ",iblock.pointers[l])
				}
			}
		}
	}

*/









void fs_debug()
{
	
	union fs_block block,iblock;
	int i,j,k,l,ninodeb;
	disk_read(0,block.data);
	ninodeb=block.super.ninodeblocks;


	printf("superblock:\n");
	printf("    %d blocks\n",block.super.nblocks);
	printf("    %d inode blocks\n",block.super.ninodeblocks);
	printf("    %d inodes\n",block.super.ninodes);
	 //printf("yoyoyoyoyoyoy\n");

	for(k=0;k<ninodeb;k++)
	{
		disk_read(k,block.data);
		for(i=1;i<INODES_PER_BLOCK;i++)
		{
			//disk_read(i,block.data);
			if(block.inode[i].isvalid == 1)
			{
				printf("inode %d\n",i );
				printf("	size: %d\n",block.inode[i].size);
				printf("	direct blocks: " );
				for( j = 0 ; j < POINTERS_PER_INODE ; j++ )
				{
					if(block.inode[i].direct[j] != 0)
					printf("%d ",block.inode[i].direct[j]);
				}
				printf("\n");

				printf("	indirect block: %d\n",block.inode[i].indirect);
				disk_read(block.inode[i].indirect,iblock.data);
				printf("	indirect data blocks: ");
				for(l=0;l<POINTERS_PER_BLOCK;l++)
				{
					if(iblock.pointers[l] != 0)
					printf("%d ",iblock.pointers[l]);
				}
				printf("\n");
			}
		}

	}

}

int fs_mount()
{
	return 0;
}

int fs_create()
{
	return 0;
}

int fs_delete( int inumber )
{
	return 0;
}

int fs_getsize( int inumber )
{
	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{
	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}
