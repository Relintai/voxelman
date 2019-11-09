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
	int get_texture_flags() const;
	void set_texture_flags(const int flags);

	int get_max_atlas_size() const;
	void set_max_atlas_size(const int size);

	bool get_keep_original_atlases() const;
	void set_keep_original_atlases(const bool value);

	Color get_background_color() const;
	void set_background_color(const Color color);

	int get_margin() const;
	void set_margin(const int margin);

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

	VoxelmanLibraryMerger();
	~VoxelmanLibraryMerger();

protected:
	static void _bind_methods();

private:
	Vector<Ref<VoxelSurfaceMerger> > _voxel_surfaces;
	Vector<Ref<VoxelSurfaceMerger> > _liquid_surfaces;

	Ref<TexturePacker> _packer;
};

#endif
