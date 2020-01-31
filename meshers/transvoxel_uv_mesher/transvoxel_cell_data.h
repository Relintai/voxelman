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

#ifndef TRANSVOXEL_CELL_DATA_H
#define TRANSVOXEL_CELL_DATA_H

#include "core/reference.h"

#include "transvoxel_tables.h"

using namespace Transvoxel;

class TransvoxelCellData : public Reference {
	GDCLASS(TransvoxelCellData, Reference)

public:
	int get_vertex_index(int index) const;
	void set_vertex_index(int index, int value);
	int get_vertex_count() const;
	void set_vertex_count(int value);
	int get_triangle_count() const;
	void set_triangle_count(int value);

	TransvoxelCellData();
	TransvoxelCellData(const RegularCellData &cell_data);
	TransvoxelCellData(const TransitionCellData &cell_data);
	~TransvoxelCellData();

protected:
	static void _bind_methods();

private:
	long geometryCounts; // High nibble is vertex count, low nibble is triangle count.
	unsigned char vertexIndex[36]; // Groups of 3 indexes giving the triangulation.
};

#endif
