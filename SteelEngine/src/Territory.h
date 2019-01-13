#ifndef TERRITORY_H
#define TERRITORY_H

#include <vector>
#include <string>

#include "PeasantFactory.h"
#include "ResourceFactory.h"

/*
#include "Castle.h"
#include "Town.h"
*/

struct ResourceLabourAllocation
{
	std::string name;
	Resource *resource;
	std::vector<Peasant*> peasants;
};

struct Pos3
{
	double x;
	double y;
	double z;
};

class Territory
{
	private:
		std::string m_name;
		PeasantFactory	*m_peasantFactory;

		std::vector<Resource*>	m_resources;
		std::vector<Peasant*>	m_peasants;

		std::vector<ResourceLabourAllocation>	m_labourAllocations;
		std::vector<Pos3> m_bounds;		//normalised
		std::vector<Pos3> m_mapBounds;	//world-space

	public:
		Territory();
		Territory(const std::string &name, PeasantFactory *peasantFactory);
		Territory(const Territory &other);
		Territory(Territory *other);
		~Territory();

		void					PassSeason();
		void					RandomlyDistributeLabour();
		
		unsigned int			AddResource(Resource *resource);
		unsigned int			AddResources(const unsigned int &num);
		unsigned int			AddResources(std::vector<Resource*> specificResources);
		unsigned int			AddPeasant(Peasant *peasant);
		unsigned int			AddPeasants(const unsigned int &num);	//Adds new peasants to the population using the stored factory, returns total number
		unsigned int			AddPeasants(std::vector<Peasant*> specificPeasants);

		inline std::string		GetName() const { return m_name; }
		inline PeasantFactory	*GetPeasantFactory() { return m_peasantFactory; }
		inline std::vector<Pos3> GetLocalBounds() { return m_bounds; }
		inline std::vector<Pos3> GetMapBounds() { return m_mapBounds; }

		inline void				SetName(const std::string &name) { m_name = name; }
		inline void				SetPeasantFactory(PeasantFactory *peasantFactory) { m_peasantFactory = peasantFactory; }
		inline void				SetLocalBounds(std::vector<Pos3> bounds) { m_bounds = bounds; }	//realistically this will affect the map bounds as well
		inline void				SetMapBounds(std::vector<Pos3> mapBounds) { m_mapBounds = mapBounds; }
};

#endif