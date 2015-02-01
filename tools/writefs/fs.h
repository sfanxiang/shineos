#ifndef FS_H
#define FS_H

#include <cstddef>
#include <cstring>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "defines.h"

#define HD_SECTOR_SIZE 512

#define FS_MAGIC ('s'+('f'<<8))
#define FS_START_RESERVED 64

class writefs{
public:
#pragma pack(push)
#pragma pack(1)
	struct partentry{
		u8 status;	//0x00:inactive,0x80:active
		u8 firsthead,firstsector,firstcylinder,type,lasthead,lastsector,lastcylinder;
		u32 firstlba,sectors;
	};
	
	struct mbr{
		u8 code1[218];
		u8 resv[6];
		u8 code2[216];
		u32 disksign;
		u16 copy_protect;
		partentry parttable[4];
		u16 bootsign;
	};

	struct superdesc{
		u16 magic;
		u32 blockcount;
		u16 state;
		u16 blocksize;
		u32 palloc;
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
#pragma pack(pop)
	
	fstream file;
	
	void open(string filename)
	{
		file.open(filename.c_str(),ios::in|ios::out|ios::binary);
	}
	
	void close()
	{
		file.close();
	}
	
	u32 getpart(u8 index)
	{
		file.seekg(offsetof(mbr,parttable[0])+sizeof(partentry)*index+offsetof(partentry,firstlba));
		if(file.fail())return 0;
		u32 part;
		file.read((char*)&part,sizeof(part));
		return part;
	}
	
	bool writeblock(u32 part,u32 block,u16 blocksize,u16 offset,u64 bytes,void *src)
	{
		file.seekp((part+FS_START_RESERVED+block*blocksize)*HD_SECTOR_SIZE+offset);
		if(file.fail())return false;
		file.write((char*)src,bytes);
		if(file.fail())return false;
		return true;
	}
	
	u32 writefileblocks(string fn,u32 pfiledesc,u32 part,u32 block,u16 blocksize)
	{
		u32 orgblock=block;
		ifstream fin(fn.c_str(),ios::binary);
		u8 cur[blocksize*HD_SECTOR_SIZE-offsetof(fileblock,data)];
		u8 fblock[blocksize*HD_SECTOR_SIZE];
		bool first=true;
		do
		{
			fin.read((char*)cur,sizeof(cur));
			((fileblock*)fblock)->datasize=fin.gcount();
			if(fin.peek()==EOF)
				((fileblock*)fblock)->pnext=0;
			else
				((fileblock*)fblock)->pnext=block+1;
			if(first)
				((fileblock*)fblock)->pprev=0;
			else
				((fileblock*)fblock)->pprev=block-1;
			((fileblock*)fblock)->pfiledesc=pfiledesc;
			memcpy(((fileblock*)fblock)->data,cur,((fileblock*)fblock)->datasize);
			
			if(!writeblock(part,block,blocksize,0,offsetof(fileblock,data)+((fileblock*)fblock)->datasize,fblock))
				return 0;
			first=false;
			block++;
		}while(fin.peek()!=EOF);
		return block-orgblock;
	}
	
private:
	struct writeitem{
		u32 block;
		string path;
	};
	
public:
	bool writetree(string path,u32 part,u16 blocksize)
	{
		queue<writeitem>wq;
		wq.push({1,path});
		u8 root[sizeof(treedesc)+1]={0};
		if(!writeblock(part,1,blocksize,0,sizeof(root),root))
			return false;
		
		u32 usedblocks=1;
		while(!wq.empty())
		{
			writeitem x=wq.front();wq.pop();
			struct stat s;
			if(stat(x.path.c_str(),&s))continue;
			if(S_ISDIR(s.st_mode))
			{
				DIR *dir;
				if(!(dir=opendir(x.path.c_str())))
					return false;
				dirent *dent;bool first=true;
				while(dent=readdir(dir))
				{
					if(dent->d_name[0]=='.')
						continue;
					
					usedblocks++;
					u8 cur[sizeof(treedesc)+strlen(dent->d_name)+1];
					((treedesc*)cur)->pfiledesc=((treedesc*)cur)->pnext
						=((treedesc*)cur)->pchild=0;
					((treedesc*)cur)->pparent=x.block;
					
					if(first)
					{
						first=false;
						if(!writeblock(part,x.block,blocksize,offsetof(treedesc,pchild),
							sizeof(usedblocks),&usedblocks))
						{
							closedir(dir);
							return false;
						}
						((treedesc*)cur)->pprev=0;
					}
					else
					{
						if(!writeblock(part,wq.back().block,blocksize,offsetof(treedesc,pnext),
							sizeof(usedblocks),&usedblocks))
						{
							closedir(dir);
							return false;
						}
						((treedesc*)cur)->pprev=wq.back().block;
					}
					strcpy(((treedesc*)cur)->name,dent->d_name);
					if(!writeblock(part,usedblocks,blocksize,0,sizeof(cur),cur))
					{
						closedir(dir);
						return false;
					}
					
					wq.push({usedblocks,x.path+'/'+dent->d_name});
				}
				closedir(dir);
			}
			else if(S_ISREG(s.st_mode))
			{
				usedblocks++;
				if(!writeblock(part,x.block,blocksize,offsetof(treedesc,pfiledesc),
					sizeof(usedblocks),&usedblocks))
					return false;
				
				filedesc fdesc;
				fdesc.mode=0b111111111111;	//todo
				fdesc.size=s.st_size;
				fdesc.ctime=0;			//todo
				fdesc.wtime=s.st_mtime;
				fdesc.pfile=usedblocks+1;
				
				if(!writeblock(part,usedblocks,blocksize,0,sizeof(fdesc),&fdesc))
					return false;
				
				u32 blockswrote=writefileblocks(x.path,usedblocks,part,usedblocks+1,blocksize);	
				if(!blockswrote)
					return false;
				usedblocks+=blockswrote;
			}
		}
		//todo: write alloc
		return true;
	}
	
	bool writepart(string path,u32 part,u16 blocksize,u32 blockcount)
	{
		u8 sdesc[sizeof(superdesc)+1];
		((superdesc*)sdesc)->magic=FS_MAGIC;
		((superdesc*)sdesc)->blockcount=blockcount;
		((superdesc*)sdesc)->state=0;
		((superdesc*)sdesc)->blocksize=blocksize;
		((superdesc*)sdesc)->proot=1;
		((superdesc*)sdesc)->wtime=0;	//todo
		((superdesc*)sdesc)->name[0]=0;
		if(!writeblock(part,0,blocksize,0,sizeof(sdesc),sdesc))
			return false;
		return writetree(path,part,blocksize);
	}
};

#endif
