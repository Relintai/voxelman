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

int VoxelStructure::get_channel_count() const {
	return _channel_count;
}
void VoxelStructure::set_channel_count(const int value) {
	_channel_count = value;
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

uint8_t VoxelStructure::get_voxel(int p_x, int p_y, int p_z, int p_channel_index) const {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	if (!_data.has(p))
		return 0;

	PoolByteArray arr = _data[p];

	ERR_FAIL_INDEX_V(arr.size(), p_channel_index, 0);

	return arr[p_channel_index];
}
void VoxelStructure::set_voxel(uint8_t p_value, int p_x, int p_y, int p_z, int p_channel_index) {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	PoolByteArray arr;

	if (!_data.has(p)) {
		arr.resize(_channel_count);

		for (int i = 0; i < _channel_count; ++i) {
			arr.set(i, p_value);
		}
	} else {
		arr = _data[p];
	}

	_data[p] = arr;
}

PoolByteArray VoxelStructure::get_voxel_data(int p_x, int p_y, int p_z) const {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	if (!_data.has(p))
		return PoolByteArray();

	return _data[p];
}
void VoxelStructure::set_voxel_data(PoolByteArray p_arr, int p_x, int p_y, int p_z) {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	_data[p] = p_arr;
}

void VoxelStructure::write_to_chunk_bind(Node *chunk) {
	VoxelChunk *c = Object::cast_to<VoxelChunk>(chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(c));

	write_to_chunk(c);
}
void VoxelStructure::write_to_chunk(VoxelChunk *chunk) {
}

void VoxelStructure::clear() {
	_data.clear();
}

VoxelStructure::VoxelStructure() {
	_channel_count = 0;

	_world_position_x = 0;
	_world_position_y = 0;
	_world_position_z = 0;
}

VoxelStructure::~VoxelStructure() {
	_data.clear();
}

void VoxelStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_count"), &VoxelStructure::get_channel_count);
	ClassDB::bind_method(D_METHOD("set_channel_count", "value"), &VoxelStructure::set_channel_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "set_channel_count", "get_channel_count");

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

	ClassDB::bind_method(D_METHOD("get_voxel_data", "x", "y", "z"), &VoxelStructure::get_voxel_data);
	ClassDB::bind_method(D_METHOD("set_voxel_data", "arr", "x", "y", "z"), &VoxelStructure::set_voxel_data);

	ClassDB::bind_method(D_METHOD("write_to_chunk", "chunk"), &VoxelStructure::write_to_chunk_bind);
}
