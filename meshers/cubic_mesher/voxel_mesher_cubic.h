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
	void _add_buffer(VoxelChunk *buffer);
	
	VoxelMesherCubic();
	~VoxelMesherCubic();

protected:
	static void _bind_methods();

};

#endif
