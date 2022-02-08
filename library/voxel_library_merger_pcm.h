/*
Copyright (c) 2019-2022 Péter Magyar

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

#ifndef VOXEL_LIBRARY_MERGER_PCM_H
#define VOXEL_LIBRARY_MERGER_PCM_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/templates/map.h"
#else
#include "core/resource.h"
#include "core/map.h"
#endif

#include "voxel_library.h"

#include "scene/resources/material.h"

#include "../data/voxel_light.h"
#include "voxel_surface_merger.h"

#include "core/os/mutex.h"

class VoxelSurfaceSimple;
class VoxelMesher;
class PackedScene;
class VoxelMaterialCache;
class VoxelMaterialCachePCM;
class TexturePacker;
class VoxelChunk;

//pcm = per chunk material
class VoxelLibraryMergerPCM : public VoxelLibrary {
	GDCLASS(VoxelLibraryMergerPCM, VoxelLibrary);

public:
	bool _supports_caching();

	void _material_cache_get_key(Ref<VoxelChunk> chunk);
	Ref<VoxelMaterialCache> _material_cache_get(const int key);
	void _material_cache_unref(const int key);

	void _prop_material_cache_get_key(Ref<VoxelChunk> chunk);
	Ref<VoxelMaterialCache> _prop_material_cache_get(const int key);
	void _prop_material_cache_unref(const int key);

	int get_texture_flags() const;
	void set_texture_flags(const int flags);

	int get_max_atlas_size() const;
	void set_max_atlas_size(const int size);

	bool get_keep_original_atlases() const;
	void set_keep_original_atlases(const bool value);

	Color get_background_color() const;
	void set_background_color(const Color &color);

	int get_margin() const;
	void set_margin(const int margin);

	Ref<VoxelSurface> voxel_surface_get(const int index);
	void voxel_surface_add(Ref<VoxelSurface> value);
	void voxel_surface_set(const int index, Ref<VoxelSurface> value);
	void voxel_surface_remove(const int index);
	int voxel_surface_get_num() const;
	void voxel_surfaces_clear();

	Vector<Variant> get_voxel_surfaces();
	void set_voxel_surfaces(const Vector<Variant> &surfaces);

#ifdef PROPS_PRESENT
	Ref<PropData> get_prop(const int index);
	void add_prop(Ref<PropData> value);
	bool has_prop(const Ref<PropData> &value) const;
	void set_prop(const int index, const Ref<PropData> &value);
	void remove_prop(const int index);
	int get_num_props() const;
	void clear_props();

	Vector<Variant> get_props();
	void set_props(const Vector<Variant> &props);

	Rect2 get_prop_uv_rect(const Ref<Texture> &texture);

	Ref<TexturePacker> get_prop_packer();
#endif

	void refresh_rects();

	void _setup_material_albedo(const int material_index, const Ref<Texture> &texture);

	VoxelLibraryMergerPCM();
	~VoxelLibraryMergerPCM();

protected:
#ifdef PROPS_PRESENT
	bool process_prop_textures(Ref<PropData> prop);
#endif

	static void _bind_methods();

	Map<int, Ref<VoxelMaterialCachePCM> > _material_cache;
	Map<int, Ref<VoxelMaterialCachePCM> > _prop_material_cache;

	Vector<Ref<VoxelSurfaceMerger> > _voxel_surfaces;
#ifdef PROPS_PRESENT
	Vector<Ref<PropData> > _props;
#endif

	//todo remove these
	Ref<TexturePacker> _packer;
	Ref<TexturePacker> _prop_packer;

	Mutex _material_cache_mutex;
	Mutex _prop_material_cache_mutex;
};

#endif
