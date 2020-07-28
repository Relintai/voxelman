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
Ref<Material> VoxelmanLibrary::get_material(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<VoxelSurface>(NULL));

	return _materials[index];
}

void VoxelmanLibrary::add_material(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void VoxelmanLibrary::set_material(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void VoxelmanLibrary::remove_material(const int index) {
	_materials.remove(index);
}

int VoxelmanLibrary::get_num_materials() const {
	return _materials.size();
}

void VoxelmanLibrary::clear_materials() {
	_materials.clear();
}

Vector<Variant> VoxelmanLibrary::get_materials() {
	VARIANT_ARRAY_GET(_materials);
}

void VoxelmanLibrary::set_materials(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Liquid Materials
Ref<Material> VoxelmanLibrary::get_liquid_material(const int index) {
	ERR_FAIL_INDEX_V(index, _liquid_materials.size(), Ref<VoxelSurface>(NULL));

	return _liquid_materials[index];
}

void VoxelmanLibrary::add_liquid_material(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_liquid_materials.push_back(value);
}

void VoxelmanLibrary::set_liquid_material(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _liquid_materials.size());

	_liquid_materials.set(index, value);
}

void VoxelmanLibrary::remove_liquid_material(const int index) {
	_liquid_materials.remove(index);
}

int VoxelmanLibrary::get_num_liquid_materials() const {
	return _liquid_materials.size();
}

void VoxelmanLibrary::clear_liquid_materials() {
	_liquid_materials.clear();
}

Vector<Variant> VoxelmanLibrary::get_liquid_materials() {
	VARIANT_ARRAY_GET(_liquid_materials);
}

void VoxelmanLibrary::set_liquid_materials(const Vector<Variant> &materials) {
	_liquid_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_liquid_materials.push_back(material);
	}
}

//Prop Materials
Ref<Material> VoxelmanLibrary::get_prop_material(const int index) {
	ERR_FAIL_INDEX_V(index, _prop_materials.size(), Ref<VoxelSurface>(NULL));

	return _prop_materials[index];
}

void VoxelmanLibrary::add_prop_material(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_prop_materials.push_back(value);
}

void VoxelmanLibrary::set_prop_material(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _prop_materials.size());

	_prop_materials.set(index, value);
}

void VoxelmanLibrary::remove_prop_material(const int index) {
	_prop_materials.remove(index);
}

int VoxelmanLibrary::get_num_prop_materials() const {
	return _prop_materials.size();
}

void VoxelmanLibrary::clear_prop_materials() {
	_prop_materials.clear();
}

Vector<Variant> VoxelmanLibrary::get_prop_materials() {
	VARIANT_ARRAY_GET(_prop_materials);
}

void VoxelmanLibrary::set_prop_materials(const Vector<Variant> &materials) {
	VARIANT_ARRAY_SET(materials, _prop_materials, Material);
}

