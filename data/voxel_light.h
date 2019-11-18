#ifndef VOXEL_LIGHT_H
#define VOXEL_LIGHT_H

#include "core/color.h"
#include "core/reference.h"
#include "core/vector.h"

class VoxelLight : public Reference {
	GDCLASS(VoxelLight, Reference);

public:
	int get_world_position_x() const;
	int get_world_position_y() const;
	int get_world_position_z() const;
	Vector3 get_world_position();
	void set_world_position(const int x, const int y, const int z);

	Color get_color() const;
	void set_color(const Color color);

	float get_size() const;
	void set_size(const float strength);

	VoxelLight();
	~VoxelLight();

private:
	static void _bind_methods();

private:
	int _chunk_position_x;
	int _chunk_position_y;
	int _chunk_position_z;

	int _world_position_x;
	int _world_position_y;
	int _world_position_z;

	Color _color;
	int _size;
};

#endif
