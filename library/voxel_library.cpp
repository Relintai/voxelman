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

#include "voxel_library.h"

#include "../world/voxel_chunk.h"
#include "voxel_material_cache.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

#include "scene/resources/packed_scene.h"

#include "../defines.h"

bool VoxelLibrary::get_initialized() const {
	return _initialized;
}
void VoxelLibrary::set_initialized(const bool value) {
	_initialized = value;
}

bool VoxelLibrary::supports_caching() {
	return call("_supports_caching");
}
bool VoxelLibrary::_supports_caching() {
	return false;
}


//Materials
Ref<Material> VoxelLibrary::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>());

	return _materials[index];
}

Ref<Material> VoxelLibrary::material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one material to VoxelLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void VoxelLibrary::material_cache_get_key(Ref<VoxelChunk> chunk) {
	call("_material_cache_get_key", chunk);
}

void VoxelLibrary::_material_cache_get_key(Ref<VoxelChunk> chunk) {
}

Ref<VoxelMaterialCache> VoxelLibrary::material_cache_get(const int key) {
	return call("_material_cache_get", key);
}

Ref<VoxelMaterialCache> VoxelLibrary::_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<VoxelMaterialCache>(), "This VoxelLibrary doesn't support cached materials!");
}

void VoxelLibrary::material_cache_unref(const int key) {
	call("_material_cache_unref", key);
}
void VoxelLibrary::_material_cache_unref(const int key) {
}

void VoxelLibrary::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void VoxelLibrary::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void VoxelLibrary::material_remove(const int index) {
#if VERSION_MAJOR <= 3
	_materials.remove(index);
#else
	_materials.remove_at(index);
#endif
}

int VoxelLibrary::material_get_num() const {
	return _materials.size();
}

void VoxelLibrary::materials_clear() {
	_materials.clear();
}

Vector<Variant> VoxelLibrary::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void VoxelLibrary::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> VoxelLibrary::liquid_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<Material>());

	return _liquid_materials[index];
}

Ref<Material> VoxelLibrary::liquid_material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one liquid to VoxelLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _liquid_materials[0];
	}

	if (index >= _liquid_materials.size()) {
		return _liquid_materials[_liquid_materials.size() - 1];
	}

	return _liquid_materials[index];
}

void VoxelLibrary::liquid_material_cache_get_key(Ref<VoxelChunk> chunk) {
	call("_liquid_material_cache_get_key", chunk);
}

void VoxelLibrary::_liquid_material_cache_get_key(Ref<VoxelChunk> chunk) {
}

Ref<VoxelMaterialCache> VoxelLibrary::liquid_material_cache_get(const int key) {
	return call("_liquid_material_cache_get", key);
}

Ref<VoxelMaterialCache> VoxelLibrary::_liquid_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<VoxelMaterialCache>(), "This VoxelLibrary doesn't support cached liquid materials!");
}

void VoxelLibrary::liquid_material_cache_unref(const int key) {
	call("_liquid_material_cache_unref", key);
}
void VoxelLibrary::_liquid_material_cache_unref(const int key) {
}


void VoxelLibrary::liquid_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void VoxelLibrary::liquid_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void VoxelLibrary::liquid_material_remove(const int index) {
#if VERSION_MAJOR <= 3
	_liquid_materials.remove(index);
#else
	_liquid_materials.remove_at(index);
#endif
}

int VoxelLibrary::liquid_material_get_num() const {
	return _liquid_materials.size();
}

void VoxelLibrary::liquid_materials_clear() {
	_liquid_materials.clear();
}

Vector<Variant> VoxelLibrary::liquid_materials_get() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void VoxelLibrary::liquid_materials_set(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> VoxelLibrary::prop_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<Material>());

	return _prop_materials[index];
}

Ref<Material> VoxelLibrary::prop_material_lod_get(const int index) {
	ERR_FAIL_COND_V_MSG(_prop_materials.size() == 0, Ref<Material>(), "Error! You should to add at least one prop material to VoxelLibrary! (By default it will use up to 5. See VoxelWorldDefault::_create_chunk().)");

	if (index < 0) {
		return _prop_materials[0];
	}

	if (index >= _prop_materials.size()) {
		return _prop_materials[_prop_materials.size() - 1];
	}

	return _prop_materials[index];
}

void VoxelLibrary::prop_material_cache_get_key(Ref<VoxelChunk> chunk) {
	call("_prop_material_cache_get_key", chunk);
}

void VoxelLibrary::_prop_material_cache_get_key(Ref<VoxelChunk> chunk) {
}

