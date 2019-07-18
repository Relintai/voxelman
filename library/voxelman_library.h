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

	Ref<VoxelSurface> create_voxel(int id, String name);

	int get_voxel_count() const;

	void load_default();

	Ref<VoxelSurface> get_voxel_surface(int index) const;
	void set_voxel_surface(int index, Ref<VoxelSurface> value);

	int get_voxel_types_count();
	void set_voxel_types_count(int value);

	int get_voxel_types_page();
	void set_voxel_types_page(int value);

	_FORCE_INLINE_ bool has_voxel(int id) const { return _voxel_types[id].is_valid(); }
	_FORCE_INLINE_ const VoxelSurface &get_voxel_const(int id) const { return **_voxel_types[id]; }
	_FORCE_INLINE_ Ref<VoxelSurface> get_surface(int id) { return Ref<VoxelSurface>(*_voxel_types[id]); }

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;

	Ref<VoxelSurface> _get_voxel_bind(int id);

private:
	enum {
		MAX_VOXEL_TYPES = 256,
		ITEMS_PER_PAGE = 10,
	};

	int _voxel_types_count;
	int _voxel_types_page;
	Ref<VoxelSurface> _voxel_types[MAX_VOXEL_TYPES];

	Ref<Material> _material;
	Ref<Material> _prop_material;

	//atlas
	int _atlas_columns;
	int _atlas_rows;
	bool _is_textured;
};

#endif // VOXEL_LIBRARY_H
