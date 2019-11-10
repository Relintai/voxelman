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
	Ref<Material> get_material() const;
	void set_material(Ref<Material> mat);

	Ref<Material> get_prop_material() const;
	void set_prop_material(Ref<Material> mat);

	Ref<Material> get_liquid_material() const;
	void set_liquid_material(Ref<Material> mat);

	Ref<Material> get_clutter_material() const;
	void set_clutter_material(Ref<Material> mat);

	virtual Ref<VoxelSurface> get_voxel_surface(int index) const;
	virtual void set_voxel_surface(int index, Ref<VoxelSurface> value);
	virtual void remove_surface(int index);
	virtual int get_num_surfaces();
	virtual void clear_surfaces();

	virtual Ref<VoxelSurface> get_liquid_voxel_surface(int index) const;
	virtual void set_liquid_voxel_surface(int index, Ref<VoxelSurface> value);
	virtual void remove_liquid_surface(int index);
	virtual int get_liquid_num_surfaces();
	virtual void clear_liquid_surfaces();

	virtual void refresh_rects();

	VoxelmanLibrary();
	~VoxelmanLibrary();

protected:
	static void _bind_methods();

private:
	Ref<Material> _material;
	Ref<Material> _prop_material;
	Ref<Material> _liquid_material;
	Ref<Material> _clutter_material;
};

#endif // VOXEL_LIBRARY_H
