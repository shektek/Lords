#include "ResourceFactory.h"

ResourceFactory::ResourceFactory()
{
	srand((unsigned int)time(0));
}

Resource *ResourceFactory::CreateNewRandomResource()
{
	int rrt = rand() % R_MAX;
	ResourceType rtype = static_cast<ResourceType>(rrt);
	
	double	maxOutputTons = ((rand() % 7) + 2) * 100.0;
	double	stockpileMax = maxOutputTons * ((rand() % 2)+1);

	return new Resource(rtype, "", Vector2d(0, 0), maxOutputTons, stockpileMax, true, 0, 0);
}