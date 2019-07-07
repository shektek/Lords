#ifndef BITMAPLOADER_H
#define BITMAPLOADER_H

#include <cstdint>
#include <vector>
#include <string>

#pragma pack(push, 1)

struct BitmapFileHeader
{
	uint16_t	FileType;
	uint32_t	FileSize;
	uint16_t	Reserved1;
	uint16_t	Reserved2;
	uint32_t	DataOffset;
};

struct BitmapInfoHeader
{
	uint32_t	HeaderSize{ 0 };
	int32_t		Width{ 0 };
	int32_t		Height{ 0 };
	uint16_t	Planes{ 1 };
	uint16_t	BitCount{ 0 };
	uint32_t	Compression{ 0 };
	uint32_t	ImageSize{ 0 };
	int32_t		xPixelsPerMetre{ 0 };
	int32_t		yPixelsPerMetre{ 0 };
	uint32_t	ColoursCount{ 0 };
	uint32_t	ColoursRequired{ 0 };
};

struct BitmapColourHeader
{
	uint32_t	RedMask		{ 0x00ff0000 };
	uint32_t	GreenMask	{ 0x0000ff00 };
	uint32_t	BlueMask	{ 0x000000ff };
	uint32_t	AlphaMask	{ 0xff000000 };
	uint32_t	ColourSpaceType{ 0x73524742 };	//sRGB
	uint32_t	Unused[16]{ 0 };
};

#pragma pack(pop)

struct BitmapChannels
{
	uint16_t	R;
	uint16_t	G;
	uint16_t	B;
	uint16_t	A;
};

struct BitmapColour
{
	union
	{
		BitmapChannels	RGBA;
		uint64_t		Value;
	};

	BitmapColour(uint16_t r, uint16_t g, uint16_t b, uint16_t a)
	{
		RGBA = { r, g, b, a };
	}
};

class BitmapFile
{
	private:
		BitmapFileHeader	_fileHeader;
		BitmapInfoHeader	_infoHeader;
		BitmapColourHeader	_colourHeader;

		std::vector<uint8_t> _data;
		std::string			_filename;

		void				CheckColourHeader(BitmapColourHeader colourHeader);
		uint32_t			MakeStrideAligned(uint32_t rowStride, uint32_t alignStride);

	public:
							BitmapFile();
							BitmapFile(std::string filename);

		void				Read();
		void				Read(std::string filename);

		BitmapColour		GetPixelAt(int x, int y);
		int					GetWidth() { return _infoHeader.Width; }
		int					GetHeight() { return _infoHeader.Height; }
};

#endif