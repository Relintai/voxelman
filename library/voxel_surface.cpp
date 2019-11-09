#include "voxel_surface.h"

int VoxelSurface::get_id() const {
	return _id;
}
void VoxelSurface::set_id(const int value) {
	_id = value;
}

bool VoxelSurface::is_transparent() const {
	return _is_transparent;
}
void VoxelSurface::set_transparent(const bool transparent) {
	_is_transparent = transparent;
}

Rect2 VoxelSurface::get_rect(const VoxelSurfaceSides side) const {
	return _rects[side];
}
void VoxelSurface::set_rect(const VoxelSurfaceSides side, const Rect2 rect) {
	_rects[side] = rect;
}

Ref<VoxelmanLibrary> VoxelSurface::get_library() const {
	return Ref<VoxelmanLibrary>(_library);
}

void VoxelSurface::set_library(Ref<VoxelmanLibrary> library) {
	_library = (*library);

	if (_library != NULL)
		refresh_rects();
}

Vector2 VoxelSurface::transform_uv(const VoxelSurfaceSides p_side, const Vector2 p_uv) const {
	Vector2 uv = p_uv;

	Rect2 r = _rects[p_side];

	uv.x *= r.size.x;
	uv.y *= r.size.y;
	uv.x += r.position.x;
	uv.y += r.position.y;

	return uv;
}

void VoxelSurface::refresh_rects() {
}

VoxelSurface::VoxelSurface() {
	_id = 0;
	_is_transparent = false;
	_library = NULL;
}

VoxelSurface::~VoxelSurface() {
	_library = NULL;
}

void VoxelSurface::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &VoxelSurface::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "value"), &VoxelSurface::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("set_transparent", "transparent"), &VoxelSurface::set_transparent);
	ClassDB::bind_method(D_METHOD("is_transparent"), &VoxelSurface::is_transparent);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transparent"), "set_transparent", "is_transparent");

	ClassDB::bind_method(D_METHOD("get_rect", "side"), &VoxelSurface::get_rect);
	ClassDB::bind_method(D_METHOD("set_rect", "side", "rect"), &VoxelSurface::set_rect);

	ClassDB::bind_method(D_METHOD("transform_uv", "side", "uv"), &VoxelSurface::transform_uv);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelSurface::refresh_rects);

	BIND_ENUM_CONSTANT(VOXEL_SIDE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_SIDE);

	BIND_CONSTANT(VOXEL_SIDES_COUNT);
}
