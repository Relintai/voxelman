#ifndef VOXELMAN_MESH_UTILS_H
#define VOXELMAN_MESH_UTILS_H

/*
Ported from https://github.com/Whinarn/UnityMeshSimplifier

MIT License

Copyright (c) 2020 Péter Magyar
Copyright(c) 2017-2020 Mattias Edlund

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

#include "core/math/vector3.h"

/// A symmetric matrix.
struct SymmetricMatrix {
	/// The m11 component.
	double m0;
	/// The m12 component.
	double m1;
	/// The m13 component.
	double m2;
	/// The m14 component.
	double m3;
	/// The m22 component.
	double m4;
	/// The m23 component.
	double m5;
	/// The m24 component.
	double m6;
	/// The m33 component.
	double m7;
	/// The m34 component.
	double m8;
	/// The m44 component.
	double m9;

	_FORCE_INLINE_ const double &operator[](int p_index) const {
		CRASH_BAD_INDEX(p_index, 10);

		switch (p_index) {
			case 0:
				return m0;
			case 1:
				return m1;
			case 2:
				return m2;
			case 3:
				return m3;
			case 4:
				return m4;
			case 5:
				return m5;
			case 6:
				return m6;
			case 7:
				return m7;
			case 8:
				return m8;
			case 9:
				return m9;
		}

		return 0;
	}

	SymmetricMatrix() {
		m0 = 0;
		m1 = 0;
		m2 = 0;
		m3 = 0;
		m4 = 0;
		m5 = 0;
		m6 = 0;
		m7 = 0;
		m8 = 0;
		m9 = 0;
	}

	/// Creates a symmetric matrix with a value in each component.
	SymmetricMatrix(double c) {
		m0 = c;
		m1 = c;
		m2 = c;
		m3 = c;
		m4 = c;
		m5 = c;
		m6 = c;
		m7 = c;
		m8 = c;
		m9 = c;
	}

	/// Creates a symmetric matrix.
	SymmetricMatrix(double p_m0, double p_m1, double p_m2, double p_m3,
			double p_m4, double p_m5, double p_m6, double p_m7, double p_m8, double p_m9) {
		m0 = p_m0;
		m1 = p_m1;
		m2 = p_m2;
		m3 = p_m3;
		m4 = p_m4;
		m5 = p_m5;
		m6 = p_m6;
		m7 = p_m7;
		m8 = p_m8;
		m9 = p_m9;
	}

	/// Creates a symmetric matrix from a plane.
	SymmetricMatrix(double a, double b, double c, double d) {
		m0 = a * a;
		m1 = a * b;
		m2 = a * c;
		m3 = a * d;

		m4 = b * b;
		m5 = b * c;
		m6 = b * d;

		m7 = c * c;
		m8 = c * d;

		m9 = d * d;
	}

	SymmetricMatrix operator+(const SymmetricMatrix &p_m) const {
		return SymmetricMatrix(m0 + p_m.m0, m1 + p_m.m1, m2 + p_m.m2, m3 + p_m.m3,
				m4 + p_m.m4, m5 + p_m.m5, m6 + p_m.m6,
				m7 + p_m.m7, m8 + p_m.m8,
				m9 + p_m.m9);
	}

	void operator+=(const SymmetricMatrix &p_m) {
		m0 += p_m.m0;
		m1 += p_m.m1;
		m2 += p_m.m2;
		m3 += p_m.m3;
		m4 += p_m.m4;
		m5 += p_m.m5;
		m6 += p_m.m6;
		m7 += p_m.m7;
		m8 += p_m.m8;
		m9 += p_m.m9;
	}

	/// Determinant(0, 1, 2, 1, 4, 5, 2, 5, 7)
	double Determinant1() {
		double det =
				m0 * m4 * m7 +
				m2 * m1 * m5 +
				m1 * m5 * m2 -
				m2 * m4 * m2 -
				m0 * m5 * m5 -
				m1 * m1 * m7;
		return det;
	}

	/// Determinant(1, 2, 3, 4, 5, 6, 5, 7, 8)
	double Determinant2() {
		double det =
				m1 * m5 * m8 +
				m3 * m4 * m7 +
				m2 * m6 * m5 -
				m3 * m5 * m5 -
				m1 * m6 * m7 -
				m2 * m4 * m8;
		return det;
	}

	double Determinant3() {
		double det =
				m0 * m5 * m8 +
				m3 * m1 * m7 +
				m2 * m6 * m2 -
				m3 * m5 * m2 -
				m0 * m6 * m7 -
				m2 * m1 * m8;
		return det;
	}

	/// Determinant(0, 1, 3, 1, 4, 6, 2, 5, 8)
	double Determinant4() {
		double det =
				m0 * m4 * m8 +
				m3 * m1 * m5 +
				m1 * m6 * m2 -
				m3 * m4 * m2 -
				m0 * m6 * m5 -
				m1 * m1 * m8;
		return det;
	}

	double Determinant(int a11, int a12, int a13,
			int a21, int a22, int a23,
			int a31, int a32, int a33) {
		double det =
				this[a11] * this[a22] * this[a33] +
				this[a13] * this[a21] * this[a32] +
				this[a12] * this[a23] * this[a31] -
				this[a13] * this[a22] * this[a31] -
				this[a11] * this[a23] * this[a32] -
				this[a12] * this[a21] * this[a33];
		return det;
	}

	void from_plane(double a, double b, double c, double d) {
		m0 = a * a;
		m1 = a * b;
		m2 = a * c;
		m3 = a * d;

		m4 = b * b;
		m5 = b * c;
		m6 = b * d;

		m7 = c * c;
		m8 = c * d;

		m9 = d * d;
	}

	void reset() {
		m0 = 0;
		m1 = 0;
		m2 = 0;
		m3 = 0;
		m4 = 0;
		m5 = 0;
		m6 = 0;
		m7 = 0;
		m8 = 0;
		m9 = 0;
	}
};

struct MUTriangle {
	int v0;
	int v1;
	int v2;
	int subMeshIndex;

	int va0;
	int va1;
	int va2;

	double err0;
	double err1;
	double err2;
	double err3;

	bool deleted;
	bool dirty;
	Vector3 n;

	_FORCE_INLINE_ int get(int p_index) {
		return (p_index == 0 ? v0 : (p_index == 1 ? v1 : v2));
	}

	_FORCE_INLINE_ int set(int p_index, int value) {
		CRASH_BAD_INDEX(p_index, 3);

		switch (p_index) {
			case 0:
				v0 = value;
				break;
			case 1:
				v1 = value;
				break;
			case 2:
				v2 = value;
				break;
		}

		return 0;
	}

	MUTriangle(int p_v0, int p_v1, int p_v2, int p_subMeshIndex) {
		v0 = p_v0;
		v1 = p_v1;
		v2 = p_v2;
		subMeshIndex = p_subMeshIndex;

		va0 = p_v0;
		va1 = p_v1;
		va2 = p_v2;

		err0 = err1 = err2 = err3 = 0;
		deleted = dirty = false;
	}

	Vector3 GetAttributeIndices() {
		Vector3 attributeIndices;

		attributeIndices[0] = va0;
		attributeIndices[1] = va1;
		attributeIndices[2] = va2;

		return attributeIndices;
	}

	void SetAttributeIndex(int index, int value) {
		CRASH_BAD_INDEX(index, 3);

		switch (index) {
			case 0:
				va0 = value;
				break;
			case 1:
				va1 = value;
				break;
			case 2:
				va2 = value;
				break;
		}
	}

	Vector3 GetErrors() {
		Vector3 err;

		err[0] = err0;
		err[1] = err1;
		err[2] = err2;

		return err;
	}
};

struct MUVertex {
	Vector3 p;
	int tstart;
	int tcount;
	SymmetricMatrix q;
	bool borderEdge;
	bool uvSeamEdge;
	bool uvFoldoverEdge;

	MUVertex(float x, float y, float z) {
		p = Vector3(x, y, z);
		tstart = 0;
		tcount = 0;
		borderEdge = true;
		uvSeamEdge = false;
		uvFoldoverEdge = false;
	}

	MUVertex(Vector3 point) {
		p = point;
		tstart = 0;
		tcount = 0;
		borderEdge = true;
		uvSeamEdge = false;
		uvFoldoverEdge = false;
	}
};

struct MURef {
	int tid;
	int tvertex;

	MURef() {
		tid = 0;
		tvertex = 0;
	}

	void Set(int p_tid, int p_tvertex) {
		tid = p_tid;
		tvertex = p_tvertex;
	}
};

struct BorderVertex {
	int index;
	int hash;

	BorderVertex() {
		index = 0;
		hash = 0;
	}

	BorderVertex(int p_index, int p_hash) {
		index = p_index;
		hash = p_hash;
	}
};

struct BorderVertexComparer {
	_FORCE_INLINE_ bool operator()(const BorderVertex &a, const BorderVertex &b) const { return x.hash < y.hash; }
};

#endif