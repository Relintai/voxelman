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

int BlockVoxelStructure::get_channel_type() const {
	return _channel_type;
}
void BlockVoxelStructure::set_channel_type(const int value) {
	_channel_type = value;
}

int BlockVoxelStructure::get_channel_isolevel() const {
	return _channel_isolevel;
}
void BlockVoxelStructure::set_channel_isolevel(const int value) {
	_channel_isolevel = value;
}

int BlockVoxelStructure::get_voxel_type(int p_x, int p_y, int p_z) const {
	DataEntry p;

	for (int i = 0; i < _data.size(); ++i) {
		p = _data[i];

		if (p.x == p_x && p.y == p_y && p.z == p_z) {
			return p.data_type;
		}
	}

	return 0;
}
int BlockVoxelStructure::get_voxel_isolevel(int p_x, int p_y, int p_z) const {
	DataEntry p;

	for (int i = 0; i < _data.size(); ++i) {
		p = _data[i];

		if (p.x == p_x && p.y == p_y && p.z == p_z) {
			return p.data_isolevel;
		}
	}

	return 0;
}

void BlockVoxelStructure::set_voxel(int p_x, int p_y, int p_z, int p_type, int p_isolevel) {
	DataEntry p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;
	p.data_type = p_type;
	p.data_isolevel = p_isolevel;

	_data.push_back(p);
}

void BlockVoxelStructure::_write_to_chunk(Ref<VoxelChunk> chunk) {
	//Ref<VoxelChunk> c = Object::cast_to<VoxelChunk>(chunk);
}

void BlockVoxelStructure::clear() {
	_data.clear();
}

BlockVoxelStructure::BlockVoxelStructure() {
	_channel_type = 0;
	_channel_isolevel = 0;
}

BlockVoxelStructure::~BlockVoxelStructure() {
	_data.clear();
}

void BlockVoxelStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_type"), &BlockVoxelStructure::get_channel_type);
	ClassDB::bind_method(D_METHOD("set_channel_type", "value"), &BlockVoxelStructure::set_channel_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_type"), "set_channel_type", "get_channel_type");

	ClassDB::bind_method(D_METHOD("get_channel_isolevel"), &BlockVoxelStructure::get_channel_isolevel);
	ClassDB::bind_method(D_METHOD("set_channel_isolevel", "value"), &BlockVoxelStructure::set_channel_isolevel);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_isolevel"), "set_channel_isolevel", "get_channel_isolevel");

	ClassDB::bind_method(D_METHOD("get_voxel_type", "x", "y", "z"), &BlockVoxelStructure::get_voxel_type);
	ClassDB::bind_method(D_METHOD("get_voxel_isolevel", "x", "y", "z"), &BlockVoxelStructure::get_voxel_isolevel);

	ClassDB::bind_method(D_METHOD("set_voxel", "x", "y", "z", "type", "isolevel"), &BlockVoxelStructure::set_voxel);

	ClassDB::bind_method(D_METHOD("_write_to_chunk", "chunk"), &BlockVoxelStructure::_write_to_chunk);
}
