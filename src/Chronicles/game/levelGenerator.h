#ifndef _LEVELGENERATOR_H
#define _LEVELGENERATOR_H

#include <cstdlib>		// For random numbers
#include "../entity/level.h"

class CLevelGenerator
{
public:
	virtual ~CLevelGenerator() { }
	virtual void GenerateLevel( CLevel* level, int seed, int difficulty ) = 0;
};

#endif
