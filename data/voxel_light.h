#ifndef VOXEL_LIGHT_H
#define VOXEL_LIGHT_H

#include "core/color.h"
#include "core/reference.h"
#include "core/vector.h"

#include "../collections/vector3i.h"

class VoxelLight : public Reference {
	GDCLASS(VoxelLight, Reference);

public:
	Vector3i get_local_position() { return _local_position; }
	void set_local_position(Vector3i neighbours) { _local_position = neighbours; }

	Vector3 get_world_position() { return _world_position; }
	void set_world_position(Vector3 world_position) { _world_position = world_position; }

	Vector3 get_offset() { return _offset; }
	void set_offset(Vector3 offset) { _offset = offset; }

	Color get_color() { return _color; }
	void set_color(Color color) { _color = color; }

	float get_strength() { return _strength; }
	void set_strength(float strength) { _strength = strength; }

	VoxelLight();
	VoxelLight(Vector3i position, Color color, float strength, Vector3 WorldPosition, Vector3 offset);
	~VoxelLight();

private:
	static void _bind_methods() {}

private:
	Vector3i _local_position;
	Vector3 _world_position;
	Vector3 _offset;
	Color _color;
	float _strength;
};

#endif
