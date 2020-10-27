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

#include "voxel_cube_points.h"
#include "../../world/default/voxel_chunk_default.h"
#include "../../world/voxel_chunk.h"

const unsigned int VoxelCubePoints::index_table[6][4] = {
	{ P000, P010, P110, P100 }, //VOXEL_FACE_FRONT 0
	{ P100, P110, P111, P101 }, //VOXEL_FACE_RIGHT 1
	{ P101, P111, P011, P001 }, //VOXEL_FACE_BACK 2
	{ P001, P011, P010, P000 }, //VOXEL_FACE_LEFT 3
	{ P111, P110, P010, P011 }, //VOXEL_FACE_TOP 4
	{ P001, P000, P100, P101 }, //VOXEL_FACE_BOTTOM 5
};

const unsigned int VoxelCubePoints::visibility_check_table[6] = {
	VOXEL_NEIGHBOUR_FRONT, //VOXEL_FACE_FRONT 0
	VOXEL_NEIGHBOUR_RIGHT, //VOXEL_FACE_RIGHT 1
	VOXEL_NEIGHBOUR_BACK, //VOXEL_FACE_BACK 2
	VOXEL_NEIGHBOUR_LEFT, //VOXEL_FACE_LEFT 3
	VOXEL_NEIGHBOUR_TOP, //VOXEL_FACE_TOP 4
	VOXEL_NEIGHBOUR_BOTTOM //VOXEL_FACE_BOTTOM 5
};

const int VoxelCubePoints::face_light_direction_table[6][3] = {
	{ 0, 0, -1 }, //VOXEL_FACE_FRONT 0
	{ -1, 0, 0 }, //VOXEL_FACE_RIGHT 1
	{ 0, 0, 1 }, //VOXEL_FACE_BACK 2
	{ 1, 0, 0 }, //VOXEL_FACE_LEFT 3
	{ 0, -1, 0 }, //VOXEL_FACE_TOP 4
	{ 0, 1, 0 } //VOXEL_FACE_BOTTOM 5
};

const float VoxelCubePoints::point_direction_table[8][3] = {
	{ -0.5, -0.5, -0.5 }, //P000
	{ 0.5, -0.5, -0.5 }, //P100
	{ -0.5, 0.5, -0.5 }, //P010
	{ -0.5, -0.5, 0.5 }, //P001

	{ 0.5, 0.5, -0.5 }, //P110
	{ -0.5, 0.5, 0.5 }, //P011
	{ 0.5, -0.5, 0.5 }, //P101
	{ 0.5, 0.5, 0.5 }, //P111
};

const unsigned int VoxelCubePoints::point_direction_neighbour_table[8][3] = {
	{ VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_FRONT }, //P000
	{ VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_FRONT }, //P100
	{ VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_FRONT }, //P010
	{ VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BACK }, //P001

	{ VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_FRONT }, //P110
	{ VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_BACK }, //P011
	{ VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BACK }, //P101
	{ VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_BACK }, //P111
};

const float VoxelCubePoints::uv_direction_table[8][4][2] = {
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_FRONT 0, P000, P010, P110, P100
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_RIGHT 1, P100, P110, P111, P101
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_BACK 2, P101, P111, P011, P001
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_LEFT 3, P001, P011, P010, P000
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_TOP 4, P111, P110, P010, P011
	{ { -0.5, -0.5 }, { 0.5, -0.5 }, { 0.5, 0.5 }, { 0.5, -0.5 } }, //VOXEL_FACE_BOTTOM 5, P001, P000, P100, P101
};

int VoxelCubePoints::get_x() {
	return _x;
}
void VoxelCubePoints::set_x(int value) {
	_x = value;
}

int VoxelCubePoints::get_y() {
	return _y;
}
void VoxelCubePoints::set_y(int value) {
	_y = value;
}

int VoxelCubePoints::get_z() {
	return _z;
}
void VoxelCubePoints::set_z(int value) {
	_z = value;
}

int VoxelCubePoints::get_size() {
	return _size;
}
void VoxelCubePoints::set_size(int value) {
	_size = value;
}

