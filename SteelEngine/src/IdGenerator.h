#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <map>

class IdGenerator
{
	private:
		
		IdGenerator(){ m_lastId = 0; }
		IdGenerator(const IdGenerator &other);
		void operator=(IdGenerator const&);
		
		unsigned int m_lastId;

	public:
		unsigned int NewId() { m_lastId++; return m_lastId; }

		static IdGenerator &GetInstance()
		{
			static IdGenerator instance;

			return instance;
		}
		
};

#endif