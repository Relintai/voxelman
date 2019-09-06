#include "voxelman_library.h"

VoxelmanLibrary::VoxelmanLibrary() {

	_atlas_rows = 8;
	_atlas_columns = 8;

	_is_textured = true;
}

VoxelmanLibrary::~VoxelmanLibrary() {
	for (int i = 0; i < _voxel_surfaces.size(); ++i) {
		if (_voxel_surfaces[i].is_valid()) {
			_voxel_surfaces.get(i)->set_library(Ref<VoxelmanLibrary>(NULL));
			_voxel_surfaces.get(i).unref();
		}
	}

	_voxel_surfaces.clear();

	_material.unref();
	_prop_material.unref();
}

Ref<VoxelSurface> VoxelmanLibrary::get_voxel_surface(int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>(NULL));

	return _voxel_surfaces[index];
}

void VoxelmanLibrary::set_voxel_surface(int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_voxel_surfaces.size() < index) {
		_voxel_surfaces.resize(index + 1);
	}

	if (_voxel_surfaces[index].is_valid()) {
		_voxel_surfaces.get(index)->set_library(Ref<VoxelmanLibrary>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<VoxelmanLibrary>(this));

		_voxel_surfaces.set(index, Ref<VoxelSurface>(value));
	}
}

Vector<Variant> VoxelmanLibrary::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}

void VoxelmanLibrary::set_voxel_surfaces(const Vector<Variant> &effects) {
	_voxel_surfaces.clear();
	for (int i = 0; i < effects.size(); i++) {
		Ref<VoxelSurface> surface = Ref<VoxelSurface>(effects[i]);

		if (surface.is_valid())
			surface->set_library(this);

		_voxel_surfaces.push_back(surface);
	}
}

int VoxelmanLibrary::get_voxel_types_count() {
	return _voxel_surfaces.size();
}

int VoxelmanLibrary::get_voxel_count() const {
	return _voxel_surfaces.size();
}

void VoxelmanLibrary::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

void VoxelmanLibrary::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

bool VoxelmanLibrary::get_is_textured() const {
	return _is_textured;
}

void VoxelmanLibrary::set_is_textured(bool value) {
	_is_textured = value;
}

void VoxelmanLibrary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_surface", "id"), &VoxelmanLibrary::get_surface);

	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &VoxelmanLibrary::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &VoxelmanLibrary::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &VoxelmanLibrary::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &VoxelmanLibrary::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_is_textured"), &VoxelmanLibrary::get_is_textured);
	ClassDB::bind_method(D_METHOD("set_is_textured", "value"), &VoxelmanLibrary::set_is_textured);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_textured"), "set_is_textured", "get_is_textured");

	ClassDB::bind_method(D_METHOD("get_material"), &VoxelmanLibrary::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &VoxelmanLibrary::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_prop_material"), &VoxelmanLibrary::get_prop_material);
	ClassDB::bind_method(D_METHOD("set_prop_material", "value"), &VoxelmanLibrary::set_prop_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_prop_material", "get_prop_material");

	ClassDB::bind_method(D_METHOD("get_voxel_types_count"), &VoxelmanLibrary::get_voxel_types_count);

	ClassDB::bind_method(D_METHOD("get_voxel_surface", "index"), &VoxelmanLibrary::get_voxel_surface);
	ClassDB::bind_method(D_METHOD("set_voxel_surface", "index", "surface"), &VoxelmanLibrary::set_voxel_surface);

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &VoxelmanLibrary::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces"), &VoxelmanLibrary::set_voxel_surfaces);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", (PropertyHint)(PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_SCRIPT_VARIABLE), "17/17:VoxelSurface", PROPERTY_USAGE_DEFAULT, "VoxelSurface"), "set_voxel_surfaces", "get_voxel_surfaces");
}
