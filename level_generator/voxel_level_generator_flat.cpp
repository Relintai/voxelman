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

#include "voxel_level_generator_flat.h"

#include "../world/voxel_chunk.h"

int VoxelLevelGeneratorFlat::get_floor_position() const {
	return _floor_position;
}
void VoxelLevelGeneratorFlat::set_floor_position(const int floor_height) {
	_floor_position = floor_height;
}

Dictionary VoxelLevelGeneratorFlat::get_channel_map() {
	return _channel_map;
}
void VoxelLevelGeneratorFlat::set_channel_map(const Dictionary &map) {
	_channel_map = map;
}

void VoxelLevelGeneratorFlat::_generate_chunk(Ref<VoxelChunk> chunk) {
	int dymin = chunk->get_position_y() * chunk->get_size_y();
	int dymax = dymin + chunk->get_size_y() + chunk->get_margin_end();

	if (_floor_position < dymin)
		return;

	if (_floor_position > dymax) {
		Variant key;
		while (_channel_map.next(&key)) {
			int k = key;
			int value = _channel_map[key];

			chunk->channel_fill(value, k);
		}

		return;
	}

	const Variant *keyptr = NULL;
	keyptr = _channel_map.next(keyptr);
	while (keyptr) {
		Variant key = (*keyptr);
		int k = key;
		int value = _channel_map[key];

		uint8_t *channel = chunk->channel_get_valid(k, 0);

		if (!channel)
			continue;

		int ty = _floor_position - dymin;
		for (int y = 0; y < ty; ++y) {
			for (int z = 0; z < chunk->get_data_size_z(); ++z) {
				for (int x = 0; x < chunk->get_data_size_x(); ++x) {
					channel[chunk->get_data_index(x, y, z)] = value;
				}
			}
		}

		keyptr = _channel_map.next(keyptr);
	}
}

VoxelLevelGeneratorFlat::VoxelLevelGeneratorFlat() {
	_floor_position = 0;
}

VoxelLevelGeneratorFlat::~VoxelLevelGeneratorFlat() {
	_channel_map.clear();
}

void VoxelLevelGeneratorFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_floor_position"), &VoxelLevelGeneratorFlat::get_floor_position);
	ClassDB::bind_method(D_METHOD("set_floor_position", "value"), &VoxelLevelGeneratorFlat::set_floor_position);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "floor_position"), "set_floor_position", "get_floor_position");

	ClassDB::bind_method(D_METHOD("get_channel_map"), &VoxelLevelGeneratorFlat::get_channel_map);
	ClassDB::bind_method(D_METHOD("set_channel_map", "value"), &VoxelLevelGeneratorFlat::set_channel_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "channel_map"), "set_channel_map", "get_channel_map");

	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &VoxelLevelGeneratorFlat::_generate_chunk);
}
