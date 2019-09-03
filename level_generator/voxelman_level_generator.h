#ifndef VOXELMAN_LEVEL_GENERATOR_H
#define VOXELMAN_LEVEL_GENERATOR_H

#include "core/resource.h"

class VoxelmanLevelGenerator : public Resource {
	GDCLASS(VoxelmanLevelGenerator, Resource);

public:
	VoxelmanLevelGenerator();
	~VoxelmanLevelGenerator();

protected:
	static void _bind_methods();

};

#endif

