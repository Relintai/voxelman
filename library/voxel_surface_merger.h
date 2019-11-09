#ifndef VOXEL_SURFACE_MERGER_H
#define VOXEL_SURFACE_MERGER_H

#include "voxel_surface.h"

class VoxelSurfaceMerger : public VoxelSurface {
	GDCLASS(VoxelSurfaceMerger, VoxelSurface)

public:
	int get_atlas_x(const VoxelSurfaceSides side) const;
	void set_atlas_x(const VoxelSurfaceSides side, int value);

	int get_atlas_y(const VoxelSurfaceSides side) const;
	void set_atlas_y(const VoxelSurfaceSides side, int value);

	void refresh_rects();

	VoxelSurfaceMerger();
	~VoxelSurfaceMerger();

protected:
	static void _bind_methods();

private:
	int _atlas_positions[VOXEL_SIDES_ARRAY_SIZE];
};

#endif
