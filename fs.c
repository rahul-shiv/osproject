
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



int fs_format(int n, int mount)
{
    if (mount == 0){
        union fs_block block;
        block.super.magic = FS_MAGIC;
        printf("\nn=%d\n",n);
        block.super.nblocks = n;
        block.super.ninodeblocks = (n + 10)/10;
        block.super.ninodes = block.super.ninodeblocks*128;
        disk_write(0,block.data);
        for (int i = 1; i < n; i++){
            disk_write(i,"");
        }
        return 1;
    }
	return 0;
}



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

int fs_mount(int *mount)
{
    union fs_block block;
    disk_read(0,block.data);
    if (block.super.magic == FS_MAGIC){
        printf("\nDisk mounted");
        *(mount)++;
        return 1;
    }   
    else{
        printf("\nMount falied.");
        return 0;
    }
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


void inode_load( int inumber, struct fs_inode *inode ) {
	union fs_block block;
	int blockno=(((inumber-1)*32)/4096)+1;
	disk_read(blockno,block.data);
	int noinblock=((inumber-1)%128);
	inode=block.inode[noinblock];
}

int fs_read( int inumber, char *data, int length, int offset )
{
	struct fs_inode *inode ;
	union fs_block block,iblock;
	int direct_blockno,act_blockno,off,i,count_no_bytes=0;
	inode_load( inumber,inode );
	if(inode->isvalid != 1)
		return 0;
	else
	{
		//go to the correct offset
		// considering only direct blocks
		//max data in all direct blocks pointed by one inodenumber =20480
		if((offset+length)<20480)
		{
			//calculate which block it is starts.
			direct_blockno=offset/4096;
			//find which data block it actually refers to
			act_blockno=inode->direct[direct_blockno];
			//read that particular data block
			disk_read(act_blockno,block.data);
			//calculate the remaining offset. i.e the offset in that particular data block 
			off=offset%4096;
			//read from offset to length
			//considering length is less than 4096-offset

			if(length<4096-off)
			{
				//start reading from that place till length
				for(i=off;i<length;i++)
				{
					data[i]=block.data[i];
					count_no_bytes=count_no_bytes+1;
				}
			}
			//else we have to read the next data block also
			//if no data blocks remaining return number of bytes copied till now
		
		}
		
	}

	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}
