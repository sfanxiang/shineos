#ifndef _FS_H
#define _FS_H

#include "types.h"

#define FS_START_RESERVED 64
struct superdesc{
	u16 magic;
	u32 size;
	u16 state;
	u16 blocksize;	//bytes per block=blocksize*512
	u32 pdiskalloc;
	u32 wtimelow,wtimehigh;
	char name[];
};

struct treedesc{
	u16 present;
	u32 pfiledesc;
	u32 pprev;
	u32 pnext;
	u32 pparent;
	u32 pchild;
	char name[];
};

#define FILE_TYPE_ERROR 0
#define FILE_TYPE_DIR 1
#define FILE_TYPE_FILE 2

struct filedesc{
	u16 type;
	u32 mode;
	u32 sizelow,sizehigh;
	u32 pfile;
};

struct fileblock{
	u32 pdesc;
	u32 pprev;
	u32 pnext;
	u16 eof;
	u8 data[];
};

bool initfs(u8 drive,u32 start,struct superdesc *sdesc)
{
	struct dap drivedata;
	
	drivedata.size=sizeof(drivedata);
	drivedata.sectors=1;
	drivedata.address=getpfar(sdesc);
	drivedata.startsector=start+FS_START_RESERVED;
	drivedata.reserved=0;
	
	if(readdrivesectors(drive,&drivedata))
		return false;
	if(sdesc->magic!=(('s'<<8)+'f'))
		return false;
	return true;
}

#endif
