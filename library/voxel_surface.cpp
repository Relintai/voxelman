#include "voxel_surface.h"

int VoxelSurface::get_id() const {
	return _id;
}
void VoxelSurface::set_id(int value) {
	_id = value;
}

int VoxelSurface::get_atlas_x(const int side) const {
	int indx = (side * 2);

	ERR_FAIL_INDEX_V(indx, VOXEL_SIDES_ARRAY_SIZE, 0);

	return _atlas_positions[indx];
}
void VoxelSurface::set_atlas_x(const int side, int value) {
	int indx = (side * 2);

	ERR_FAIL_INDEX(indx, VOXEL_SIDES_ARRAY_SIZE);

	_atlas_positions[indx] = value;
}

int VoxelSurface::get_atlas_y(const int side) const {
	int indx = (side * 2) + 1;

	ERR_FAIL_INDEX_V(indx, VOXEL_SIDES_ARRAY_SIZE, 0);

	return _atlas_positions[indx];
}
void VoxelSurface::set_atlas_y(const int side, int value) {
	int indx = (side * 2) + 1;

	ERR_FAIL_INDEX(indx, VOXEL_SIDES_ARRAY_SIZE);

	_atlas_positions[indx] = value;
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
	_library = (*library);
}

Vector2 VoxelSurface::transform_uv(const int side, const Vector2 uv) const {
	float culomn = 1.0 / static_cast<float>(_library->get_atlas_columns());
	float row = 1.0 / static_cast<float>(_library->get_atlas_rows());

	return Vector2(get_atlas_x(side) * culomn + culomn * uv.x, get_atlas_y(side) * row + row * uv.y);
}

VoxelSurface::VoxelSurface() {
	_id = 0;

	for (int i = 0; i < VOXEL_SIDES_ARRAY_SIZE; ++i) {
		_atlas_positions[i] = 0;
	}

	_is_transparent = false;

	_library = NULL;
}

VoxelSurface::VoxelSurface(int id) {
	_id = id;

	for (int i = 0; i < VOXEL_SIDES_ARRAY_SIZE; ++i) {
		_atlas_positions[i] = 0;
	}

	_is_transparent = false;

	_library = NULL;
}

VoxelSurface::~VoxelSurface() {
	//_library = NULL;
}

void VoxelSurface::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &VoxelSurface::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "value"), &VoxelSurface::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	//create an array
	ClassDB::bind_method(D_METHOD("get_atlas_x", "side"), &VoxelSurface::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "side", "value"), &VoxelSurface::set_atlas_x);
	
	ClassDB::bind_method(D_METHOD("get_atlas_y", "side"), &VoxelSurface::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "side", "value"), &VoxelSurface::set_atlas_y);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "top_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_TOP);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "bottom_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_BOTTOM);

	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_x"), "set_atlas_x", "get_atlas_x", VOXEL_SIDE_SIDE);
	ADD_PROPERTYI(PropertyInfo(Variant::INT, "side_atlas_y"), "set_atlas_y", "get_atlas_y", VOXEL_SIDE_SIDE);
	

	ClassDB::bind_method(D_METHOD("set_transparent", "transparent"), &VoxelSurface::set_transparent, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("is_transparent"), &VoxelSurface::is_transparent);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transparent"), "set_transparent", "is_transparent");

	ClassDB::bind_method(D_METHOD("set_voxel_name", "name"), &VoxelSurface::set_voxel_name);
	ClassDB::bind_method(D_METHOD("get_voxel_name"), &VoxelSurface::get_voxel_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("transform_uv", "side", "uv"), &VoxelSurface::transform_uv);

	BIND_ENUM_CONSTANT(VOXEL_SIDE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_SIDE);
	BIND_ENUM_CONSTANT(VOXEL_SIDES_COUNT);
}
