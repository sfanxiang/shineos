#include <iostream>
using namespace std;

#include "fs.h"
#include "types.h"

int main(int argc,char *argv[])
{
	if(argc<3)
	{
		cout<<"Too few argument(s)."<<endl;
		return 1;
	}
	else
	{
		writefs wfs;
		wfs.open(argv[1]);
		if(!wfs.writetree(argv[2],0x100,2))	//todo
		{
			cout<<"Failed."<<endl;
			return 1;
		}
		wfs.close();
	}	
	
	return 0;
}
