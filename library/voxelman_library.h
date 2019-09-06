#ifndef VOXELMAN_LIBRARY_H
#define VOXELMAN_LIBRARY_H

#include "core/resource.h"
#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface.h"

class VoxelSurface;
class VoxelMesher;

class VoxelmanLibrary : public Resource {
	GDCLASS(VoxelmanLibrary, Resource)

public:
	VoxelmanLibrary();
	~VoxelmanLibrary();

	int get_atlas_columns() const { return _atlas_columns; }
	void set_atlas_columns(int s);

	int get_atlas_rows() const { return _atlas_rows; }
	void set_atlas_rows(int s);

	bool get_is_textured() const;
	void set_is_textured(bool s);

	Ref<Material> get_material() const { return _material; }
	void set_material(Ref<Material> mat) { _material = mat; }

	Ref<Material> get_prop_material() const { return _prop_material; }
	void set_prop_material(Ref<Material> mat) { _prop_material = mat; }

	int get_voxel_count() const;

	Ref<VoxelSurface> get_voxel_surface(int index) const;
	void set_voxel_surface(int index, Ref<VoxelSurface> value);

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &effects);

	int get_voxel_types_count();

	_FORCE_INLINE_ bool has_voxel(int id) const { return _voxel_surfaces[id].is_valid(); }
	_FORCE_INLINE_ Ref<VoxelSurface> get_surface(int id) const { return _voxel_surfaces[id]; }

protected:
	static void _bind_methods();

private:
	Vector<Ref<VoxelSurface> > _voxel_surfaces;

	Ref<Material> _material;
	Ref<Material> _prop_material;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
	bool _is_textured;
};

#endif // VOXEL_LIBRARY_H
