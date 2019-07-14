#ifndef VOXEL_MESHER_CUBIC_H
#define VOXEL_MESHER_CUBIC_H

#include "../voxel_mesher.h"

class VoxelMesherCubic : public VoxelMesher {
	GDCLASS(VoxelMesherCubic, VoxelMesher);

public:

	VoxelMesherCubic();

protected:
	static void _bind_methods();

};

#endif
