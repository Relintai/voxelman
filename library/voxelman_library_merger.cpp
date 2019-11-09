#include "voxelman_library_merger.h"

int VoxelmanLibraryMerger::get_atlas_columns() const {
	return _atlas_columns;
}

void VoxelmanLibraryMerger::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

int VoxelmanLibraryMerger::get_atlas_rows() const {
	return _atlas_rows;
}

void VoxelmanLibraryMerger::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

Ref<VoxelSurface> VoxelmanLibraryMerger::get_voxel_surface(int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>(NULL));

	return _voxel_surfaces[index];
}

void VoxelmanLibraryMerger::set_voxel_surface(int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_voxel_surfaces.size() < index) {
		_voxel_surfaces.resize(index + 1);
	}

	if (_voxel_surfaces[index].is_valid()) {
		_voxel_surfaces.get(index)->set_library(Ref<VoxelmanLibraryMerger>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<VoxelmanLibraryMerger>(this));

		_voxel_surfaces.set(index, value);
	}
}

void VoxelmanLibraryMerger::remove_surface(int index) {
	_voxel_surfaces.remove(index);
}

int VoxelmanLibraryMerger::get_num_surfaces() {
	return _voxel_surfaces.size();
}

Vector<Variant> VoxelmanLibraryMerger::get_voxel_surfaces() {
	Vector<Variant> r;
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		r.push_back(_voxel_surfaces[i].get_ref_ptr());
	}
	return r;
}

void VoxelmanLibraryMerger::set_voxel_surfaces(const Vector<Variant> &surfaces) {
	_voxel_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<VoxelSurfaceMerger> surface = Ref<VoxelSurfaceMerger>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(this);
		}

		_voxel_surfaces.push_back(surface);
	}
}

void VoxelmanLibraryMerger::refresh_rects() {
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<VoxelSurfaceMerger> surface = Ref<VoxelSurfaceMerger>(_voxel_surfaces[i]);

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

VoxelmanLibraryMerger::VoxelmanLibraryMerger() {
	_atlas_rows = 8;
	_atlas_columns = 8;
}

VoxelmanLibraryMerger::~VoxelmanLibraryMerger() {
	for (int i = 0; i < _voxel_surfaces.size(); ++i) {
		Ref<VoxelSurface> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<VoxelmanLibraryMerger>());
			surface.unref();
		}
	}

	_voxel_surfaces.clear();
}


void VoxelmanLibraryMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &VoxelmanLibraryMerger::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &VoxelmanLibraryMerger::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &VoxelmanLibraryMerger::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &VoxelmanLibraryMerger::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &VoxelmanLibraryMerger::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces"), &VoxelmanLibraryMerger::set_voxel_surfaces);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurfaceMerger", PROPERTY_USAGE_DEFAULT, "VoxelSurfaceMerger"), "set_voxel_surfaces", "get_voxel_surfaces");
}
