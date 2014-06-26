// tools/sfsmgr/main.cpp
// SFS Manager

#include <iostream>
#include <string>
using namespace std;

void shell(string optf)
{
	string &file=optf;
	for(;;cout<<'\n')
	{
		cout<<"\
		Menu:\n\
		[0]New file\n\
		[1]Open file\n\
		[2]Close file\n\
		[3]Save file\n";
		
		int choice;
		cin>>choice;
		if(choice==0)
		{
			cout<<"Enter new file:\n";
		}
	}
}

int main(int argc,char *argv[])
{
	cout<<"================SFS Manager================\n";
	if(argc>1)
	{
		string optf;
		for(int i=1;i<argc;i++)
		{
			if(string(argv[i])=="-f")
			{
				if(i<argc-1)
					optf=argv[i++];
				else
				{
					cout<<"-f: expected 1 argument\n";
					return -1;
				}
			}
			else
			{
				cout<<argv[i]<<": not found\n";
				return -1;
			}
		}
		shell(optf);
	}
	else
		shell("");
	return 0;
}
