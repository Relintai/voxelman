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

#include "voxelman_library.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"
#endif

#include "scene/resources/packed_scene.h"

#include "../defines.h"

bool VoxelmanLibrary::get_initialized() const {
	return _initialized;
}
void VoxelmanLibrary::set_initialized(const bool value) {
	_initialized = value;
}

//Materials
Ref<Material> VoxelmanLibrary::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<VoxelSurface>(NULL));

	return _materials[index];
}

void VoxelmanLibrary::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void VoxelmanLibrary::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void VoxelmanLibrary::material_remove(const int index) {
	_materials.remove(index);
}

int VoxelmanLibrary::material_get_num() const {
	return _materials.size();
}

void VoxelmanLibrary::materials_clear() {
	_materials.clear();
}

Vector<Variant> VoxelmanLibrary::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void VoxelmanLibrary::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> VoxelmanLibrary::liquid_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<VoxelSurface>(NULL));

	return _liquid_materials[index];
}

void VoxelmanLibrary::liquid_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void VoxelmanLibrary::liquid_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void VoxelmanLibrary::liquid_material_remove(const int index) {
	_liquid_materials.remove(index);
}

int VoxelmanLibrary::liquid_material_get_num() const {
	return _liquid_materials.size();
}

void VoxelmanLibrary::liquid_materials_clear() {
	_liquid_materials.clear();
}

Vector<Variant> VoxelmanLibrary::liquid_materials_get() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void VoxelmanLibrary::liquid_materials_set(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> VoxelmanLibrary::prop_material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<VoxelSurface>(NULL));

	return _prop_materials[index];
}

void VoxelmanLibrary::prop_material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void VoxelmanLibrary::prop_material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void VoxelmanLibrary::prop_material_remove(const int index) {
	_prop_materials.remove(index);
}

int VoxelmanLibrary::prop_material_get_num() const {
	return _prop_materials.size();
}

void VoxelmanLibrary::prop_materials_clear() {
	_prop_materials.clear();
}