Ref<VoxelMaterialCache> VoxelLibrary::prop_material_cache_get(const int key) {
	return call("_prop_material_cache_get", key);
}

Ref<VoxelMaterialCache> VoxelLibrary::_prop_material_cache_get(const int key) {
	ERR_FAIL_V_MSG(Ref<VoxelMaterialCache>(), "This VoxelLibrary doesn't support cached prop materials!");
}

void VoxelLibrary::prop_material_cache_unref(const int key) {
	call("_prop_material_cache_unref", key);
}
void VoxelLibrary::_prop_material_cache_unref(const int key) {
}


void VoxelLibrary::prop_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void VoxelLibrary::prop_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void VoxelLibrary::prop_material_remove(const int index) {
#if VERSION_MAJOR <= 3
	_prop_materials.remove(index);
#else
	_prop_materials.remove_at(index);
#endif
}

int VoxelLibrary::prop_material_get_num() const {
	return _prop_materials.size();
}

void VoxelLibrary::prop_materials_clear() {
	_prop_materials.clear();
}

Vector<Variant> VoxelLibrary::prop_materials_get() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void VoxelLibrary::prop_materials_set(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<VoxelSurface> VoxelLibrary::voxel_surface_get(const int index) {
	return Ref<VoxelSurface>();
}
void VoxelLibrary::voxel_surface_add(Ref<VoxelSurface> value) {
}
void VoxelLibrary::voxel_surface_set(int index, Ref<VoxelSurface> value) {
}
void VoxelLibrary::voxel_surface_remove(const int index) {
}
int VoxelLibrary::voxel_surface_get_num() const {
	return 0;
}
void VoxelLibrary::voxel_surfaces_clear() {
}

Ref<PackedScene> VoxelLibrary::scene_get(const int id) {
	return Ref<PackedScene>();
}
void VoxelLibrary::scene_add(Ref<PackedScene> value) {
}
void VoxelLibrary::scene_set(int id, Ref<PackedScene> value) {
}
void VoxelLibrary::scene_remove(const int id) {
}
int VoxelLibrary::scene_get_num() const {
	return 0;
}
void VoxelLibrary::scenes_clear() {
}

#ifdef PROPS_PRESENT
Ref<PropData> VoxelLibrary::prop_get(const int id) {
	return Ref<PropData>();
}
void VoxelLibrary::prop_add(Ref<PropData> value) {
}
bool VoxelLibrary::prop_has(const Ref<PropData> &value) const {
	return false;
}
void VoxelLibrary::prop_set(int id, Ref<PropData> value) {
}
void VoxelLibrary::prop_remove(const int id) {
}
int VoxelLibrary::prop_get_num() const {
	return 0;
}
void VoxelLibrary::props_clear() {
}

Rect2 VoxelLibrary::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void VoxelLibrary::refresh_rects() {
	_initialized = true;
}

void VoxelLibrary::setup_material_albedo(int material_index, Ref<Texture> texture) {
#if VERSION_MAJOR < 4
	if (has_method("_setup_material_albedo")) {
		call("_setup_material_albedo", material_index, texture);
	}
#else
	GDVIRTUAL_CALL(_setup_material_albedo, material_index, texture);
#endif
}

VoxelLibrary::VoxelLibrary() {
	_initialized = false;
}

VoxelLibrary::~VoxelLibrary() {
	_materials.clear();
	_liquid_materials.clear();
	_prop_materials.clear();
}

void VoxelLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &VoxelLibrary::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &VoxelLibrary::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_supports_caching"));
#else
	GDVIRTUAL_BIND(_supports_caching);
#endif

	ClassDB::bind_method(D_METHOD("_supports_caching"), &VoxelLibrary::_supports_caching);
	ClassDB::bind_method(D_METHOD("supports_caching"), &VoxelLibrary::supports_caching);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "material_index", "texture");
