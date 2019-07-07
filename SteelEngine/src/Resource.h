#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>
#include "vector2d.h"

typedef irr::core::vector2d<double> Vector2d;

/* A resource is like a mine, a resource source but that's awkward looking
	It can only supply one resource type
	Efficiency will be adjusted by the player assigning serfs to operate it
	Max output tonnage is considered 100% efficiency:
		- a large workbase is probably needed to achieve this
		- specialised supply lines would be needed to exceed this
	Seasonal output is sent to the stockpile
	The stockpile is filled gradually filled until the next supply cart arrives
*/

enum ResourceType
{
	R_NONE = 0,
	R_LUMBER,
	R_STONE,
	R_IRON,
	R_WHEAT,
	R_DAIRY,
	R_MEAT,
	R_WATER,
	R_ALE,
	R_WINE,
	R_BLACKSMITH,
	R_MAX
};

 const std::string ResourceName[] =
{
	"Idle",
	"Forestry",
	"Quarry",
	"Mining",
	"Wheat Farming",
	"Dairy",
	"Hunting",
	"Water supply",
	"Brewery",
	"Vineyard",
	" "
};

class Resource
{
	protected:
		std::string		m_name;				//This is like a label used on maps etc.
		Vector2d		m_location;			//Where to place it on the map
		double			m_maxOutputTons;	//The most that it could produce in one season
		double			m_efficiency;		//How much of maxOutputTons is produced in 1 season
		bool			m_operational;		//Is it producing?
		ResourceType	m_resourceType;		//See ResourceType above
		double			m_seasonalOutput;	//Output in tons, m_maxOutputTons * (m_efficiency / 100.0)
		double			m_stockpileLoad;	//Current amount on the local stockpile - must be taken to town before it can be used
		double			m_stockpileMax;		//Maximum amount (tons) to put on the local stockpile before work must halt

	public:
		Resource();
		Resource(ResourceType type, const std::string &name, const Vector2d &location, const double &maxOutput, const double &maxStockpile, bool operational, double startingEfficiency, double startingStock);
		Resource(const Resource &other);
		Resource(Resource *other);
		virtual ~Resource();

		inline std::string	GetName() const { return m_name; }
		inline double	GetEfficiency() const { return m_efficiency; }
		inline double	GetMaxOutputTons() const { return m_maxOutputTons; }
		inline Vector2d	GetLocation() const { return m_location; }
		inline bool		IsOperating() const { return m_operational; }
		inline ResourceType	GetResourceType() const { return m_resourceType; }
		inline double	GetSeasonalOutput() const { return m_seasonalOutput; }
		inline double	GetCurrentStockpileLoad() const { return m_stockpileLoad; }
		inline double	GetMaxStockpileLoad() const { return m_stockpileMax; }

		//This affects the seasonal output. Returns the calculated efficiency level
		double			AdjustEfficiency(double addedEfficiency);
		//This can be reduced by supply carts, increased by seasonal output, etc. Returns the calculated load - this may be higher than the maximum.
		double			AdjustStockpileLoad(double addedLoad);
		
		inline void		SetName(const std::string &name) { m_name = name; }
		inline void		SetEfficiency(const double &newEfficiency) { m_efficiency = newEfficiency; m_seasonalOutput = m_maxOutputTons * (m_efficiency / 100.0); }
		inline void		SetMaxOutputTons(const double &newMaxOutput) { m_maxOutputTons = newMaxOutput; }
		inline void		SetLocation(const Vector2d &newLocation) { m_location = newLocation; }
		inline void		SetOperationState(const bool &newOperatingState){ m_operational = newOperatingState; }
		inline void		SetResourceType(const ResourceType &newResourceType){ m_resourceType = newResourceType; }
};

#endif