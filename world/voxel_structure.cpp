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

int VoxelStructure::get_chunk_size_x() const {
	return _chunk_size_x;
}
void VoxelStructure::set_chunk_size_x(const int value) {
	_chunk_size_x = value;
}

int VoxelStructure::get_chunk_size_y() const {
	return _chunk_size_y;
}
void VoxelStructure::set_chunk_size_y(const int value) {
	_chunk_size_y = value;
}

int VoxelStructure::get_chunk_size_z() const {
	return _chunk_size_z;
}
void VoxelStructure::set_chunk_size_z(const int value) {
	_chunk_size_z = value;
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

int VoxelStructure::get_voxel(int x, int y, int z, unsigned int channel_index) {
	return 0;
}
void VoxelStructure::set_voxel(int value, int x, int y, int z, unsigned int channel_index) {
}

void VoxelStructure::add_from_chunk_bind(Node *chunk) {
	VoxelChunk *c = Object::cast_to<VoxelChunk>(chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(c));

	add_from_chunk(c);
}
void VoxelStructure::add_from_chunk(VoxelChunk *chunk) {
}

void VoxelStructure::write_to_chunk_bind(Node *chunk) {
	VoxelChunk *c = Object::cast_to<VoxelChunk>(chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(c));

	add_from_chunk(c);
}
void VoxelStructure::write_to_chunk(VoxelChunk *chunk) {
}

VoxelStructure::VoxelStructure() {
}

VoxelStructure::~VoxelStructure() {
	_data.clear();
}

void VoxelStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_chunk_size_x"), &VoxelStructure::get_chunk_size_x);
	ClassDB::bind_method(D_METHOD("set_chunk_size_x", "value"), &VoxelStructure::set_chunk_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_x"), "set_chunk_size_x", "get_chunk_size_x");

	ClassDB::bind_method(D_METHOD("get_chunk_size_y"), &VoxelStructure::get_chunk_size_y);
	ClassDB::bind_method(D_METHOD("set_chunk_size_y", "value"), &VoxelStructure::set_chunk_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_y"), "set_chunk_size_y", "get_chunk_size_y");

	ClassDB::bind_method(D_METHOD("get_chunk_size_z"), &VoxelStructure::get_chunk_size_z);
	ClassDB::bind_method(D_METHOD("set_chunk_size_z", "value"), &VoxelStructure::set_chunk_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "chunk_size_z"), "set_chunk_size_z", "get_chunk_size_z");

	ClassDB::bind_method(D_METHOD("get_world_position_x"), &VoxelStructure::get_world_position_x);
	ClassDB::bind_method(D_METHOD("set_world_position_x", "value"), &VoxelStructure::set_world_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_x"), "set_world_position_x", "get_world_position_x");

	ClassDB::bind_method(D_METHOD("get_world_position_y"), &VoxelStructure::get_world_position_y);
	ClassDB::bind_method(D_METHOD("set_world_position_y", "value"), &VoxelStructure::set_world_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_y"), "set_world_position_y", "get_world_position_y");

	ClassDB::bind_method(D_METHOD("get_world_position_z"), &VoxelStructure::get_world_position_z);
	ClassDB::bind_method(D_METHOD("set_world_position_z", "value"), &VoxelStructure::set_world_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "world_position_z"), "set_world_position_z", "get_world_position_z");

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z", "channel_index"), &VoxelStructure::get_voxel, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "y", "z", "channel_index"), &VoxelStructure::set_voxel, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("add_from_chunk", "chunk"), &VoxelStructure::add_from_chunk_bind);
	ClassDB::bind_method(D_METHOD("write_to_chunk", "chunk"), &VoxelStructure::write_to_chunk_bind);
}
