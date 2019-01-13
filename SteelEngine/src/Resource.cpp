#include "Resource.h"

Resource::Resource()
{
	m_name = "Unknown resource";
	m_location = Vector2d(0, 0);
	m_maxOutputTons = 0;
	m_efficiency = 0;
	m_operational = false;
	m_resourceType = R_NONE;
	m_seasonalOutput = 0;
	m_stockpileLoad = 0;
	m_stockpileMax = 0;
}

Resource::Resource(ResourceType type, const std::string &name, const Vector2d &location, const double &maxOutput, const double &maxStockpile, bool operational, double startingEfficiency, double startingStock)
{
	m_name = name;
	m_resourceType = type;
	m_location = location;
	m_maxOutputTons = maxOutput;
	m_stockpileMax = maxStockpile;
	m_operational = operational;
	m_efficiency = startingEfficiency;
	m_stockpileLoad = startingStock;
	m_seasonalOutput = m_maxOutputTons * (m_efficiency / 100.0);
}

Resource::Resource(const Resource &other)
{
	m_name = other.m_name;
	m_efficiency = other.m_efficiency;
	m_location = other.m_location;
	m_maxOutputTons = other.m_maxOutputTons;
	m_operational = other.m_operational;
	m_resourceType = other.m_resourceType;
	m_seasonalOutput = other.m_seasonalOutput;
	m_stockpileLoad = other.m_stockpileLoad;
	m_stockpileMax = other.m_stockpileMax;
}

Resource::Resource(Resource *other)
{
	m_name = other->m_name;
	m_efficiency = other->m_efficiency;
	m_location = other->m_location;
	m_maxOutputTons = other->m_maxOutputTons;
	m_operational = other->m_operational;
	m_resourceType = other->m_resourceType;
	m_seasonalOutput = other->m_seasonalOutput;
	m_stockpileLoad = other->m_stockpileLoad;
	m_stockpileMax = other->m_stockpileMax;
}

Resource::~Resource()
{
	m_name.clear();
	m_location = Vector2d(0, 0);
	m_maxOutputTons = 0;
	m_efficiency = 0;
	m_operational = false;
	m_resourceType = R_NONE;
	m_seasonalOutput = 0;
	m_stockpileLoad = 0;
	m_stockpileMax = 0;
}

double Resource::AdjustEfficiency(double addedEfficiency)
{
	m_efficiency += addedEfficiency;
	m_seasonalOutput = m_maxOutputTons * (m_efficiency / 100.0);
	
	return m_efficiency;
}

double Resource::AdjustStockpileLoad(double addedLoad)
{
	double ret = m_stockpileLoad + addedLoad;
	
	m_stockpileLoad += addedLoad;
	if (m_stockpileLoad < 0) m_stockpileLoad = 0;
	else if (m_stockpileLoad > m_stockpileMax) m_stockpileLoad = m_stockpileMax;

	return ret;
}
