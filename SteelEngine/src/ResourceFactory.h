#ifndef RESOURCEFACTORY_H
#define RESOURCEFACTORY_H

#include <cmath>
#include <ctime>
#include "Resource.h"

//The difference here is that you don't have to load anything from files in order to generate random resources.
//You will, however, need to adjust the parameters post-generation and perform other operations on the Territory in order
// for the Resource to be usable
class ResourceFactory
{
	private:


	public:
		ResourceFactory();

		Resource	*CreateNewRandomResource();
};

#endif