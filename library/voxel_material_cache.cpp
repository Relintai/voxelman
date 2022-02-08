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

#include "voxel_material_cache.h"

#include "../defines.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_prop.h"

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif
#endif

bool VoxelMaterialCache::get_initialized() {
	return _initialized;
}
void VoxelMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

int VoxelMaterialCache::get_ref_count() {
	return _ref_count;
}
void VoxelMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void VoxelMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void VoxelMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

//Materials
Ref<Material> VoxelMaterialCache::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> VoxelMaterialCache::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void VoxelMaterialCache::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void VoxelMaterialCache::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void VoxelMaterialCache::material_remove(const int index) {
	_materials.VREMOVE(index);
}

int VoxelMaterialCache::material_get_num() const {
	return _materials.size();
}

void VoxelMaterialCache::materials_clear() {
	_materials.clear();
}

Vector<Variant> VoxelMaterialCache::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void VoxelMaterialCache::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Surfaces
Ref<VoxelSurface> VoxelMaterialCache::surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _surfaces.size(), Ref<VoxelSurface>());

	return _surfaces[index];
}
Ref<VoxelSurface> VoxelMaterialCache::surface_id_get(const int id) {
	Ref<VoxelSurface> surface;

	for (int i = 0; i < _surfaces.size(); ++i) {
		surface = _surfaces[i];

		if (surface.is_valid()) {
			if (surface->get_id() == id) {
				break;
			}
		}
	}

	return surface;
}
void VoxelMaterialCache::surface_add(Ref<VoxelSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	_surfaces.push_back(value);
}
void VoxelMaterialCache::surface_set(int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_surfaces.size() < index) {
		_surfaces.resize(index + 1);
	}

	_surfaces.set(index, value);
}
void VoxelMaterialCache::surface_remove(const int index) {
	_surfaces.VREMOVE(index);
}
int VoxelMaterialCache::surface_get_num() const {
	return _surfaces.size();
}
void VoxelMaterialCache::surfaces_clear() {
	_surfaces.clear();
}

void VoxelMaterialCache::additional_texture_add(const Ref<Texture> &texture) {
	_additional_textures.push_back(texture);
}
void VoxelMaterialCache::additional_texture_remove(const Ref<Texture> &texture) {
	for (int i = 0; i < _additional_textures.size(); ++i) {
		if (_additional_textures[i] == texture) {
			_additional_textures.VREMOVE(i);
			return;
		}
	}
}
void VoxelMaterialCache::additional_texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _additional_textures.size());

	_additional_textures.VREMOVE(index);
}
void VoxelMaterialCache::additional_textures_clear() {
	_additional_textures.clear();
}
int VoxelMaterialCache::additional_texture_count() {
	return _additional_textures.size();
}
Ref<Texture> VoxelMaterialCache::additional_texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<Texture>());

	return _additional_textures[index];
}
Ref<AtlasTexture> VoxelMaterialCache::additional_texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<AtlasTexture>());

	return additional_texture_get_atlas_tex(_additional_textures[index]);
}
Ref<AtlasTexture> VoxelMaterialCache::additional_texture_get_atlas_tex(const Ref<Texture> &texture) {
	return Ref<AtlasTexture>();
}
Rect2 VoxelMaterialCache::additional_texture_get_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}

#ifdef PROPS_PRESENT
void VoxelMaterialCache::prop_add_textures(const Ref<PropData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			additional_texture_add(tex);
		}
#endif

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_add_textures(pdp);
		}
	}
}
void VoxelMaterialCache::prop_remove_textures(const Ref<PropData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			additional_texture_remove(tex);
		}
#endif

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_remove_textures(pdp);
		}
	}
}
#endif

void VoxelMaterialCache::refresh_rects() {
	_initialized = true;
}

void VoxelMaterialCache::setup_material_albedo(Ref<Texture> texture) {
#if VERSION_MAJOR < 4
	if (has_method("_setup_material_albedo")) {
		call("_setup_material_albedo", texture);
	}
#else
	GDVIRTUAL_CALL(_setup_material_albedo, texture);
#endif
}

VoxelMaterialCache::VoxelMaterialCache() {
	_ref_count = 0;
	_initialized = false;
}

VoxelMaterialCache::~VoxelMaterialCache() {
	_materials.clear();
	_surfaces.clear();
}

void VoxelMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &VoxelMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &VoxelMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_ref_count"), &VoxelMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &VoxelMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &VoxelMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &VoxelMaterialCache::dec_ref_count);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "texture");
#endif

	ClassDB::bind_method(D_METHOD("material_get", "index"), &VoxelMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &VoxelMaterialCache::material_lod_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &VoxelMaterialCache::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &VoxelMaterialCache::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &VoxelMaterialCache::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &VoxelMaterialCache::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &VoxelMaterialCache::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &VoxelMaterialCache::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &VoxelMaterialCache::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("surface_get", "index"), &VoxelMaterialCache::surface_get);
	ClassDB::bind_method(D_METHOD("surface_id_get", "index"), &VoxelMaterialCache::surface_id_get);

	ClassDB::bind_method(D_METHOD("surface_add", "value"), &VoxelMaterialCache::surface_add);
	ClassDB::bind_method(D_METHOD("surface_set", "index", "surface"), &VoxelMaterialCache::surface_set);
	ClassDB::bind_method(D_METHOD("surface_remove", "index"), &VoxelMaterialCache::surface_remove);
	ClassDB::bind_method(D_METHOD("surface_get_num"), &VoxelMaterialCache::surface_get_num);
	ClassDB::bind_method(D_METHOD("surfaces_clear"), &VoxelMaterialCache::surfaces_clear);

	ClassDB::bind_method(D_METHOD("additional_texture_add", "texture"), &VoxelMaterialCache::additional_texture_add);
	ClassDB::bind_method(D_METHOD("additional_texture_remove", "texture"), &VoxelMaterialCache::additional_texture_remove);
	ClassDB::bind_method(D_METHOD("additional_texture_remove_index", "index"), &VoxelMaterialCache::additional_texture_remove_index);
	ClassDB::bind_method(D_METHOD("additional_textures_clear"), &VoxelMaterialCache::additional_textures_clear);
	ClassDB::bind_method(D_METHOD("additional_texture_count"), &VoxelMaterialCache::additional_texture_count);
	ClassDB::bind_method(D_METHOD("additional_texture_get", "index"), &VoxelMaterialCache::additional_texture_get);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas", "index"), &VoxelMaterialCache::additional_texture_get_atlas);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas_tex", "index"), &VoxelMaterialCache::additional_texture_get_atlas_tex);
	ClassDB::bind_method(D_METHOD("additional_texture_get_uv_rect", "texture"), &VoxelMaterialCache::additional_texture_get_uv_rect);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add_textures", "prop"), &VoxelMaterialCache::prop_add_textures);
	ClassDB::bind_method(D_METHOD("prop_remove_textures", "prop"), &VoxelMaterialCache::prop_remove_textures);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &VoxelMaterialCache::setup_material_albedo);
}
