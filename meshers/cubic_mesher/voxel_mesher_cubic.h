#ifndef VOXEL_MESHER_CUBIC_H
#define VOXEL_MESHER_CUBIC_H

#include "core/color.h"
#include "core/math/vector3.h"
#include "core/math/vector2.h"

#include "../voxel_mesher.h"

#include "voxel_cube_points.h"

class VoxelMesherCubic : public VoxelMesher {
	GDCLASS(VoxelMesherCubic, VoxelMesher);

public:
	float get_ao_strength() const;
	void set_ao_strength(float value);

	float get_base_light_value() const;
	void set_base_light_value(float value);

	void _add_buffer(Ref<VoxelBuffer> buffer);

	VoxelMesherCubic();
	~VoxelMesherCubic();

protected:
	static void _bind_methods();

private:
	float _ao_strength;
	float _base_light_value;
};

#endif
