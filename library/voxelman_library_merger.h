#ifndef VOXELMAN_LIBRARY_MERGER_H
#define VOXELMAN_LIBRARY_MERGER_H

#include "voxelman_library.h"

#include "core/resource.h"
#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface_merger.h"

class VoxelSurfaceSimple;
class VoxelMesher;

class VoxelmanLibraryMerger : public VoxelmanLibrary {
	GDCLASS(VoxelmanLibraryMerger, VoxelmanLibrary)

public:
	int get_atlas_columns() const;
	void set_atlas_columns(int s);

	int get_atlas_rows() const;
	void set_atlas_rows(int s);

	Ref<VoxelSurface> get_voxel_surface(int index) const;
	void set_voxel_surface(int index, Ref<VoxelSurface> value);
	void remove_surface(int index);
	int get_num_surfaces();

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &surfaces);

	void refresh_rects();

	VoxelmanLibraryMerger();
	~VoxelmanLibraryMerger();

protected:
	static void _bind_methods();

private:
	Vector<Ref<VoxelSurfaceMerger> > _voxel_surfaces;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
};

#endif // VOXEL_LIBRARY_H
