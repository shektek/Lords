#include "Terrain.h"

namespace Steel
{
	Terrain::Terrain(const std::string name, unsigned char *data, const unsigned int width, const unsigned int height, const unsigned int scale, const std::string hmfile, const std::string diffuseFile, const std::string detailFile)
	{
		m_name = name;
		m_width = width;
		m_height = height;
		m_scale = scale;
		m_data = data;	//shallow copying is for men only, children need not apply
		m_hmFilename = hmfile;
		m_diffuseFilename = diffuseFile;
		m_detailFilename = detailFile;
	}

	Terrain::~Terrain()
	{
		m_name.clear();
		m_scale = 0;
		delete[] m_data;
		m_width = 0;
		m_height = 0;
	}
}