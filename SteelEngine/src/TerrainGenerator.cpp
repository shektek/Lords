#include "TerrainGenerator.h"
#include <vector>

struct CharPair
{
	unsigned char a;
	unsigned char b;
};

unsigned char *Steel::TerrainGenerator::Generate(const int width, const int height)
{
	bool smoothing = false;
	unsigned char *terrain = new unsigned char[width*height * sizeof(unsigned char)];

	//begin by elevating the whole map to a random value

	unsigned char rz = rand() % 255;
	
	//let's consider 0-50 to be sea level, and 200+ to be alpine
	//first apply a random perturbation between -5 and 5 over every point
	//smoothing as we go

	unsigned char **t2 = new unsigned char*[width];
	
	for (int i = 0; i < width; i++)
	{
		t2[i] = new unsigned char[height];
		for (int j = 0; j < height; j++)
		{
			t2[i][j] = rz + ((rand() % 10) - 5);

			//roll to get a higher random thing
			if (rand() % 100 == 0)
			{
				t2[i][j] = rz + ((rand() % 14) - 7);
			}
		}
	}

	//place some mountains
	//first, determine how many to place, and the dimensions of each mountain range
	//within each mountain range is some variation also, but we can leave that as totally random
	unsigned char numMountains = rand() % 6;
	unsigned char widthMountains = (rand() % width) + 4;
	unsigned char lengthMountains = (rand() % height) + 4;

	for (int mi = numMountains; mi > 0; mi--)
	{
		int posx = rand() % width;
		int posy = rand() % height;
		
		t2[posx][posy] += rand() % 75 + 25;

		//first, average around the peak
		unsigned char surrounds[3][3];
		memset(surrounds, t2[posx][posy], 9);

		if (posx > 0 && posy > 0) t2[posx - 1][posy - 1] = (t2[posx - 1][posy - 1] + surrounds[0][0]) / 2;
		if (posx > 0) t2[posx - 1][posy] = (t2[posx - 1][posy] + surrounds[0][1]) / 2;
		if (posx > 0 && posy < height - 1) t2[posx - 1][posy + 1] = (t2[posx - 1][posy + 1] + surrounds[0][2]) / 2;
		if (posy < height - 1) t2[posx][posy + 1] = (t2[posx][posy + 1] + surrounds[1][2]) / 2;
		if (posx < width - 1 && posy < height - 1) t2[posx + 1][posy + 1] = (t2[posx + 1][posy + 1] + surrounds[2][2]) / 2;
		if (posx < width - 1) t2[posx + 1][posy] = (t2[posx + 1][posy] + surrounds[2][1]) / 2;
		if (posx < width - 1 && posy > 0) t2[posx + 1][posy - 1] = (t2[posx + 1][posy - 1]  + surrounds[2][0]) / 2;
		if (posy > 0) t2[posx][posy - 1] = (t2[posx][posy - 1] + surrounds[1][0]) / 2;

		//now average around those averages
		//this is where length and width of mountains is important

	}

	if (smoothing)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				unsigned char surrounds[3][3];
				memset(surrounds, t2[i][j], 9);

				if (i > 0 && j > 0) surrounds[0][0] = t2[i - 1][j - 1];
				if (i > 0) surrounds[0][1] = t2[i - 1][j];
				if (i > 0 && j < height - 1) surrounds[0][2] = t2[i - 1][j + 1];
				if (j < height - 1) surrounds[1][2] = t2[i][j + 1];
				if (i < width - 1 && j < height - 1) surrounds[2][2] = t2[i + 1][j + 1];
				if (i < width - 1)  surrounds[2][1] = t2[i + 1][j];
				if (i < width - 1 && j > 0) surrounds[2][0] = t2[i + 1][j - 1];
				if (j > 0) surrounds[1][0] = t2[i][j - 1];

				int avg = 0;
				for (int ai = 0; ai < 3; ai++) for (int aj = 0; aj < 3; aj++) avg += surrounds[ai][aj];

				avg /= 9;

				t2[i][j] = avg;
			}
		}
	}
	

	//find lowest points
	/*
	std::vector<CharPair> lowest;
	int lastLowest = rz + 10;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		if (t2[i][j] <= lastLowest)
		{
			CharPair pair = { i, j };
			lastLowest = t2[i][j];
			lowest.push_back(pair);
		}
	}

	//do something interesting to them
	for (int i = 0; i < lowest.size() - 1; i++)
	{
	}
	*/

	for (int i = 0; i < width; i++)
	{
		memcpy(&terrain[i*width], t2[i], height);
		delete[] t2[i];
	}

	return terrain;
}

void RecursePeaks(unsigned char **buf, const int pWidth, const int pHeight, const int centerX, const int centerY)
{

}