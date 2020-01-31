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

#include "transvoxel_cell_data.h"

int TransvoxelCellData::get_vertex_index(int index) const {
	return static_cast<int>(vertexIndex[index]);
}
void TransvoxelCellData::set_vertex_index(int index, int value) {
	ERR_FAIL_INDEX(index, 36);

	vertexIndex[index] = static_cast<unsigned char>(value);
}

int TransvoxelCellData::get_vertex_count() const {
	return (geometryCounts >> 4);
}
void TransvoxelCellData::set_vertex_count(int value) {
	geometryCounts &= 0xFF0F;

	geometryCounts |= value << 4;
}

int TransvoxelCellData::get_triangle_count() const {
	return (geometryCounts & 0x0F);
}
void TransvoxelCellData::set_triangle_count(int value) {
	geometryCounts &= 0xFFF0;

	geometryCounts |= value;
}

TransvoxelCellData::TransvoxelCellData() {
	geometryCounts = 0;

	for (int i = 0; i < 36; ++i) {
		vertexIndex[i] = 0;
	}
}

TransvoxelCellData::TransvoxelCellData(const RegularCellData &cell_data) {
	geometryCounts = cell_data.geometryCounts;

	for (int i = 0; i < 15; ++i) {
		vertexIndex[i] = cell_data.vertexIndex[i];
	}
}

TransvoxelCellData::TransvoxelCellData(const TransitionCellData &cell_data) {
	geometryCounts = cell_data.geometryCounts;

	for (int i = 0; i < 36; ++i) {
		vertexIndex[i] = cell_data.vertexIndex[i];
	}
}

TransvoxelCellData::~TransvoxelCellData() {
}

void TransvoxelCellData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_vertex_index", "index"), &TransvoxelCellData::get_vertex_index);
	ClassDB::bind_method(D_METHOD("set_vertex_index", "index", "value"), &TransvoxelCellData::set_vertex_index);

	ClassDB::bind_method(D_METHOD("get_vertex_count"), &TransvoxelCellData::get_vertex_count);
	ClassDB::bind_method(D_METHOD("set_vertex_count", "value"), &TransvoxelCellData::set_vertex_count);

	ClassDB::bind_method(D_METHOD("get_triangle_count"), &TransvoxelCellData::get_triangle_count);
	ClassDB::bind_method(D_METHOD("set_triangle_count", "value"), &TransvoxelCellData::set_triangle_count);
}
