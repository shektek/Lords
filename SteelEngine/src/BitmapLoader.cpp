#include "BitmapLoader.h"

BitmapFile::BitmapFile()
{
	_filename.clear();
	_data.clear();
	_fileHeader = { 0 };
}

BitmapFile::BitmapFile(std::string filename) : BitmapFile()
{
	_filename = filename;
	Read();
}

void BitmapFile::CheckColourHeader(BitmapColourHeader colourHeader)
{
	BitmapColourHeader expectedHeader;
	if (expectedHeader.RedMask != colourHeader.RedMask
		|| expectedHeader.BlueMask != colourHeader.BlueMask
		|| expectedHeader.GreenMask != colourHeader.GreenMask
		|| expectedHeader.AlphaMask != colourHeader.AlphaMask)
		return;
	if (expectedHeader.ColourSpaceType != colourHeader.ColourSpaceType)
		return;
}

uint32_t BitmapFile::MakeStrideAligned(uint32_t rowStride, uint32_t alignStride)
{
	uint32_t newStride = rowStride;
	while (newStride % alignStride != 0)
		newStride++;

	return newStride;
}

void BitmapFile::Read()
{
	if(_filename.size() > 0) Read(_filename);
}

void BitmapFile::Read(std::string filename)
{
	FILE *f = fopen(filename.c_str(), "rb");
	if (f)
	{
		fread(&_fileHeader, 1, sizeof(_fileHeader), f);

		if (_fileHeader.FileType == 0x4D42)
		{
			fread(&_infoHeader, 1, sizeof(_infoHeader), f);

			if (_infoHeader.BitCount == 32)
			{
				if (_infoHeader.HeaderSize >= (sizeof(BitmapInfoHeader)+sizeof(BitmapColourHeader)))
				{
					fread(&_colourHeader, 1, sizeof(_colourHeader), f);
					CheckColourHeader(_colourHeader);
				}
				else return;
			}

			fseek(f, _fileHeader.DataOffset, SEEK_SET);

			if (_infoHeader.BitCount == 32)
			{
				_infoHeader.HeaderSize = sizeof(BitmapInfoHeader)+sizeof(BitmapColourHeader);
				_fileHeader.DataOffset = sizeof(BitmapFile)+sizeof(BitmapInfoHeader)+sizeof(BitmapColourHeader);
			}
			else
			{
				_infoHeader.HeaderSize = sizeof(BitmapInfoHeader);
				_fileHeader.DataOffset = sizeof(BitmapFileHeader)+sizeof(BitmapInfoHeader);
			}
			
			_fileHeader.FileSize = _fileHeader.DataOffset;

			if (_infoHeader.Height < 0)
				return;	//to be complete later on

			_data.resize(_infoHeader.Width * _infoHeader.Height * _infoHeader.BitCount / 8);

			if (_infoHeader.Width % 4 == 0)
			{
				fread(_data.data(), 1, _data.size(), f);
				_fileHeader.FileSize += _data.size();
			}
			else
			{
				uint32_t rowStride = _infoHeader.Width * _infoHeader.BitCount / 8;
				uint32_t newStride = MakeStrideAligned(rowStride, 4);
				std::vector<uint8_t> paddingRow(newStride - rowStride);

				for (int y = 0; y < _infoHeader.Height; y++)
				{
					fread(_data.data() + rowStride*y, 1, rowStride, f);
					fread(paddingRow.data(), 1, paddingRow.size(), f);
				}

				_fileHeader.FileSize += _data.size() + _infoHeader.Height * paddingRow.size();
			}
		}

		fclose(f);
	}
}

BitmapColour BitmapFile::GetPixelAt(int x, int y)
{
	BitmapColour result(0, 0, 0, 0);
	int ch = _infoHeader.BitCount / 8;

	result.RGBA.B = _data[ch * _infoHeader.Width];
	result.RGBA.G = _data[ch * _infoHeader.Width + 1];
	result.RGBA.R = _data[ch * _infoHeader.Width + 2];
	if (ch == 4) result.RGBA.A = _data[ch * _infoHeader.Width + 3];

	return result;
}
