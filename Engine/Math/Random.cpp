#include "Random.h"
#include <stdlib.h>

namespace PhoenixEngine
{
	void SeedRandom(unsigned int seed)
	{
		srand(seed);
	}

	float Random()
	{
		return rand() / static_cast<float>(RAND_MAX); // 0 - 1
	}

	float RandomRange(float min, float max)
	{
		return min + (max - min) * Random(); // 3 - 10 = 3 + (7)
	}
	
	int RandomInt()
	{
		return rand();
	}
	
	int RandomRangeInt(int min, int max)
	{
		return min + rand() % (max - min);
	}
}