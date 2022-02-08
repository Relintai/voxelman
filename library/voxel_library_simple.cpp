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

#include "voxel_library_simple.h"

#include "../defines.h"

int VoxelLibrarySimple::get_atlas_columns() const {
	return _atlas_columns;
}

void VoxelLibrarySimple::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

int VoxelLibrarySimple::get_atlas_rows() const {
	return _atlas_rows;
}

void VoxelLibrarySimple::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

//Surfaces
Ref<VoxelSurface> VoxelLibrarySimple::voxel_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _voxel_surfaces.size(), Ref<VoxelSurface>(NULL));

	return _voxel_surfaces[index];
}

void VoxelLibrarySimple::voxel_surface_add(Ref<VoxelSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<VoxelLibrarySimple>(this));
	value->set_id(_voxel_surfaces.size());

	_voxel_surfaces.push_back(value);
}

void VoxelLibrarySimple::voxel_surface_set(const int index, Ref<VoxelSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_voxel_surfaces.size() < index) {
		_voxel_surfaces.resize(index + 1);
	}

	if (_voxel_surfaces[index].is_valid()) {
		_voxel_surfaces.get(index)->set_library(Ref<VoxelLibrarySimple>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<VoxelLibrarySimple>(this));

		_voxel_surfaces.set(index, value);
	}
}

void VoxelLibrarySimple::voxel_surface_remove(const int index) {
#if VERSION_MAJOR <= 3
	_voxel_surfaces.remove(index);
#else
	_voxel_surfaces.remove_at(index);
#endif
}

int VoxelLibrarySimple::voxel_surface_get_num() const {
	return _voxel_surfaces.size();
}

void VoxelLibrarySimple::voxel_surfaces_clear() {
	_voxel_surfaces.clear();
}

Vector<Variant> VoxelLibrarySimple::get_voxel_surfaces() {
	VARIANT_ARRAY_GET(_voxel_surfaces);
}

void VoxelLibrarySimple::set_voxel_surfaces(const Vector<Variant> &surfaces) {
	_voxel_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<VoxelSurfaceSimple> surface = Ref<VoxelSurfaceSimple>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(this);
			surface->refresh_rects();
		}

		_voxel_surfaces.push_back(surface);
	}

	set_initialized(true);
}

void VoxelLibrarySimple::refresh_rects() {
	for (int i = 0; i < _voxel_surfaces.size(); i++) {
		Ref<VoxelSurfaceSimple> surface = Ref<VoxelSurfaceSimple>(_voxel_surfaces[i]);

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

VoxelLibrarySimple::VoxelLibrarySimple() {
	_atlas_rows = 8;
	_atlas_columns = 8;
}

VoxelLibrarySimple::~VoxelLibrarySimple() {
	for (int i = 0; i < _voxel_surfaces.size(); ++i) {
		Ref<VoxelSurface> surface = _voxel_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<VoxelLibrarySimple>());
		}
	}

	_voxel_surfaces.clear();
}

void VoxelLibrarySimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &VoxelLibrarySimple::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &VoxelLibrarySimple::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &VoxelLibrarySimple::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &VoxelLibrarySimple::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_voxel_surfaces"), &VoxelLibrarySimple::get_voxel_surfaces);
	ClassDB::bind_method(D_METHOD("set_voxel_surfaces"), &VoxelLibrarySimple::set_voxel_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_surfaces", PROPERTY_HINT_NONE, "17/17:VoxelSurfaceSimple", PROPERTY_USAGE_DEFAULT, "VoxelSurfaceSimple"), "set_voxel_surfaces", "get_voxel_surfaces");
}
