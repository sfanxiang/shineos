#ifndef _FS_H
#define _FS_H

struct superdesc{
	u2 magic;
	u4 size;
	u2 state;
	u2 blocksize;	//bytes per block=blocksize*512
	u4 pdiskalloc;
	u4 wtimelow,wtimehigh;
	char name[];
};

struct treedesc{
	u2 present;
	u4 pfiledesc;
	u4 pprev;
	u4 pnext;
	u4 pparent;
	u4 pchild;
	char name[];
};

#define FILE_TYPE_ERROR 0
#define FILE_TYPE_DIR 1
#define FILE_TYPE_FILE 2

struct filedesc{
	u2 type;
	u4 mode;
	u4 sizelow,sizehigh;
	u4 pfile;
};

struct fileblock{
	u4 pdesc;
	u4 pprev;
	u4 pnext;
	u2 eof;
	u1 data[];
};

bool initfs()	//todo

#endif
