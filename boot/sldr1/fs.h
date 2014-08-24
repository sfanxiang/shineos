#ifndef _FS_H
#define _FS_H

#include "memory.h"
#include "types.h"

#define FS_START_RESERVED 64
struct superdesc{
	u16 magic;
	u32 blockcount;
	u16 state;
	u16 blocksize;	//bytes per block = blocksize*512
	u32 pdiskalloc;
	u32 proot;
	u32 wtimelow,wtimehigh;
	char name[];
};

struct treedesc{
	u32 pfiledesc;
	u32 pprev;
	u32 pnext;
	u32 pparent;
	u32 pchild;
	char name[];
};

struct filedesc{
	u32 mode;
	u32 sizelow,sizehigh;
	u32 ctimelow,ctimehigh;
	u32 wtimelow,wtimehigh;
	u32 pfile;
};

struct fileblock{
	u32 pfiledesc;
	u32 pprev;
	u32 pnext;
	u16 eof;
	u8 data[];
};

u8 readblock(u8 drive,u32 part,u32 block,u16 blocksize,void *dest)
{
	struct dap drivedata;
	
	drivedata.size=sizeof(drivedata);
	drivedata.sectors=blocksize;
	drivedata.address=getpfar(dest);
	drivedata.startsector=part+FS_START_RESERVED+block*blocksize;
	drivedata.reserved=0;
	
	if(readdrivesectors(drive,&drivedata))
		return 0;
	return 1;
}

u8 searchindir(u8 drive,u32 part,u32 dir,u16 blocksize,char *filename,u32 *pfiledesc,u32 *pchild)
{
	u8 data[8192];
	if(!readblock(drive,part,dir,blocksize,data))return 0;
	dir=((struct treedesc*)data)->pchild;
	while(dir)
	{
		if(!readblock(drive,part,dir,blocksize,data))return 0;
		if(!strcmp(((struct treedesc*)data)->name,filename))break;
		dir=((struct treedesc*)data)->pnext;
	}
	if(dir)
	{
		if(pfiledesc)*pfiledesc=((struct treedesc*)data)->pfiledesc;
		if(pchild)*pchild=((struct treedesc*)data)->pchild;
		return 1;
	}
	return 0;
}

u8 openfile(u8 drive,u32 part,char *path,u32 *pfile,u16 *blocksize,u32 *size)
{
	u8 data[8192];
	u32 cur;
	char *head=path;
	if(!readblock(drive,part,(u32)0,1,data))
		return 0;
	if(((struct superdesc*)data)->magic!=('s'+('f'<<8)))
		return 0;
	*blocksize=((struct superdesc*)data)->blocksize;
	cur=((struct superdesc*)data)->proot;
	while(*path)
	{
		if(*head=='/'){head++;path++;}
		else if(*path!='/')path++;
		else
		{
			*path=0;
			if(!searchindir(drive,part,
				cur,*blocksize,
				head,0,&cur)){
				*path='/';
				return 0;
			}
			*path='/';head=path;
		}
	}
	if(head!=path)
	{
		if(!searchindir(drive,part,
			cur,*blocksize,
			head,&cur,0))
			return 0;
		if(!readblock(drive,part,cur,
			*blocksize,data))
			return 0;
		if(pfile)*pfile=((struct filedesc*)data)->pfile;
		if(size)*size=((struct filedesc*)data)->sizelow;
		return 1;
	}
	return 0;
}

#endif
