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

#ifndef MARCHING_CUBES_CELL_DATA_H
#define MARCHING_CUBES_CELL_DATA_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/ref_counted.h"
#ifndef Reference
#define Reference RefCounted
#endif
#else
#include "core/reference.h"
#endif


#include "marching_cubes_tables.h"

using namespace MarchingCubes;

class MarchingCubesCellData : public Reference {
	GDCLASS(MarchingCubesCellData, Reference)

public:
	int get_vertex_index(int index) const;
	void set_vertex_index(int index, int value);

	int get_vertex_count() const;
	void set_vertex_count(int value);

	int get_triangle_count() const;
	void set_triangle_count(int value);

	MarchingCubesCellData();
	MarchingCubesCellData(const MarchingCubes::RegularCellData &cell_data);
	~MarchingCubesCellData();

protected:
	static void _bind_methods();

private:
	long geometryCounts; // High nibble is vertex count, low nibble is triangle count.
	unsigned char vertexIndex[15]; // Groups of 3 indexes giving the triangulation.
};

#endif
