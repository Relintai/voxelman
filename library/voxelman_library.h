/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VOXELMAN_LIBRARY_H
#define VOXELMAN_LIBRARY_H

#include "core/resource.h"
#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface.h"

class VoxelSurface;
class VoxelMesher;
class PackedScene;

class VoxelmanLibrary : public Resource {
	GDCLASS(VoxelmanLibrary, Resource)

public:
	enum {
		MATERIAL_INDEX_VOXELS = 0,
		MATERIAL_INDEX_LIQUID = 1,
	};

public:
	bool get_initialized() const;
	void set_initialized(const bool value);

	Ref<Material> get_material(const int index);
	void add_material(const Ref<Material> &value);
	void set_material(const int index, const Ref<Material> &value);
	void remove_material(const int index);
	int get_num_materials() const;
	void clear_materials();

	Vector<Variant> get_materials();
	void set_materials(const Vector<Variant> &materials);

	Ref<Material> get_liquid_material(const int index);
	void add_liquid_material(const Ref<Material> &value);
	void set_liquid_material(const int index, const Ref<Material> &value);
	void remove_liquid_material(const int index);
	int get_num_liquid_materials() const;
	void clear_liquid_materials();

	Vector<Variant> get_liquid_materials();
	void set_liquid_materials(const Vector<Variant> &materials);

	virtual Ref<VoxelSurface> get_voxel_surface(const int index);
	virtual void add_voxel_surface(Ref<VoxelSurface> value);
	virtual void set_voxel_surface(const int index, Ref<VoxelSurface> value);
	virtual void remove_surface(const int index);
	virtual int get_num_surfaces() const;
	virtual void clear_surfaces();

	virtual Ref<PackedScene> get_prop(const int id);
	virtual void add_prop(Ref<PackedScene> value);
	virtual void set_prop(const int id, Ref<PackedScene> value);
	virtual void remove_prop(const int id);
	virtual int get_num_props() const;
	virtual void clear_props();

	virtual void refresh_rects();

	void setup_material_albedo(int material_index, Ref<Texture> texture);

	VoxelmanLibrary();
	~VoxelmanLibrary();

protected:
	static void _bind_methods();

private:
	bool _initialized;
	Vector<Ref<Material> > _materials;
	Vector<Ref<Material> > _liquid_materials;
};

#endif // VOXEL_LIBRARY_H
