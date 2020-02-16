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
class PropData;

class VoxelmanLibrary : public Resource {
	GDCLASS(VoxelmanLibrary, Resource)

public:
	enum {
		MATERIAL_INDEX_VOXELS = 0,
		MATERIAL_INDEX_PROP = 1,
		MATERIAL_INDEX_LIQUID = 2,
		MATERIAL_INDEX_CLUTTER = 3,
	};

public:
	Ref<Material> get_material() const;
	void set_material(Ref<Material> mat);

	Ref<Material> get_prop_material() const;
	void set_prop_material(Ref<Material> mat);

	Ref<Material> get_liquid_material() const;
	void set_liquid_material(Ref<Material> mat);

	Ref<Material> get_clutter_material() const;
	void set_clutter_material(Ref<Material> mat);

	virtual Ref<VoxelSurface> get_voxel_surface(const int index);
	virtual void add_voxel_surface(Ref<VoxelSurface> value);
	virtual void set_voxel_surface(const int index, Ref<VoxelSurface> value);
	virtual void remove_surface(const int index);
	virtual int get_num_surfaces() const;
	virtual void clear_surfaces();

	virtual Ref<VoxelSurface> get_liquid_surface(const int index);
	virtual void add_liquid_surface(Ref<VoxelSurface> value);
	virtual void set_liquid_surface(const int index, Ref<VoxelSurface> value);
	virtual void remove_liquid_surface(const int index);
	virtual int get_num_liquid_surfaces() const;
	virtual void clear_liquid_surfaces();

	virtual Ref<PropData> get_prop(const int id);
	virtual void add_prop(Ref<PropData> value);
	virtual void set_prop(const int id, Ref<PropData> value);
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
	Ref<Material> _material;
	Ref<Material> _prop_material;
	Ref<Material> _liquid_material;
	Ref<Material> _clutter_material;
};

#endif // VOXEL_LIBRARY_H
