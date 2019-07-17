#ifndef VOXEL_LIGHT_H
#define VOXEL_LIGHT_H

#include "core/color.h"
#include "core/reference.h"
#include "core/vector.h"

#include "../math/vector3i.h"

class VoxelLight : public Reference {
	GDCLASS(VoxelLight, Reference);

public:
    int get_world_position_x();
    int get_world_position_y();
    int get_world_position_z();
	Vector3i get_world_position();
	void set_world_position(int x, int y, int z);

	Color get_color();
	void set_color(Color color);

	float get_size();
	void set_size(float strength);

	VoxelLight();
	~VoxelLight();

private:
	static void _bind_methods();

private:
	Vector3i _chunk_position;
	Vector3i _world_position;
	Color _color;
	int _size;
};

#endif
