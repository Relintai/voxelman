#ifndef GROUND_CLUTTER_H
#define GROUND_CLUTTER_H

#include "core/resource.h"

#include "../meshers/voxel_mesher.h"
#include "../../texture_packer/texture_packer.h"

class VoxelChunk;
class VoxelMesher;

class GroundClutter : public Resource {
	GDCLASS(GroundClutter, Resource);

public:
	bool should_spawn(VoxelChunk *chunk, int x, int y, int z);
	bool should_spawn_bind(Node *chunk, int x, int y, int z);

	void add_meshes_to(Ref<VoxelMesher> mesher, VoxelChunk *chunk, int x, int y, int z);
	void add_meshes_to_bind(Ref<VoxelMesher> mesher, Node *chunk, int x, int y, int z);
	void add_textures_to(Ref<TexturePacker> packer);

	GroundClutter();
	~GroundClutter();

private:
	static void _bind_methods();

private:
};

#endif
