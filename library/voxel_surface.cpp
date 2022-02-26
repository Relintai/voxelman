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

#include "voxel_surface.h"

int VoxelSurface::get_id() const {
	return _id;
}
void VoxelSurface::set_id(const int value) {
	_id = value;
}

int VoxelSurface::get_mesher_index() const {
	return _mesher_index;
}
void VoxelSurface::set_mesher_index(const int value) {
	_mesher_index = value;
}

bool VoxelSurface::get_transparent() const {
	return _transparent;
}
void VoxelSurface::set_transparent(const bool transparent) {
	_transparent = transparent;
}

bool VoxelSurface::get_liquid() const {
	return _liquid;
}
void VoxelSurface::set_liquid(const bool value) {
	_liquid = value;
}

Rect2 VoxelSurface::get_rect(const VoxelSurfaceSides side) const {
	return _rects[side];
}
void VoxelSurface::set_rect(const VoxelSurfaceSides side, const Rect2 &rect) {
	_rects[side] = rect;
}

Ref<VoxelLibrary> VoxelSurface::get_library() const {
	return Ref<VoxelLibrary>(_library);
}

void VoxelSurface::set_library(Ref<VoxelLibrary> library) {
	_library = (*library);
}

_FORCE_INLINE_ Vector2 VoxelSurface::transform_uv(const VoxelSurfaceSides p_side, const Vector2 &p_uv) const {
	Vector2 uv = p_uv;

	Rect2 r = _rects[p_side];

	uv.x *= r.size.x;
	uv.y *= r.size.y;
	uv.x += r.position.x;
	uv.y += r.position.y;

	return uv;
}

_FORCE_INLINE_ Vector2 VoxelSurface::transform_uv_scaled(const VoxelSurfaceSides p_side, const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const {
	Vector2 uv = p_uv;

	Rect2 r = _rects[p_side];

	float sizex = r.size.x / static_cast<float>(p_max);
	float sizey = r.size.y / static_cast<float>(p_max);

	uv.x *= sizex;
	uv.y *= sizey;

	uv.x += r.position.x + sizex * p_current_x;
	uv.y += r.position.y + sizey * p_current_y;

	return uv;
}

void VoxelSurface::refresh_rects() {
}

VoxelSurface::VoxelSurface() {
	_id = 0;
	_mesher_index = 0;
	_transparent = false;
	_liquid = false;
	_library = NULL;
}

VoxelSurface::~VoxelSurface() {
	_library = NULL;
}

void VoxelSurface::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &VoxelSurface::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "value"), &VoxelSurface::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ClassDB::bind_method(D_METHOD("get_mesher_index"), &VoxelSurface::get_mesher_index);
	ClassDB::bind_method(D_METHOD("set_mesher_index", "value"), &VoxelSurface::set_mesher_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mesher_index"), "set_mesher_index", "get_mesher_index");

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("get_transparent"), &VoxelSurface::get_transparent);
	ClassDB::bind_method(D_METHOD("set_transparent", "transparent"), &VoxelSurface::set_transparent);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "transparent"), "set_transparent", "get_transparent");

	ClassDB::bind_method(D_METHOD("get_liquid"), &VoxelSurface::get_liquid);
	ClassDB::bind_method(D_METHOD("set_liquid", "transparent"), &VoxelSurface::set_liquid);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "liquid"), "set_liquid", "get_liquid");

	ClassDB::bind_method(D_METHOD("get_rect", "side"), &VoxelSurface::get_rect);
	ClassDB::bind_method(D_METHOD("set_rect", "side", "rect"), &VoxelSurface::set_rect);

	ClassDB::bind_method(D_METHOD("transform_uv", "side", "uv"), &VoxelSurface::transform_uv);
	ClassDB::bind_method(D_METHOD("transform_uv_scaled", "side", "uv", "p_current_x", "p_current_y", "max"), &VoxelSurface::transform_uv_scaled);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &VoxelSurface::refresh_rects);

	BIND_ENUM_CONSTANT(VOXEL_SIDE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_SIDE_SIDE);

	BIND_CONSTANT(VOXEL_SIDES_COUNT);
}