int VoxelCubePoints::get_channel_index_type() const {
	return _channel_index_type;
}
void VoxelCubePoints::set_channel_index_type(const int value) {
	_channel_index_type = value;
}

int VoxelCubePoints::get_channel_index_isolevel() const {
	return _channel_index_isolevel;
}
void VoxelCubePoints::set_channel_index_isolevel(const int value) {
	_channel_index_isolevel = value;
}

void VoxelCubePoints::refresh_points() {
	for (int i = 0; i < POINT_COUNT; ++i) {
		recalculate_point(i);
	}

	/*
	//Front
	//Bottom Left
	refresh_point(P000, 0, 0, 0,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT);

	//Bottom Right
	refresh_point(P100, 255, 0, 0,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_BOTTOM_FRONT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT);

	//Top Left
	refresh_point(P010, 0, 255, 0,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_TOP_LEFT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_FRONT);

	//Top Right
	refresh_point(P110, 255, 255, 0,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT,
			VOXEL_NEIGHBOUR_FRONT, VOXEL_NEIGHBOUR_TOP_FRONT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_FRONT);

	//Back
	//Bottom Left
	refresh_point(P001, 0, 0, 255,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_LEFT, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK);

	//Bottom Right
	refresh_point(P101, 255, 0, 255,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_BOTTOM, VOXEL_NEIGHBOUR_BOTTOM_RIGHT, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_BOTTOM_BACK, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK);

	//Top Left
	refresh_point(P011, 0, 255, 255,
			VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_LEFT, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_TOP_LEFT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_LEFT, VOXEL_NEIGHBOUR_BACK);

	//Top Right
	refresh_point(P111, 255, 255, 255,
			VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK,
			VOXEL_NEIGHBOUR_TOP, VOXEL_NEIGHBOUR_TOP_RIGHT, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_TOP_RIGHT_BACK,
			VOXEL_NEIGHBOUR_BACK, VOXEL_NEIGHBOUR_TOP_BACK, VOXEL_NEIGHBOUR_RIGHT, VOXEL_NEIGHBOUR_BACK);*/
}

void VoxelCubePoints::recalculate_point(int point) {
	ERR_FAIL_INDEX(point, POINT_COUNT);

	Vector3 static_offset;
	Vector3 dynamic_offset;

	for (int i = 0; i < 3; ++i) {
		int neighbour_check = point_direction_neighbour_table[point][i];

		if ((_point_neighbours[point] & neighbour_check) == 0) {
			dynamic_offset[i] = point_direction_table[point][i];
		} else {
			static_offset[i] = point_direction_table[point][i];
		}
	}

	dynamic_offset *= (_point_fills[point] / 255.0);
	dynamic_offset += static_offset;
	dynamic_offset += Vector3(0.5 * _size, 0.5 * _size, 0.5 * _size);

	_points[point] = dynamic_offset;
}