#endif

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_material_cache_get, "key");
	GDVIRTUAL_BIND(_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("material_get", "index"), &VoxelLibrary::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &VoxelLibrary::material_lod_get);

	ClassDB::bind_method(D_METHOD("material_cache_get_key", "chunk"), &VoxelLibrary::material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_material_cache_get_key", "chunk"), &VoxelLibrary::_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("material_cache_get", "key"), &VoxelLibrary::material_cache_get);
	ClassDB::bind_method(D_METHOD("_material_cache_get", "key"), &VoxelLibrary::_material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "key"), &VoxelLibrary::material_cache_unref);
	ClassDB::bind_method(D_METHOD("_material_cache_unref", "key"), &VoxelLibrary::_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_add", "value"), &VoxelLibrary::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &VoxelLibrary::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &VoxelLibrary::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &VoxelLibrary::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &VoxelLibrary::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &VoxelLibrary::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &VoxelLibrary::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_liquid_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_liquid_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_liquid_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_liquid_material_cache_get, "key");
	GDVIRTUAL_BIND(_liquid_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("liquid_material_get", "index"), &VoxelLibrary::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_lod_get", "index"), &VoxelLibrary::liquid_material_lod_get);

	ClassDB::bind_method(D_METHOD("liquid_material_cache_get_key", "chunk"), &VoxelLibrary::liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get_key", "chunk"), &VoxelLibrary::_liquid_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_get", "key"), &VoxelLibrary::liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_get", "key"), &VoxelLibrary::_liquid_material_cache_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_unref", "key"), &VoxelLibrary::liquid_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_liquid_material_cache_unref", "key"), &VoxelLibrary::_liquid_material_cache_unref);

	ClassDB::bind_method(D_METHOD("liquid_material_add", "value"), &VoxelLibrary::liquid_material_add);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "index", "value"), &VoxelLibrary::liquid_material_set);
	ClassDB::bind_method(D_METHOD("liquid_material_remove", "index"), &VoxelLibrary::liquid_material_remove);
	ClassDB::bind_method(D_METHOD("liquid_material_get_num"), &VoxelLibrary::liquid_material_get_num);
	ClassDB::bind_method(D_METHOD("liquid_materials_clear"), &VoxelLibrary::liquid_materials_clear);

	ClassDB::bind_method(D_METHOD("liquid_materials_get"), &VoxelLibrary::liquid_materials_get);
	ClassDB::bind_method(D_METHOD("liquid_materials_set"), &VoxelLibrary::liquid_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "liquid_materials_set", "liquid_materials_get");

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_prop_material_cache_get_key", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "TerrainMaterialCache"), "_prop_material_cache_get", PropertyInfo(Variant::INT, "key")));
	BIND_VMETHOD(MethodInfo("_prop_material_cache_unref", PropertyInfo(Variant::INT, "key")));
#else
	GDVIRTUAL_BIND(_prop_material_cache_get_key, "chunk", "texture");
	GDVIRTUAL_BIND(_prop_material_cache_get, "key");
	GDVIRTUAL_BIND(_prop_material_cache_unref, "key");
#endif

	ClassDB::bind_method(D_METHOD("prop_material_get", "index"), &VoxelLibrary::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_lod_get", "index"), &VoxelLibrary::prop_material_lod_get);

	ClassDB::bind_method(D_METHOD("prop_material_cache_get_key", "chunk"), &VoxelLibrary::prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get_key", "chunk"), &VoxelLibrary::_prop_material_cache_get_key);
	ClassDB::bind_method(D_METHOD("prop_material_cache_get", "key"), &VoxelLibrary::prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_get", "key"), &VoxelLibrary::_prop_material_cache_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_unref", "key"), &VoxelLibrary::prop_material_cache_unref);
	ClassDB::bind_method(D_METHOD("_prop_material_cache_unref", "key"), &VoxelLibrary::_prop_material_cache_unref);

	ClassDB::bind_method(D_METHOD("prop_material_add", "value"), &VoxelLibrary::prop_material_add);
	ClassDB::bind_method(D_METHOD("prop_material_set", "index", "value"), &VoxelLibrary::prop_material_set);
	ClassDB::bind_method(D_METHOD("prop_material_remove", "index"), &VoxelLibrary::prop_material_remove);
	ClassDB::bind_method(D_METHOD("prop_material_get_num"), &VoxelLibrary::prop_material_get_num);
	ClassDB::bind_method(D_METHOD("prop_materials_clear"), &VoxelLibrary::prop_materials_clear);

	ClassDB::bind_method(D_METHOD("prop_materials_get"), &VoxelLibrary::prop_materials_get);
	ClassDB::bind_method(D_METHOD("prop_materials_set"), &VoxelLibrary::prop_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "prop_materials_set", "prop_materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &VoxelLibrary::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &VoxelLibrary::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &VoxelLibrary::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &VoxelLibrary::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &VoxelLibrary::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &VoxelLibrary::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &VoxelLibrary::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &VoxelLibrary::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &VoxelLibrary::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &VoxelLibrary::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &VoxelLibrary::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &VoxelLibrary::scenes_clear);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &VoxelLibrary::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &VoxelLibrary::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &VoxelLibrary::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &VoxelLibrary::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &VoxelLibrary::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &VoxelLibrary::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &VoxelLibrary::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &VoxelLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_VOXELS);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
