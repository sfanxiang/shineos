#ifndef FS_H
#define FS_H

#include "types.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class writefs{
public:
	struct superdesc{
		u16 magic;
		u32 blockcount;
		u16 state;
		u16 blocksize;
		u32 pdiskalloc;
		u32 proot;
		u64 wtime;
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
		u64 size;
		u64 ctime;
		u64 wtime;
		u32 pfile;
	};
	
	struct fileblock{
		u32 pfiledesc;
		u32 pprev;
		u32 pnext;
		u16 datasize;
		u8 data[];
	};
	
	fstream file;
	
	void open(string filename)
	{
		file.open(filename.c_str(),ios::in|ios::out|ios::binary);
	}
	
	void close()
	{
		file.close();
	}
	
	bool writefrom(string path,u8 part=0)
	{
		cout<<path<<endl;
		DIR *dir;
		dirent *dent;
		struct stat s;
		if(!(dir=opendir(path.c_str())))
			return false;
		while(dent=readdir(dir))
		{
			if(!strncmp(dent->d_name,".",1))
				continue;
			if(!stat((path+"/"+dent->d_name).c_str(),&s)
				&&S_ISDIR(s.st_mode))
			{
				//make dir here
				if(!writefrom((path+"/"+dent->d_name).c_str(),part))
					return false;
			}
			else
			{
				//make file here
			}
		}
		closedir(dir);
		return true;
	}
};

#endif
