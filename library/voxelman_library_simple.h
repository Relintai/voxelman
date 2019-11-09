#ifndef VOXELMAN_LIBRARY_SIMPLE_H
#define VOXELMAN_LIBRARY_SIMPLE_H

#include "voxelman_library.h"

#include "core/resource.h"
#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface_simple.h"

class VoxelSurfaceSimple;
class VoxelMesher;

class VoxelmanLibrarySimple : public VoxelmanLibrary {
	GDCLASS(VoxelmanLibrarySimple, VoxelmanLibrary)

public:
	int get_atlas_columns() const;
	void set_atlas_columns(int s);

	int get_atlas_rows() const;
	void set_atlas_rows(int s);

	Ref<VoxelSurface> get_voxel_surface(int index) const;
	void set_voxel_surface(int index, Ref<VoxelSurface> value);
	void remove_surface(int index);
	int get_num_surfaces();
	void clear_surfaces();

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &surfaces);

	Ref<VoxelSurface> get_liquid_voxel_surface(int index) const;
	void set_liquid_voxel_surface(int index, Ref<VoxelSurface> value);
	void remove_liquid_surface(int index);
	int get_liquid_num_surfaces();
	void clear_liquid_surfaces();

	Vector<Variant> get_liquid_voxel_surfaces();
	void set_liquid_voxel_surfaces(const Vector<Variant> &surfaces);

	void refresh_rects();

	VoxelmanLibrarySimple();
	~VoxelmanLibrarySimple();

protected:
	static void _bind_methods();

private:
	Vector<Ref<VoxelSurfaceSimple> > _voxel_surfaces;
	Vector<Ref<VoxelSurfaceSimple> > _liquid_surfaces;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
};

#endif // VOXEL_LIBRARY_H
