#ifndef VOXEL_H
#define VOXEL_H

#include "core/reference.h"
#include "../collections/vector3i.h"
#include "core/math/quat.h"
#include "core/resource.h"
#include "core/vector.h"

#include "../library/voxel_surface.h"

class VoxelSurface;

class Voxel : public Reference {
	GDCLASS(Voxel, Reference);

public:
	Ref<VoxelSurface> get_surface();
	void set_surface(Ref<VoxelSurface> surface);

	Vector3i get_local_position() const;
	void set_local_position(Vector3i value);

	Vector3 get_local_position_bind() const;
	void set_local_position_bind(Vector3 value);

	char get_fill();
	void set_fill(char fill);

	int get_fill_bind();
	void set_fill_bind(int fill);

	float get_light() const;
	void set_light(float value);

	Voxel();
	Voxel(Vector3i position, char fill, Ref<VoxelSurface> surface);
	~Voxel();

protected:
	static void _bind_methods();

private:
	char _fill;
	Ref<VoxelSurface> _surface;
	Vector3i _local_position;
	float _light;
};

#endif
