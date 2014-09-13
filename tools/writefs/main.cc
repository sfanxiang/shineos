#include "fs.h"
#include "types.h"
#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
	if(argc<3)
	{
		cout<<"Too few argument(s)."<<endl;
		return 1;
	}
	
	writefs wf;
	wf.writefrom(".");
	
	return 0;
}