//Surfaces
Ref<VoxelSurface> VoxelmanLibrary::get_voxel_surface(const int index) {
	return Ref<VoxelSurface>();
}
void VoxelmanLibrary::add_voxel_surface(Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::set_voxel_surface(int index, Ref<VoxelSurface> value) {
}
void VoxelmanLibrary::remove_surface(const int index) {
}
int VoxelmanLibrary::get_num_surfaces() const {
	return 0;
}
void VoxelmanLibrary::clear_surfaces() {
}

Ref<PackedScene> VoxelmanLibrary::get_scene(const int id) {
	return Ref<PackedScene>();
}
void VoxelmanLibrary::add_scene(Ref<PackedScene> value) {
}
void VoxelmanLibrary::set_scene(int id, Ref<PackedScene> value) {
}
void VoxelmanLibrary::remove_scene(const int id) {
}
int VoxelmanLibrary::get_num_scenes() const {
	return 0;
}
void VoxelmanLibrary::clear_scenes() {
}

#ifdef PROPS_PRESENT
Ref<PropData> VoxelmanLibrary::get_prop(const int id) {
	return Ref<PropData>();
}
void VoxelmanLibrary::add_prop(Ref<PropData> value) {
}
bool VoxelmanLibrary::has_prop(const Ref<PropData> &value) const {
	return false;
}
void VoxelmanLibrary::set_prop(int id, Ref<PropData> value) {
}
void VoxelmanLibrary::remove_prop(const int id) {
}
int VoxelmanLibrary::get_num_props() const {
	return 0;
}
void VoxelmanLibrary::clear_props() {
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

	ClassDB::bind_method(D_METHOD("get_material", "index"), &VoxelmanLibrary::get_material);
	ClassDB::bind_method(D_METHOD("add_material", "value"), &VoxelmanLibrary::add_material);
	ClassDB::bind_method(D_METHOD("set_material", "index", "value"), &VoxelmanLibrary::set_material);
	ClassDB::bind_method(D_METHOD("remove_material", "index"), &VoxelmanLibrary::remove_material);
	ClassDB::bind_method(D_METHOD("get_num_materials"), &VoxelmanLibrary::get_num_materials);
	ClassDB::bind_method(D_METHOD("clear_materials"), &VoxelmanLibrary::clear_materials);

	ClassDB::bind_method(D_METHOD("get_materials"), &VoxelmanLibrary::get_materials);
	ClassDB::bind_method(D_METHOD("set_materials"), &VoxelmanLibrary::set_materials);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "set_materials", "get_materials");

	ClassDB::bind_method(D_METHOD("get_liquid_material", "index"), &VoxelmanLibrary::get_liquid_material);
	ClassDB::bind_method(D_METHOD("add_liquid_material", "value"), &VoxelmanLibrary::add_liquid_material);
	ClassDB::bind_method(D_METHOD("set_liquid_material", "index", "value"), &VoxelmanLibrary::set_liquid_material);
	ClassDB::bind_method(D_METHOD("remove_liquid_material", "index"), &VoxelmanLibrary::remove_liquid_material);
	ClassDB::bind_method(D_METHOD("get_num_liquid_materials"), &VoxelmanLibrary::get_num_liquid_materials);
	ClassDB::bind_method(D_METHOD("clear_liquid_materials"), &VoxelmanLibrary::clear_liquid_materials);

	ClassDB::bind_method(D_METHOD("get_liquid_materials"), &VoxelmanLibrary::get_liquid_materials);
	ClassDB::bind_method(D_METHOD("set_liquid_materials"), &VoxelmanLibrary::set_liquid_materials);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "liquid_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "set_liquid_materials", "get_liquid_materials");

	ClassDB::bind_method(D_METHOD("get_prop_material", "index"), &VoxelmanLibrary::get_prop_material);
	ClassDB::bind_method(D_METHOD("add_prop_material", "value"), &VoxelmanLibrary::add_prop_material);
	ClassDB::bind_method(D_METHOD("set_prop_material", "index", "value"), &VoxelmanLibrary::set_prop_material);
	ClassDB::bind_method(D_METHOD("remove_prop_material", "index"), &VoxelmanLibrary::remove_prop_material);
	ClassDB::bind_method(D_METHOD("get_num_prop_materials"), &VoxelmanLibrary::get_num_prop_materials);
	ClassDB::bind_method(D_METHOD("clear_prop_materials"), &VoxelmanLibrary::clear_prop_materials);

	ClassDB::bind_method(D_METHOD("get_prop_materials"), &VoxelmanLibrary::get_prop_materials);
	ClassDB::bind_method(D_METHOD("set_prop_materials"), &VoxelmanLibrary::set_prop_materials);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prop_materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "set_prop_materials", "get_prop_materials");

	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &VoxelmanLibrary::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("add_voxel_surface", "value"), &VoxelmanLibrary::add_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "surface"), &VoxelmanLibrary::set_voxel_surface);
	ClassDB::bind_method(D_METHOD("remove_surface", "index"), &VoxelmanLibrary::remove_surface);
	ClassDB::bind_method(D_METHOD("get_num_surfaces"), &VoxelmanLibrary::get_num_surfaces);
	ClassDB::bind_method(D_METHOD("clear_surfaces"), &VoxelmanLibrary::clear_surfaces);

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("get_prop", "id"), &VoxelmanLibrary::get_prop);
	ClassDB::bind_method(D_METHOD("add_prop", "value"), &VoxelmanLibrary::add_prop);
	ClassDB::bind_method(D_METHOD("has_prop", "prop"), &VoxelmanLibrary::has_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "id", "surface"), &VoxelmanLibrary::set_prop);
	ClassDB::bind_method(D_METHOD("remove_prop", "id"), &VoxelmanLibrary::remove_prop);
	ClassDB::bind_method(D_METHOD("get_num_props"), &VoxelmanLibrary::get_num_props);
	ClassDB::bind_method(D_METHOD("clear_props"), &VoxelmanLibrary::clear_props);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelmanLibrary::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "material_index", "texture"), &VoxelmanLibrary::setup_material_albedo);

	BIND_CONSTANT(MATERIAL_INDEX_VOXELS);
	BIND_CONSTANT(MATERIAL_INDEX_LIQUID);
	BIND_CONSTANT(MATERIAL_INDEX_PROP);
}
