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

#include "voxel_surface_merger.h"

#include "voxel_library_merger.h"

#include "../defines.h"

#if GODOT4
#define Texture Texture2D
#endif

Ref<AtlasTexture> VoxelSurfaceMerger::get_region(const VoxelSurfaceSides side) {
	return _regions[side];
}
void VoxelSurfaceMerger::set_region(const VoxelSurfaceSides side, const Ref<AtlasTexture> &texture) {
	_regions[side] = texture;
}

Ref<Texture> VoxelSurfaceMerger::get_texture(const VoxelSurfaceSides side) {
	return _textures[side];
}
void VoxelSurfaceMerger::set_texture(const VoxelSurfaceSides side, const Ref<Texture> &texture) {
	_textures[side] = texture;
}

void VoxelSurfaceMerger::refresh_rects() {
	for (int i = 0; i < VOXEL_SIDES_COUNT; ++i) {
		if (!_regions[i].is_valid()) {
			_rects[i] = Rect2();
			continue;
		}

		Ref<AtlasTexture> at = _regions[i];
		Ref<Texture> tex = at->get_atlas();

		if (!tex.is_valid()) {
			_rects[i] = Rect2();
			continue;
		}

		Rect2 region = at->get_region();
		float w = tex->get_width();
		float h = tex->get_height();

		Rect2 r;

		r.position.x = region.position.x / w;
		r.position.y = region.position.y / h;
		r.size.x = region.size.x / w;
		r.size.y = region.size.y / h;

		_rects[i] = r;
	}
}

VoxelSurfaceMerger::VoxelSurfaceMerger() {
	for (int i = 0; i < VOXEL_SIDES_COUNT; ++i) {
		_regions[i].unref();
		_textures[i].unref();
	}
}

VoxelSurfaceMerger::~VoxelSurfaceMerger() {
}

void VoxelSurfaceMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_region", "side"), &VoxelSurfaceMerger::get_region);
	ClassDB::bind_method(D_METHOD("set_region", "side", "texture"), &VoxelSurfaceMerger::set_region);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_top", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", VOXEL_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_bottom", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", VOXEL_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_side", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", VOXEL_SIDE_SIDE);

	ClassDB::bind_method(D_METHOD("get_texture", "side"), &VoxelSurfaceMerger::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "side", "texture"), &VoxelSurfaceMerger::set_texture);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_top", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", VOXEL_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_bottom", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", VOXEL_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_side", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", VOXEL_SIDE_SIDE);
}
