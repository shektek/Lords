#include "Territory.h"

Territory::Territory()
{
	m_name = "Unknown land";
	m_needsScaling = true;
}

Territory::Territory(const std::string &name, Pos3 castlePosition, Pos3 townPosition, PeasantFactory *peasantFactory)
{
	m_name = name;
	m_peasants.clear();
	m_resources.push_back(new Resource(R_NONE, ResourceName[R_NONE], Vector2d(0, 0), 0, 0, true, 0, 0));
	ResourceLabourAllocation rla;
	rla.name = m_resources[0]->GetName();
	rla.peasants.clear();
	rla.resource = m_resources[0];
	m_castlePosition = castlePosition;
	m_townPosition = townPosition;
	m_labourAllocations.push_back(rla);
	m_bounds.clear();
	m_mapBounds.clear();
	m_peasantFactory = peasantFactory;
	m_needsScaling = true;
}

Territory::Territory(const Territory &other)
{
	m_name = other.m_name;
	m_resources = other.m_resources;
	m_labourAllocations = other.m_labourAllocations;
	m_peasants = other.m_peasants;
	m_peasantFactory = other.m_peasantFactory;
	//TODO: Finish copying everything else
}

Territory::Territory(Territory *other)
{
	m_name = other->m_name;
	m_resources = other->m_resources;
	m_labourAllocations = other->m_labourAllocations;
	m_peasants = other->m_peasants;
	m_peasantFactory = other->m_peasantFactory;
	//TODO: Finish copying everything else
}

Territory::~Territory()
{
	m_name.clear();
	
	for (Resource *r : m_resources)
		delete r;

	m_resources.clear();

	for (Peasant *p : m_peasants)
		delete p;

	m_peasants.clear();

	//this should also be empty by now
	m_labourAllocations.clear();
}

void Territory::PassSeason()
{
	for (ResourceLabourAllocation &r : m_labourAllocations)
	{
		double efficiency = 0;

		for (Peasant *p : r.peasants)
			efficiency += p->GetEfficiencyModifier();

		r.resource->SetEfficiency(efficiency);
		r.resource->AdjustStockpileLoad(r.resource->GetSeasonalOutput());
	}
}

void Territory::RandomlyDistributeLabour()
{
	int remainingPeasants = m_peasants.size();
	int lastEnd = 0;

	for (ResourceLabourAllocation &rla : m_labourAllocations)
	{
		if (rla.resource->GetResourceType() != R_NONE)
		{
			rla.peasants.clear();
			int r = rand() % remainingPeasants;
			rla.peasants = std::vector<Peasant*>(m_peasants.begin() + lastEnd, m_peasants.begin() + lastEnd + r);
			lastEnd += r;
			remainingPeasants -= r;
		}
	}

	for (ResourceLabourAllocation &rla : m_labourAllocations)
	{
		if (rla.resource->GetResourceType() == R_NONE)
		{
			rla.peasants = std::vector<Peasant*>(m_peasants.begin()+lastEnd, m_peasants.end());
			break;
		}
	}
}

unsigned int Territory::AddResource(Resource *r)
{
	if(r->GetName().size() <= 1)
		r->SetName(m_name + " " + ResourceName[r->GetResourceType()]);
	
	//eventually have r->SetLocation, but only if it's 0, 0...?
	m_resources.push_back(r);
	ResourceLabourAllocation rla;
	rla.resource = r;
	rla.name = m_name + " " + ResourceName[r->GetResourceType()];
	rla.peasants.clear();
	m_labourAllocations.push_back(rla);

	return m_resources.size();
}

unsigned int Territory::AddResources(const unsigned int &num)
{
	return m_resources.size();
}

unsigned int Territory::AddResources(std::vector<Resource*> specificResources)
{
	for (Resource *r : specificResources)
	{
		if (r->GetName().size() <= 1)
			r->SetName(m_name + " " + ResourceName[r->GetResourceType()]);

		m_resources.push_back(r);
		ResourceLabourAllocation rla;
		rla.resource = r;
		rla.name = m_name + " " + ResourceName[r->GetResourceType()];
		rla.peasants.clear();
		m_labourAllocations.push_back(rla);
	}

	return m_resources.size();
}

unsigned int Territory::AddPeasant(Peasant *p)
{
	m_peasants.push_back(p);

	for (ResourceLabourAllocation &r : m_labourAllocations)
	{
		if (r.resource->GetResourceType() == R_NONE)
		{
			r.peasants.push_back(p);
			break;
		}
	}

	return m_peasants.size();
}

unsigned int Territory::AddPeasants(const unsigned int &num)
{
	for (unsigned int i = 0; i < num; i++)
	{
		m_peasants.push_back(m_peasantFactory->CreateNewRandomPeasant());
		
		for (ResourceLabourAllocation &r : m_labourAllocations)
		{
			if (r.resource->GetResourceType() == R_NONE)
			{
				r.peasants.push_back(m_peasants.back());
				break;
			}
		}
	}

	return m_peasants.size();
}

//this just gives the pointer to the peasant array, it doesn't create new objects
unsigned int Territory::AddPeasants(std::vector<Peasant*> specificPeasants)
{
	for (Peasant *p : specificPeasants)
	{
		m_peasants.push_back(p);
		
		for (ResourceLabourAllocation &r : m_labourAllocations)
		{
			if (r.resource->GetResourceType() == R_NONE)
			{
				r.peasants.push_back(m_peasants.back());
				break;
			}
		}
	}

	return m_peasants.size();
}
