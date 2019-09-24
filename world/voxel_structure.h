#ifndef VOXEL_STRUCTURE_H
#define VOXEL_STRUCTURE_H

#include "core/reference.h"

class VoxelStructure : public Reference {
	GDCLASS(VoxelStructure, Reference);

public:
	VoxelStructure();
	~VoxelStructure();
    
protected:
	static void _bind_methods();

};

#endif
