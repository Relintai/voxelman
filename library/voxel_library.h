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

#ifndef VOXEL_LIBRARY_H
#define VOXEL_LIBRARY_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#else
#include "core/resource.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface.h"

#include "../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

class VoxelMaterialCache;
class VoxelChunk;
class VoxelSurface;
class VoxelMesher;
class PackedScene;
#ifdef PROPS_PRESENT
class PropData;
#endif

class VoxelLibrary : public Resource {
	GDCLASS(VoxelLibrary, Resource)

public:
	enum {
		MATERIAL_INDEX_VOXELS = 0,
		MATERIAL_INDEX_LIQUID = 1,
		MATERIAL_INDEX_PROP = 2,
	};

public:
	bool get_initialized() const;
	void set_initialized(const bool value);

	bool supports_caching();
	virtual bool _supports_caching();

	Ref<Material> material_get(const int index);
	Ref<Material> material_lod_get(const int index);

	void material_cache_get_key(Ref<VoxelChunk> chunk);
	virtual void _material_cache_get_key(Ref<VoxelChunk> chunk);
	Ref<VoxelMaterialCache> material_cache_get(const int key);
	virtual Ref<VoxelMaterialCache> _material_cache_get(const int key);
	void material_cache_unref(const int key);
	virtual void _material_cache_unref(const int key);

	void material_add(const Ref<Material> &value);
	void material_set(const int index, const Ref<Material> &value);
	void material_remove(const int index);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

	Ref<Material> liquid_material_get(const int index);
	Ref<Material> liquid_material_lod_get(const int index);

	void liquid_material_cache_get_key(Ref<VoxelChunk> chunk);
	virtual void _liquid_material_cache_get_key(Ref<VoxelChunk> chunk);
	Ref<VoxelMaterialCache> liquid_material_cache_get(const int key);
	virtual Ref<VoxelMaterialCache> _liquid_material_cache_get(const int key);
	void liquid_material_cache_unref(const int key);
	virtual void _liquid_material_cache_unref(const int key);

	void liquid_material_add(const Ref<Material> &value);
	void liquid_material_set(const int index, const Ref<Material> &value);
	void liquid_material_remove(const int index);
	int liquid_material_get_num() const;
	void liquid_materials_clear();

	Vector<Variant> liquid_materials_get();
	void liquid_materials_set(const Vector<Variant> &materials);

	Ref<Material> prop_material_get(const int index);
	Ref<Material> prop_material_lod_get(const int index);

	void prop_material_cache_get_key(Ref<VoxelChunk> chunk);
	virtual void _prop_material_cache_get_key(Ref<VoxelChunk> chunk);
	Ref<VoxelMaterialCache> prop_material_cache_get(const int key);
	virtual Ref<VoxelMaterialCache> _prop_material_cache_get(const int key);
	void prop_material_cache_unref(const int key);
	virtual void _prop_material_cache_unref(const int key);

	void prop_material_add(const Ref<Material> &value);
	void prop_material_set(const int index, const Ref<Material> &value);
	void prop_material_remove(const int index);
	int prop_material_get_num() const;
	void prop_materials_clear();

	Vector<Variant> prop_materials_get();
	void prop_materials_set(const Vector<Variant> &materials);

	virtual Ref<VoxelSurface> voxel_surface_get(const int index);
	virtual void voxel_surface_add(Ref<VoxelSurface> value);
	virtual void voxel_surface_set(const int index, Ref<VoxelSurface> value);
	virtual void voxel_surface_remove(const int index);
	virtual int voxel_surface_get_num() const;
	virtual void voxel_surfaces_clear();

	virtual Ref<PackedScene> scene_get(const int id);
	virtual void scene_add(Ref<PackedScene> value);
	virtual void scene_set(const int id, Ref<PackedScene> value);
	virtual void scene_remove(const int id);
	virtual int scene_get_num() const;
	virtual void scenes_clear();

#ifdef PROPS_PRESENT
	virtual Ref<PropData> prop_get(const int id);
	virtual void prop_add(Ref<PropData> value);
	virtual bool prop_has(const Ref<PropData> &value) const;
	virtual void prop_set(const int id, Ref<PropData> value);
	virtual void prop_remove(const int id);
	virtual int prop_get_num() const;
	virtual void props_clear();

	virtual Rect2 get_prop_uv_rect(const Ref<Texture> &texture);
#endif

	virtual void refresh_rects();

	void setup_material_albedo(int material_index, Ref<Texture> texture);


#if VERSION_MAJOR >= 4
	GDVIRTUAL2(_setup_material_albedo, int, Ref<Texture>);
#endif

	VoxelLibrary();
	~VoxelLibrary();

protected:
	static void _bind_methods();

	bool _initialized;
	Vector<Ref<Material> > _materials;
	Vector<Ref<Material> > _liquid_materials;
	Vector<Ref<Material> > _prop_materials;
};

#endif // VOXEL_LIBRARY_H
