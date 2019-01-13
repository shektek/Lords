#ifndef PEASANT_H
#define PEASANT_H

/*
	Later on, peasants will have their own personality system.
	Balancing this is integral to early game difficulty, as a smaller group
	of peasants will be more influenced by outlier personalities. 
	However, as the peasant population grows larger in the middle-end game,
	there will be less focus on managing the population except in unusual
	circumstances.
*/
class Peasant
{
	private:
		std::string		m_name;
		unsigned int	m_id;
		double m_efficiencyModifier;

	public:
		Peasant(unsigned int id, const std::string &name, double efficiencyModifier) { m_id = id; m_name = name; m_efficiencyModifier = efficiencyModifier; }
		
		double		GetId() const { return m_id; }
		std::string	GetName() const { return m_name; }
		double		GetEfficiencyModifier() const { return m_efficiencyModifier; }
};

#endif