#include "voxelman_level_generator.h"

#include "../world/voxel_chunk.h"

void VoxelmanLevelGenerator::generate_chunk_bind(Node *chunk) {
	generate_chunk(Object::cast_to<VoxelChunk>(chunk));
}
void VoxelmanLevelGenerator::generate_chunk(VoxelChunk *chunk) {
	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk);
	}
}

VoxelmanLevelGenerator::VoxelmanLevelGenerator() {
}

VoxelmanLevelGenerator::~VoxelmanLevelGenerator() {
}

void VoxelmanLevelGenerator::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &VoxelmanLevelGenerator::generate_chunk_bind);
}