void VoxelCubePoints::refresh_neighbours(Ref<VoxelChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	int neighbours = 0;

	int x = _x;
	int y = _y;
	int z = _z;

	//000
	if (chunk->get_voxel(x - 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT;

	if (chunk->get_voxel(x, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM;

	if (chunk->get_voxel(x, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_FRONT;
	/*
	if (chunk->get_voxel(x - 1, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT_FRONT;

	if (chunk->get_voxel(x - 1, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_LEFT;

	if (chunk->get_voxel(x, y - 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_FRONT;

	if (chunk->get_voxel(x - 1, y - 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT;*/

	_point_neighbours[P000] = neighbours;

	neighbours = 0;
	x = _x + 1;
	y = _y;
	z = _z;

	//100
	if (chunk->get_voxel(x + 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT;

	if (chunk->get_voxel(x, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM;

	if (chunk->get_voxel(x, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_FRONT;
	/*
	if (chunk->get_voxel(x + 1, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT_FRONT;

	if (chunk->get_voxel(x + 1, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_RIGHT;

	if (chunk->get_voxel(x, y - 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_FRONT;

	if (chunk->get_voxel(x + 1, y - 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT;*/

	_point_neighbours[P100] = neighbours;

	neighbours = 0;
	x = _x;
	y = _y + 1;
	z = _z;

	//010
	if (chunk->get_voxel(x - 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT;

	if (chunk->get_voxel(x, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP;

	if (chunk->get_voxel(x, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_FRONT;
	/*
	if (chunk->get_voxel(x - 1, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT_FRONT;

	if (chunk->get_voxel(x - 1, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_LEFT;

	if (chunk->get_voxel(x, y + 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_FRONT;

	if (chunk->get_voxel(x - 1, y + 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_LEFT_FRONT;*/

	_point_neighbours[P010] = neighbours;

	neighbours = 0;
	x = _x + 1;
	y = _y + 1;
	z = _z;

	//110
	if (chunk->get_voxel(x + 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT;

	if (chunk->get_voxel(x, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP;

	if (chunk->get_voxel(x, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_FRONT;
	/*
	if (chunk->get_voxel(x + 1, y, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT_FRONT;

	if (chunk->get_voxel(x + 1, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_RIGHT;

	if (chunk->get_voxel(x, y + 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_FRONT;

	if (chunk->get_voxel(x + 1, y + 1, z - 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT;*/

	_point_neighbours[P110] = neighbours;

	neighbours = 0;
	x = _x;
	y = _y;
	z = _z + 1;

	//001
	if (chunk->get_voxel(x - 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT;

	if (chunk->get_voxel(x, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM;

	if (chunk->get_voxel(x, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BACK;
	/*
	if (chunk->get_voxel(x - 1, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT_BACK;

	if (chunk->get_voxel(x - 1, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_LEFT;

	if (chunk->get_voxel(x, y - 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_BACK;

	if (chunk->get_voxel(x - 1, y - 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK;*/

	_point_neighbours[P001] = neighbours;

	neighbours = 0;
	x = _x + 1;
	y = _y;
	z = _z + 1;

	//101
	if (chunk->get_voxel(x + 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT;

	if (chunk->get_voxel(x, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM;

	if (chunk->get_voxel(x, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BACK;
	/*
	if (chunk->get_voxel(x + 1, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT_BACK;

	if (chunk->get_voxel(x + 1, y - 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_RIGHT;

	if (chunk->get_voxel(x, y - 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_BACK;

	if (chunk->get_voxel(x + 1, y - 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK;*/

	_point_neighbours[P101] = neighbours;

	neighbours = 0;
	x = _x;
	y = _y + 1;
	z = _z + 1;

	//011
	if (chunk->get_voxel(x - 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT;

	if (chunk->get_voxel(x, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP;

	if (chunk->get_voxel(x, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BACK;
	/*
	if (chunk->get_voxel(x - 1, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_LEFT_BACK;

	if (chunk->get_voxel(x - 1, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_LEFT;

	if (chunk->get_voxel(x, y + 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_BACK;

	if (chunk->get_voxel(x - 1, y + 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_LEFT_BACK;*/

	_point_neighbours[P011] = neighbours;

	neighbours = 0;
	x = _x + 1;
	y = _y + 1;
	z = _z + 1;

	//111
	if (chunk->get_voxel(x + 1, y, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT;

	if (chunk->get_voxel(x, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP;

	if (chunk->get_voxel(x, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_BACK;
	/*
	if (chunk->get_voxel(x + 1, y, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_RIGHT_BACK;

	if (chunk->get_voxel(x + 1, y + 1, z, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_RIGHT;

	if (chunk->get_voxel(x, y + 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_BACK;

	if (chunk->get_voxel(x + 1, y + 1, z + 1, _channel_index_type) != 0)
		neighbours = neighbours | VOXEL_NEIGHBOUR_TOP_RIGHT_BACK;*/

	_point_neighbours[P111] = neighbours;
}

void VoxelCubePoints::setup(Ref<VoxelChunk> chunk, int x, int y, int z, int size) {
	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND(size <= 0);
	ERR_FAIL_COND(!chunk->validate_data_position(x + size, y + size, z + size) || !chunk->validate_data_position(x, y, z));

	reset();

	_x = x;
	_y = y;
	_z = z;
	_size = size;

	_point_types[P000] = chunk->get_voxel(x, y, z, _channel_index_type);
	_point_types[P100] = chunk->get_voxel(x + size, y, z, _channel_index_type);
	_point_types[P010] = chunk->get_voxel(x, y + size, z, _channel_index_type);
	_point_types[P001] = chunk->get_voxel(x, y, z + size, _channel_index_type);
	_point_types[P110] = chunk->get_voxel(x + size, y + size, z, _channel_index_type);
	_point_types[P011] = chunk->get_voxel(x, y + size, z + size, _channel_index_type);
	_point_types[P101] = chunk->get_voxel(x + size, y, z + size, _channel_index_type);
	_point_types[P111] = chunk->get_voxel(x + size, y + size, z + size, _channel_index_type);

	if (!has_points())
		return;

	//for (int i = 0; i < 8; ++i) {
	//	if (_point_types[i] == 0) {
	//		_point_types[i] = 1;
	//	}
	//}

	_point_fills[P000] = chunk->get_voxel(x, y, z, _channel_index_isolevel);
	_point_fills[P100] = chunk->get_voxel(x + size, y, z, _channel_index_isolevel);
	_point_fills[P010] = chunk->get_voxel(x, y + size, z, _channel_index_isolevel);
	_point_fills[P001] = chunk->get_voxel(x, y, z + size, _channel_index_isolevel);
	_point_fills[P110] = chunk->get_voxel(x + size, y + size, z, _channel_index_isolevel);
	_point_fills[P011] = chunk->get_voxel(x, y + size, z + size, _channel_index_isolevel);
	_point_fills[P101] = chunk->get_voxel(x + size, y, z + size, _channel_index_isolevel);
	_point_fills[P111] = chunk->get_voxel(x + size, y + size, z + size, _channel_index_isolevel);

	//for (int i = 0; i < 8; ++i) {
	//	if (_point_fills[i] == 0) {
	//		_point_fills[i] = 1;
	//	}
	//}

	_point_aos[P000] = chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P100] = chunk->get_voxel(x + size, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P010] = chunk->get_voxel(x, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P001] = chunk->get_voxel(x, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P110] = chunk->get_voxel(x + size, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P011] = chunk->get_voxel(x, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P101] = chunk->get_voxel(x + size, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_AO);
	_point_aos[P111] = chunk->get_voxel(x + size, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_AO);

	_point_colors[P000] = Color(chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P100] = Color(chunk->get_voxel(x + size, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x + size, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x + size, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P010] = Color(chunk->get_voxel(x, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P001] = Color(chunk->get_voxel(x, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P110] = Color(chunk->get_voxel(x + size, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x + size, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x + size, y + size, z, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P011] = Color(chunk->get_voxel(x, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P101] = Color(chunk->get_voxel(x + size, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x + size, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x + size, y, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);
	_point_colors[P111] = Color(chunk->get_voxel(x + size, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0, chunk->get_voxel(x + size, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0, chunk->get_voxel(x + size, y + size, z + size, VoxelChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

	refresh_neighbours(chunk);

	refresh_points();
}

void VoxelCubePoints::reset() {
	for (int i = 0; i < POINT_COUNT; ++i) {
		_point_types[i] = 0;
		_point_fills[i] = 0;
		_point_neighbours[i] = 0;
	}

	_x = 0;
	_y = 0;
	_z = 0;
	_size = 1;
}

int VoxelCubePoints::get_point_index(int face, int index) {
	ERR_FAIL_INDEX_V(face, VOXEL_FACE_COUNT, 0);
	ERR_FAIL_INDEX_V(index, 4, 0);

	return index_table[face][index];
}

Vector2 VoxelCubePoints::get_point_uv_direction(int face, int index) {
	ERR_FAIL_INDEX_V(face, VOXEL_FACE_COUNT, Vector2());
	ERR_FAIL_INDEX_V(index, 4, Vector2());

	return Vector2(uv_direction_table[face][index][0], uv_direction_table[face][index][1]);
}

Vector3 VoxelCubePoints::get_points_for_face(int face, int index) {
	return _points[get_point_index(face, index)];
}

bool VoxelCubePoints::is_face_visible(int face) {
	ERR_FAIL_INDEX_V(face, VOXEL_FACE_COUNT, false);

	int target_neighbour = visibility_check_table[face];

	for (int i = 0; i < 4; ++i) {
		int indx = get_point_index(face, i);
		int neighbour_mask = _point_neighbours[indx];

		if ((neighbour_mask & target_neighbour) == 0)
			return true;
	}

	return false;
}

bool VoxelCubePoints::is_sub_voxel_point(int x, int y, int z) {
	for (int i = 0; i < POINT_COUNT; i += 1) {
		if (get_point(i) == Vector3(x, y, z)) {
			return true;
		}
	}
	return false;
}

void VoxelCubePoints::set_point(int point, int x, int y, int z) {
	_points[point] = Vector3(x, y, z);
}

int VoxelCubePoints::get_point_id(int x, int y, int z) {
	//for (int i = 0; i < POINT_COUNT; ++i) {
	//	if (get_point(i) == Vector3i(x, y, z)) {
	//		return i;
	//	}
	//}
	return 0;
}

Vector3 VoxelCubePoints::get_point_for_face(int face, int index) {
	int indx = get_point_index(face, index);

	return _points[indx];
}

Vector3 VoxelCubePoints::get_vertex_vector3_for_point(int face, int index) {
	int point_index = get_point_index(face, index);

	Vector3 a = get_point(point_index);

	Vector3 vector(a.x, a.y, a.z);

	float num = (float)255;
	float num2 = num / (float)2;
	vector.x -= num2;
	vector.y -= num2;
	vector.z -= num2;
	vector /= num;

	return vector;
}

int VoxelCubePoints::get_point_type(int index) {
	ERR_FAIL_INDEX_V(index, POINT_COUNT, 0);

	return _point_types[index];
}

int VoxelCubePoints::get_point_fill(int index) {
	ERR_FAIL_INDEX_V(index, POINT_COUNT, 0);

	return _point_fills[index];
}

int VoxelCubePoints::get_point_neighbours(int index) {
	ERR_FAIL_INDEX_V(index, POINT_COUNT, 0);

	return _point_neighbours[index];
}

int VoxelCubePoints::get_point_ao(int index) {
	ERR_FAIL_INDEX_V(index, POINT_COUNT, 0);

	return _point_aos[index];
}

int VoxelCubePoints::get_face_point_ao(int face, int index) {
	int indx = get_point_index(face, index);

	return _point_aos[indx];
}

Color VoxelCubePoints::get_face_point_ao_color(int face, int index) {
	int indx = get_point_index(face, index);

	float ao_value = (_point_aos[indx] / 255.0) * 0.75;

	return Color(ao_value, ao_value, ao_value);
}

Color VoxelCubePoints::get_face_point_light_color(int face, int index) {
	int indx = get_point_index(face, index);

	return _point_colors[indx];
}
Color VoxelCubePoints::get_face_point_color_mixed(int face, int index) {
	int indx = get_point_index(face, index);

	float ao_value = (_point_aos[indx] / 255.0) * 0.75;

	return _point_colors[indx] - Color(ao_value, ao_value, ao_value);
}

Vector3 VoxelCubePoints::get_face_light_direction(int face) {
	ERR_FAIL_INDEX_V(face, VOXEL_FACE_COUNT, Vector3());

	return Vector3(face_light_direction_table[face][0], face_light_direction_table[face][1], face_light_direction_table[face][2]);
}

Vector3 VoxelCubePoints::get_point(int index) {
	ERR_FAIL_INDEX_V(index, POINT_COUNT, Vector3());

	return _points[index];
}

Vector3 VoxelCubePoints::get_top_left_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P010];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P000];
	}
	return _points[0];
}

Vector3 VoxelCubePoints::get_top_right_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P110];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P100];
	}
	return _points[0];
}

Vector3 VoxelCubePoints::get_bottom_left_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P000];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P101];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P011];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P001];
	}
	return _points[0];
}

Vector3 VoxelCubePoints::get_bottom_right_point(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _points[P101];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _points[P100];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _points[P100];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _points[P001];
	}
	if (face == VOXEL_FACE_TOP) {
		return _points[P111];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _points[P101];
	}
	return _points[P000];
}

uint8_t VoxelCubePoints::get_face_type(int face) {
	if (face == VOXEL_FACE_BACK) {
		return _point_types[P111];
	}
	if (face == VOXEL_FACE_FRONT) {
		return _point_types[P110];
	}
	if (face == VOXEL_FACE_RIGHT) {
		return _point_types[P110];
	}
	if (face == VOXEL_FACE_LEFT) {
		return _point_types[P011];
	}
	if (face == VOXEL_FACE_TOP) {
		return _point_types[P110];
	}
	if (face == VOXEL_FACE_BOTTOM) {
		return _point_types[P100];
	}
	return _point_types[0];
}

bool VoxelCubePoints::has_points() {
	return (_point_types[P000] != 0 && _point_types[P100] != 0 && _point_types[P010] != 0 && _point_types[P001] != 0 &&
			_point_types[P110] != 0 && _point_types[P011] != 0 && _point_types[P101] != 0 && _point_types[P111] != 0);

	//return !(_point_types[P000] == 0 && _point_types[P100] == 0 && _point_types[P010] == 0 && _point_types[P001] == 0 &&
	//		 _point_types[P110] == 0 && _point_types[P011] == 0 && _point_types[P101] == 0 && _point_types[P111] == 0);
}

int VoxelCubePoints::get_opposite_face(int face) {
	if (face == VOXEL_FACE_FRONT) {
		return VOXEL_FACE_BACK;
	}
	if (face == VOXEL_FACE_BACK) {
		return VOXEL_FACE_FRONT;
	}
	if (face == VOXEL_FACE_LEFT) {
		return VOXEL_FACE_RIGHT;
	}
	if (face == VOXEL_FACE_RIGHT) {
		return VOXEL_FACE_LEFT;
	}
	if (face == VOXEL_FACE_TOP) {
		return VOXEL_FACE_BOTTOM;
	}

	return VOXEL_FACE_BOTTOM;
}

VoxelCubePoints::VoxelCubePoints() {
	_channel_index_type = 0;
	_channel_index_isolevel = 0;

	reset();
}

VoxelCubePoints::~VoxelCubePoints() {
}

void VoxelCubePoints::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_x"), &VoxelCubePoints::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "value"), &VoxelCubePoints::set_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");

	ClassDB::bind_method(D_METHOD("get_y"), &VoxelCubePoints::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "value"), &VoxelCubePoints::set_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");

	ClassDB::bind_method(D_METHOD("get_z"), &VoxelCubePoints::get_z);
	ClassDB::bind_method(D_METHOD("set_z", "value"), &VoxelCubePoints::set_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "z"), "set_z", "get_z");

	ClassDB::bind_method(D_METHOD("get_size"), &VoxelCubePoints::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &VoxelCubePoints::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_channel_index_type"), &VoxelCubePoints::get_channel_index_type);
	ClassDB::bind_method(D_METHOD("set_channel_index_type", "value"), &VoxelCubePoints::set_channel_index_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_index_type"), "set_channel_index_type", "get_channel_index_type");

	ClassDB::bind_method(D_METHOD("get_channel_index_isolevel"), &VoxelCubePoints::get_channel_index_isolevel);
	ClassDB::bind_method(D_METHOD("set_channel_index_isolevel", "value"), &VoxelCubePoints::set_channel_index_isolevel);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_index_isolevel"), "set_channel_index_isolevel", "get_channel_index_isolevel");

	ClassDB::bind_method(D_METHOD("refresh_points"), &VoxelCubePoints::refresh_points);
	ClassDB::bind_method(D_METHOD("setup", "chunk", "x", "y", "z", "size"), &VoxelCubePoints::setup, DEFVAL(1));

	ClassDB::bind_method(D_METHOD("get_point_index", "face", "index"), &VoxelCubePoints::get_point_index);
	ClassDB::bind_method(D_METHOD("get_point_uv_direction", "face", "index"), &VoxelCubePoints::get_point_uv_direction);

	ClassDB::bind_method(D_METHOD("get_points_for_face", "face", "index"), &VoxelCubePoints::get_points_for_face);

	ClassDB::bind_method(D_METHOD("is_face_visible", "face"), &VoxelCubePoints::is_face_visible);

	ClassDB::bind_method(D_METHOD("is_sub_voxel_point", "x", "y", "z"), &VoxelCubePoints::is_sub_voxel_point);
	ClassDB::bind_method(D_METHOD("set_point", "point", "x", "y", "z"), &VoxelCubePoints::set_point);
	ClassDB::bind_method(D_METHOD("get_point_id", "x", "y", "z"), &VoxelCubePoints::get_point_id);

	ClassDB::bind_method(D_METHOD("get_point_for_face", "face", "index"), &VoxelCubePoints::get_point_for_face);
	ClassDB::bind_method(D_METHOD("get_vertex_vector3_for_point", "face", "index"), &VoxelCubePoints::get_vertex_vector3_for_point);

	ClassDB::bind_method(D_METHOD("get_point_type", "index"), &VoxelCubePoints::get_point_type);
	ClassDB::bind_method(D_METHOD("get_point_fill", "index"), &VoxelCubePoints::get_point_fill);
	ClassDB::bind_method(D_METHOD("get_point_neighbours", "index"), &VoxelCubePoints::get_point_neighbours);

	ClassDB::bind_method(D_METHOD("get_point_ao", "index"), &VoxelCubePoints::get_point_ao);
	ClassDB::bind_method(D_METHOD("get_face_point_ao", "face", "index"), &VoxelCubePoints::get_face_point_ao);
	ClassDB::bind_method(D_METHOD("get_face_point_ao_color", "face", "index"), &VoxelCubePoints::get_face_point_ao_color);
	ClassDB::bind_method(D_METHOD("get_face_point_light_color", "face", "index"), &VoxelCubePoints::get_face_point_light_color);
	ClassDB::bind_method(D_METHOD("get_face_point_color_mixed", "face", "index"), &VoxelCubePoints::get_face_point_color_mixed);

	ClassDB::bind_method(D_METHOD("get_face_light_direction", "face"), &VoxelCubePoints::get_face_light_direction);

	ClassDB::bind_method(D_METHOD("get_point", "index"), &VoxelCubePoints::get_point);

	ClassDB::bind_method(D_METHOD("get_top_left_point", "face"), &VoxelCubePoints::get_top_left_point);
	ClassDB::bind_method(D_METHOD("get_top_right_point", "face"), &VoxelCubePoints::get_top_right_point);
	ClassDB::bind_method(D_METHOD("get_bottom_left_point", "face"), &VoxelCubePoints::get_bottom_left_point);
	ClassDB::bind_method(D_METHOD("get_bottom_right_point", "face"), &VoxelCubePoints::get_bottom_right_point);

	ClassDB::bind_method(D_METHOD("get_face_type", "face"), &VoxelCubePoints::get_face_type);

	ClassDB::bind_method(D_METHOD("has_points"), &VoxelCubePoints::has_points);
	ClassDB::bind_method(D_METHOD("get_opposite_face", "face"), &VoxelCubePoints::get_opposite_face);

	BIND_ENUM_CONSTANT(P000);
	BIND_ENUM_CONSTANT(P100);
	BIND_ENUM_CONSTANT(P010);
	BIND_ENUM_CONSTANT(P001);
	BIND_ENUM_CONSTANT(P110);
	BIND_ENUM_CONSTANT(P011);
	BIND_ENUM_CONSTANT(P101);
	BIND_ENUM_CONSTANT(P111);
	BIND_ENUM_CONSTANT(POINT_COUNT);

	BIND_ENUM_CONSTANT(VOXEL_FACE_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_BACK);
	BIND_ENUM_CONSTANT(VOXEL_FACE_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_FACE_TOP);
	BIND_ENUM_CONSTANT(VOXEL_FACE_BOTTOM);
	BIND_ENUM_CONSTANT(VOXEL_FACE_COUNT);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_NONE);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_BOTTOM_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_LEFT_BACK);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT_FRONT);
	BIND_ENUM_CONSTANT(VOXEL_NEIGHBOUR_TOP_RIGHT_BACK);

	BIND_ENUM_CONSTANT(VOXEL_FULL_NEIGHBOURS_CROSS);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS_TOP);
	BIND_ENUM_CONSTANT(VOXEL_FULL_SIDE_NEIGHBOURS_DOWN);
}