Vector<Variant> VoxelmanLibrary::prop_materials_get() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void VoxelmanLibrary::prop_materials_set(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<VoxelSurface> VoxelmanLibrary::voxel_surface_get(const int index) {
	return Ref<VoxelSurface>();
}
void VoxelmanLibrary::voxel_surface_add(Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::voxel_surface_set(int index, Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::voxel_surface_remove(const int index) {
}
int VoxelmanLibrary::voxel_surface_get_num() const {
	return 0;
}
void VoxelmanLibrary::voxel_surfaces_clear() {
}

Ref<PackedScene> VoxelmanLibrary::scene_get(const int id) {
	return Ref<PackedScene>();
}
void VoxelmanLibrary::scene_add(Ref<PackedScene> value) {
}
void VoxelmanLibrary::scene_set(int id, Ref<PackedScene> value) {
}
void VoxelmanLibrary::scene_remove(const int id) {
}
int VoxelmanLibrary::scene_get_num() const {
	return 0;
}
void VoxelmanLibrary::scenes_clear() {
}

#ifdef PROPS_PRESENT
Ref<PropData> VoxelmanLibrary::prop_get(const int id) {
	return Ref<PropData>();
}
void VoxelmanLibrary::prop_add(Ref<PropData> value) {
}
bool VoxelmanLibrary::prop_has(const Ref<PropData> &value) const {
	return false;
}
void VoxelmanLibrary::prop_set(int id, Ref<PropData> value) {
}
void VoxelmanLibrary::prop_remove(const int id) {
}
int VoxelmanLibrary::prop_get_num() const {
	return 0;
}
void VoxelmanLibrary::props_clear() {
}

Rect2 VoxelmanLibrary::get_prop_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}
#endif

//Rects
void VoxelmanLibrary::refresh_rects() {
	_initialized = true;
}

void VoxelmanLibrary::setup_material_albedo(int material_index, Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", material_index, texture);
}

VoxelmanLibrary::VoxelmanLibrary() {
	_initialized = false;
}

VoxelmanLibrary::~VoxelmanLibrary() {
	_materials.clear();
	_liquid_materials.clear();
	_prop_materials.clear();
}

void VoxelmanLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &VoxelmanLibrary::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &VoxelmanLibrary::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized", PROPERTY_HINT_NONE, "", 0), "set_initialized", "get_initialized");

	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::INT, "material_index"), PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	ClassDB::bind_method(D_METHOD("material_get", "index"), &VoxelmanLibrary::material_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &VoxelmanLibrary::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &VoxelmanLibrary::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &VoxelmanLibrary::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &VoxelmanLibrary::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &VoxelmanLibrary::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &VoxelmanLibrary::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &VoxelmanLibrary::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("liquid_material_get", "index"), &VoxelmanLibrary::liquid_material_get);
	ClassDB::bind_method(D_METHOD("liquid_material_add", "value"), &VoxelmanLibrary::liquid_material_add);
	ClassDB::bind_method(D_METHOD("liquid_material_set", "index", "value"), &VoxelmanLibrary::liquid_material_set);
	ClassDB::bind_method(D_METHOD("liquid_material_remove", "index"), &VoxelmanLibrary::liquid_material_remove);
	ClassDB::bind_method(D_METHOD("liquid_material_get_num"), &VoxelmanLibrary::liquid_material_get_num);
	ClassDB::bind_method(D_METHOD("liquid_materials_clear"), &VoxelmanLibrary::liquid_materials_clear);

	ClassDB::bind_method(D_METHOD("liquid_materials_get"), &VoxelmanLibrary::liquid_materials_get);
	ClassDB::bind_method(D_METHOD("liquid_materials_set"), &VoxelmanLibrary::liquid_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "liquid_materials_set", "liquid_materials_get");

	ClassDB::bind_method(D_METHOD("prop_material_get", "index"), &VoxelmanLibrary::prop_material_get);
	ClassDB::bind_method(D_METHOD("prop_material_add", "value"), &VoxelmanLibrary::prop_material_add);
	ClassDB::bind_method(D_METHOD("prop_material_set", "index", "value"), &VoxelmanLibrary::prop_material_set);
	ClassDB::bind_method(D_METHOD("prop_material_remove", "index"), &VoxelmanLibrary::prop_material_remove);
	ClassDB::bind_method(D_METHOD("prop_material_get_num"), &VoxelmanLibrary::prop_material_get_num);
	ClassDB::bind_method(D_METHOD("prop_materials_clear"), &VoxelmanLibrary::prop_materials_clear);

	ClassDB::bind_method(D_METHOD("prop_materials_get"), &VoxelmanLibrary::prop_materials_get);
	ClassDB::bind_method(D_METHOD("prop_materials_set"), &VoxelmanLibrary::prop_materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "prop_materials_set", "prop_materials_get");

	ClassDB::bind_method(D_METHOD("voxel_surface_get", "index"), &VoxelmanLibrary::voxel_surface_get);
	ClassDB::bind_method(D_METHOD("voxel_surface_add", "value"), &VoxelmanLibrary::voxel_surface_add);
	ClassDB::bind_method(D_METHOD("voxel_surface_set", "index", "surface"), &VoxelmanLibrary::voxel_surface_set);
	ClassDB::bind_method(D_METHOD("voxel_surface_remove", "index"), &VoxelmanLibrary::voxel_surface_remove);
	ClassDB::bind_method(D_METHOD("voxel_surface_get_num"), &VoxelmanLibrary::voxel_surface_get_num);
	ClassDB::bind_method(D_METHOD("voxel_surfaces_clear"), &VoxelmanLibrary::voxel_surfaces_clear);

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &VoxelmanLibrary::scene_get);
	ClassDB::bind_method(D_METHOD("scene_add", "value"), &VoxelmanLibrary::scene_add);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "value"), &VoxelmanLibrary::scene_set);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &VoxelmanLibrary::scene_remove);
	ClassDB::bind_method(D_METHOD("scene_get_num"), &VoxelmanLibrary::scene_get_num);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &VoxelmanLibrary::scenes_clear);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("prop_get", "id"), &VoxelmanLibrary::prop_get);
	ClassDB::bind_method(D_METHOD("prop_add", "value"), &VoxelmanLibrary::prop_add);
	ClassDB::bind_method(D_METHOD("prop_has", "prop"), &VoxelmanLibrary::prop_has);
	ClassDB::bind_method(D_METHOD("prop_set", "id", "surface"), &VoxelmanLibrary::prop_set);
	ClassDB::bind_method(D_METHOD("prop_remove", "id"), &VoxelmanLibrary::prop_remove);
	ClassDB::bind_method(D_METHOD("prop_get_num"), &VoxelmanLibrary::prop_get_num);
	ClassDB::bind_method(D_METHOD("props_clear"), &VoxelmanLibrary::props_clear);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelmanLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &VoxelmanLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_VOXELS);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
