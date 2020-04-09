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

#include "voxel_structure.h"

bool VoxelStructure::get_use_aabb() const {
	return _use_aabb;
}
void VoxelStructure::set_use_aabb(const bool value) {
	_use_aabb = value;
}

AABB VoxelStructure::get_aabb() const {
	return _aabb;
}
void VoxelStructure::set_aabb(const AABB &value) {
	_aabb = value;
}

int VoxelStructure::get_world_position_x() const {
	return _world_position_x;
}
void VoxelStructure::set_world_position_x(const int value) {
	_world_position_x = value;
}

int VoxelStructure::get_world_position_y() const {
	return _world_position_y;
}
void VoxelStructure::set_world_position_y(const int value) {
	_world_position_y = value;
}

int VoxelStructure::get_world_position_z() const {
	return _world_position_z;
}
void VoxelStructure::set_world_position_z(const int value) {
	_world_position_z = value;
}

void VoxelStructure::write_to_chunk(Node *chunk) {
	ERR_FAIL_COND(Object::cast_to<VoxelChunk>(chunk) == NULL);

	if (has_method("_write_to_chunk"))
		call("_write_to_chunk", chunk);
}

VoxelStructure::VoxelStructure() {
	_use_aabb = true;
	_world_position_x = 0;
	_world_position_y = 0;
	_world_position_z = 0;
}

VoxelStructure::~VoxelStructure() {
}

void VoxelStructure::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_write_to_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_use_aabb"), &VoxelStructure::get_use_aabb);
	ClassDB::bind_method(D_METHOD("set_use_aabb", "value"), &VoxelStructure::set_use_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_aabb"), "set_use_aabb", "get_use_aabb");

	ClassDB::bind_method(D_METHOD("get_aabb"), &VoxelStructure::get_aabb);
	ClassDB::bind_method(D_METHOD("set_aabb", "value"), &VoxelStructure::set_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "aabb"), "set_aabb", "get_aabb");

	ClassDB::bind_method(D_METHOD("get_world_position_x"), &VoxelStructure::get_world_position_x);
	ClassDB::bind_method(D_METHOD("set_world_position_x", "value"), &VoxelStructure::set_world_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_x"), "set_world_position_x", "get_world_position_x");

	ClassDB::bind_method(D_METHOD("get_world_position_y"), &VoxelStructure::get_world_position_y);
	ClassDB::bind_method(D_METHOD("set_world_position_y", "value"), &VoxelStructure::set_world_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_y"), "set_world_position_y", "get_world_position_y");

	ClassDB::bind_method(D_METHOD("get_world_position_z"), &VoxelStructure::get_world_position_z);
	ClassDB::bind_method(D_METHOD("set_world_position_z", "value"), &VoxelStructure::set_world_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_z"), "set_world_position_z", "get_world_position_z");

	ClassDB::bind_method(D_METHOD("write_to_chunk", "chunk"), &VoxelStructure::write_to_chunk);
}
