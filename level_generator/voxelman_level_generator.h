#ifndef VOXELMAN_LEVEL_GENERATOR_H
#define VOXELMAN_LEVEL_GENERATOR_H

#include "core/resource.h"

#include "../world/voxel_chunk.h"

class VoxelmanLevelGenerator : public Resource {
	GDCLASS(VoxelmanLevelGenerator, Resource);

public:
	void generate_chunk_bind(Node *chunk);
	void generate_chunk(VoxelChunk *chunk);

	VoxelmanLevelGenerator();
	~VoxelmanLevelGenerator();

protected:
	static void _bind_methods();

};

#endif

