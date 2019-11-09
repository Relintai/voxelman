#ifndef VOXEL_SURFACE_SIMPLE_H
#define VOXEL_SURFACE_SIMPLE_H

#include "voxel_surface.h"

class VoxelSurfaceSimple : public VoxelSurface {
	GDCLASS(VoxelSurfaceSimple, VoxelSurface)

public:
	int get_atlas_x(const VoxelSurfaceSides side) const;
	void set_atlas_x(const VoxelSurfaceSides side, int value);

	int get_atlas_y(const VoxelSurfaceSides side) const;
	void set_atlas_y(const VoxelSurfaceSides side, int value);

	void refresh_rects();

	VoxelSurfaceSimple();
	~VoxelSurfaceSimple();

protected:
	static void _bind_methods();

private:
	int _atlas_positions[VOXEL_SIDES_ARRAY_SIZE];
};

#endif
