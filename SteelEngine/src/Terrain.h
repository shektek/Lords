#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>

namespace Steel
{
	/*	Terrain takes control of a pointer assigned elsewhere. The other pointer should not be deleted, but set to nullptr.
		Terrain will delete m_data in destructor.
		So yes, this is a SHALLOW COPY, but we're doing that on purpose to avoid consuming a lot of memory.
	*/
	class Terrain
	{
	private:
		std::string		m_name;
		std::string		m_hmFilename;
		std::string		m_diffuseFilename;
		std::string		m_detailFilename;
		unsigned int	m_width;
		unsigned int	m_height;
		int				m_scale;

	public:
		unsigned char	*m_data;	//check for nullptr! If null, then the data is held by irrlicht

		Terrain(const std::string name, unsigned char *data, const unsigned int width, const unsigned int height, const unsigned int scale, const std::string hmfile, const std::string diffuseFile, const std::string detailFile);
		~Terrain();

		inline std::string		GetName() const { return m_name; }
		inline std::string		GetHeightmapFilename() const { return m_hmFilename; }
		inline std::string		GetDiffuseFilename() const { return m_diffuseFilename; }
		inline std::string		GetDetailFilename() const { return m_detailFilename; }
		inline unsigned int		GetWidth() const { return m_width; }
		inline unsigned int		GetHeight() const { return m_height; }
		inline int				GetScale() const { return m_scale; }

		inline std::string		SetName(std::string name){ m_name = name; }
		inline int				SetScale(const int scale){ m_scale = scale; }
	};
}

#endif