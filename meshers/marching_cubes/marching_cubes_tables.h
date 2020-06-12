//Please note, that these tables were modified to get support for uv-mapping!

//================================================================================
//
// The MarchingCubes Algorithm look-up tables
//
// Copyright 2009 by Eric Lengyel
//
// The following data originates from Eric Lengyel's MarchingCubes Algorithm.
// http://transvoxel.org/
//
// The data in this file may be freely used in implementations of the MarchingCubes
// Algorithm. If you do use this data, or any transformation of it, in your own
// projects, commercial or otherwise, please give credit by indicating in your
// source code that the data is part of the author's implementation of the
// MarchingCubes Algorithm and that it came from the web address given above.
// (Simply copying and pasting the two lines of the previous paragraph would be
// perfect.) If you distribute a commercial product with source code included,
// then the credit in the source code is required.
//
// If you distribute any kind of product that uses this data, a credit visible to
// the end-user would be appreciated, but it is not required. However, you may
// not claim that the entire implementation of the MarchingCubes Algorithm is your
// own if you use the data in this file or any transformation of it.
//
// The format of the data in this file is described in the dissertation "Voxel-
// Based Terrain for Real-Time Virtual Simulations", available at the web page
// given above. References to sections and figures below pertain to that paper.
//
// The contents of this file are protected by copyright and may not be publicly
// reproduced without permission.
//
//================================================================================

#ifndef MARCHING_CUBES_TABLES_H
#define MARCHING_CUBES_TABLES_H

#include "core/math/vector3.h"

namespace MarchingCubes {

// The RegularCellData structure holds information about the triangulation
// used for a single equivalence class in the modified Marching Cubes algorithm,
// described in Section 3.2.

struct RegularCellData {
	unsigned char geometryCounts; // High nibble is vertex count, low nibble is triangle count.
	unsigned char vertexIndex[15]; // Groups of 3 indexes giving the triangulation.

	long GetVertexCount(void) const {
		return (geometryCounts >> 4);
	}

	long GetTriangleCount(void) const {
		return (geometryCounts & 0x0F);
	}
};

extern const unsigned char regularCellClass[256];
extern const RegularCellData regularCellData[16];
extern const unsigned short regularVertexData[256][15];

extern const Vector3 marching_cube_vertices[8];

} // namespace MarchingCubes

#endif
