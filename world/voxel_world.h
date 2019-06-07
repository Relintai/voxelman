#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "scene/3d/spatial.h"
#include "core/hash_map.h"

#include "../math/vector3i.h"
#include "voxel_buffer.h"

class VoxelWorld : public Spatial {
	GDCLASS(VoxelWorld, Spatial);

public:
	NodePath get_player_path();
	void set_player_path(NodePath player_path);

	VoxelWorld() {}
	~VoxelWorld();

protected:
	static void _bind_methods();

private:
	HashMap<Vector3i, Ref<VoxelBuffer> > _chunks; 

	NodePath _player_path;
	Spatial *_player;
};

#endif
