#include "voxel_surface.h"

int VoxelSurface::get_id() const {
    return _id;
}
void VoxelSurface::set_id(int value) {
    _id = value;
}

int VoxelSurface::get_atlas_x() const {
	return _atlas_x;
}
void VoxelSurface::set_atlas_x(int value) {
	_atlas_x = value;
}

int VoxelSurface::get_atlas_y() const {
	return _atlas_y;
}
void VoxelSurface::set_atlas_y(int value) {
	_atlas_y = value;
}

bool VoxelSurface::is_transparent() const {
    return _is_transparent;
}
void VoxelSurface::set_transparent(bool transparent) {
    _is_transparent = transparent;
}

String VoxelSurface::get_voxel_name() const {
    return _name;
}
void VoxelSurface::set_voxel_name(String name) {
    _name = name;
}

Ref<VoxelmanLibrary> VoxelSurface::get_library() const {
    return Ref<VoxelmanLibrary>(_library);
}

void VoxelSurface::set_library(Ref<VoxelmanLibrary> library) {
    _library = library;
}

VoxelSurface::VoxelSurface() {
	_id = 0;

	_atlas_x = 0;
	_atlas_y = 0;

	_is_transparent = false;
}

VoxelSurface::VoxelSurface(int id) {
	_id = id;

	_atlas_x = 0;
	_atlas_y = 0;

	_is_transparent = false;
}

VoxelSurface::~VoxelSurface() {
	_library.unref();
}

void VoxelSurface::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_id"), &VoxelSurface::get_id);
    ClassDB::bind_method(D_METHOD("set_id", "value"), &VoxelSurface::set_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	//create an array
    ClassDB::bind_method(D_METHOD("get_atlas_x"), &VoxelSurface::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "value"), &VoxelSurface::set_atlas_x);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_x"), "set_atlas_x", "get_atlas_x");

	ClassDB::bind_method(D_METHOD("get_atlas_y"), &VoxelSurface::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "value"), &VoxelSurface::set_atlas_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_y"), "set_atlas_y", "get_atlas_y");

    ClassDB::bind_method(D_METHOD("set_transparent", "transparent"), &VoxelSurface::set_transparent, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("is_transparent"), &VoxelSurface::is_transparent);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transparent"), "set_transparent", "is_transparent");

    ClassDB::bind_method(D_METHOD("set_voxel_name", "name"), &VoxelSurface::set_voxel_name);
    ClassDB::bind_method(D_METHOD("get_voxel_name"), &VoxelSurface::get_voxel_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");

	BIND_ENUM_CONSTANT(VOXEL_SIDE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_BACK);
	BIND_ENUM_CONSTANT(VOXEL_SIDES_COUNT);
}
