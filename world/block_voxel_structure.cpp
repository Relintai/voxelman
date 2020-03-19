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

#include "block_voxel_structure.h"

int BlockVoxelStructure::get_channel_count() const {
	return _channel_count;
}
void BlockVoxelStructure::set_channel_count(const int value) {
	_channel_count = value;
}

uint8_t BlockVoxelStructure::get_voxel(int p_x, int p_y, int p_z, int p_channel_index) const {
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
void BlockVoxelStructure::set_voxel(uint8_t p_value, int p_x, int p_y, int p_z, int p_channel_index) {
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

PoolByteArray BlockVoxelStructure::get_voxel_data(int p_x, int p_y, int p_z) const {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	if (!_data.has(p))
		return PoolByteArray();

	return _data[p];
}
void BlockVoxelStructure::set_voxel_data(PoolByteArray p_arr, int p_x, int p_y, int p_z) {
	VSIntPos p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;

	_data[p] = p_arr;
}

//void BlockVoxelStructure::_write_to_chunk(Node *chunk) {
//VoxelChunk *c = Object::cast_to<VoxelChunk>(chunk);
//}

void BlockVoxelStructure::clear() {
	_data.clear();
}

BlockVoxelStructure::BlockVoxelStructure() {
	_channel_count = 0;
}

BlockVoxelStructure::~BlockVoxelStructure() {
	_data.clear();
}

void BlockVoxelStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_count"), &BlockVoxelStructure::get_channel_count);
	ClassDB::bind_method(D_METHOD("set_channel_count", "value"), &BlockVoxelStructure::set_channel_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "set_channel_count", "get_channel_count");

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "z", "channel_index"), &BlockVoxelStructure::get_voxel, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "y", "z", "channel_index"), &BlockVoxelStructure::set_voxel, DEFVAL(0));

	ClassDB::bind_method(D_METHOD("get_voxel_data", "x", "y", "z"), &BlockVoxelStructure::get_voxel_data);
	ClassDB::bind_method(D_METHOD("set_voxel_data", "arr", "x", "y", "z"), &BlockVoxelStructure::set_voxel_data);

	//ClassDB::bind_method(D_METHOD("_write_to_chunk", "chunk"), &BlockVoxelStructure::_write_to_chunk);
}
