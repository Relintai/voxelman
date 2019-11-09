#include "voxelman_library_simple.h"

int VoxelmanLibrarySimple::get_atlas_columns() const {
	return _atlas_columns;
}

void VoxelmanLibrarySimple::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

int VoxelmanLibrarySimple::get_atlas_rows() const {
	return _atlas_rows;
}

void VoxelmanLibrarySimple::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

Ref<VoxelSurface> VoxelmanLibrarySimple::get_voxel_surface(int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>(NULL));

	return _voxel_surfaces[index];
}

void VoxelmanLibrarySimple::set_voxel_surface(int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_voxel_surfaces.size() < index) {
		_voxel_surfaces.resize(index + 1);
	}

	if (_voxel_surfaces[index].is_valid()) {
		_voxel_surfaces.get(index)->set_library(Ref<VoxelmanLibrarySimple>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<VoxelmanLibrarySimple>(this));

		_voxel_surfaces.set(index, value);
	}
}

void VoxelmanLibrarySimple::remove_surface(int index) {
	_voxel_surfaces.remove(index);
}

int VoxelmanLibrarySimple::get_num_surfaces() {
	return _voxel_surfaces.size();
}

Vector<Variant> VoxelmanLibrarySimple::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}

void VoxelmanLibrarySimple::set_voxel_surfaces(const Vector<Variant> &surfaces) {
	_voxel_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<VoxelSurfaceSimple> surface = Ref<VoxelSurfaceSimple>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(this);
		}

		_voxel_surfaces.push_back(surface);
	}
}

void VoxelmanLibrarySimple::refresh_rects() {
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<VoxelSurfaceSimple> surface = Ref<VoxelSurfaceSimple>(_voxel_surfaces[i]);

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

VoxelmanLibrarySimple::VoxelmanLibrarySimple() {
	_atlas_rows = 8;
	_atlas_columns = 8;
}

VoxelmanLibrarySimple::~VoxelmanLibrarySimple() {
	for (int i = 0; i < _voxel_surfaces.size(); ++i) {
		Ref<VoxelSurface> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<VoxelmanLibrarySimple>());
			surface.unref();
		}
	}

	_voxel_surfaces.clear();
}


void VoxelmanLibrarySimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &VoxelmanLibrarySimple::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &VoxelmanLibrarySimple::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &VoxelmanLibrarySimple::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &VoxelmanLibrarySimple::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &VoxelmanLibrarySimple::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces"), &VoxelmanLibrarySimple::set_voxel_surfaces);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurfaceSimple", PROPERTY_USAGE_DEFAULT, "VoxelSurfaceSimple"), "set_voxel_surfaces", "get_voxel_surfaces");
}
