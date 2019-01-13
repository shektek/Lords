#include <cstdio>

#include "AppManager.h"

int main(int argc, char **argv)
{
	AppManager *irrApp = new AppManager();

	if(irrApp->Init())
	{
		irrApp->Run();
		irrApp->Destroy();
	}

	return 0;
}