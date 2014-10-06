#include <cstdlib>
#include <iostream>
using namespace std;

#include "fs.h"
#include "types.h"

int main(int argc,char *argv[])
{
	if(argc<6)
	{
		cout<<"\
Too few argument(s).\n\
Usage: <target> <dir> <partition index> <block size> <part size>\n";
		return 1;
	}
	else
	{
		writefs wfs;
		wfs.open(argv[1]);
		if(!wfs.writepart(argv[2],wfs.getpart(atoi(argv[3])),atoi(argv[4]),atoi(argv[5])))	//todo
		{
			cout<<"Failed."<<endl;
			return 1;
		}
		wfs.close();
	}	
	
	return 0;
}
