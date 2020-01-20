#include "mesh_simplifier.h"

#include "../voxel_mesher.h"

//Mesh Simplification
//Mesh Simplification
//Ported from https://github.com/Whinarn/UnityMeshSimplifier
//Original license: MIT License Copyright (c) 2017 Mattias Edlund
void MeshSimplifier::initialize(Ref<VoxelMesher> mesher) {
	_mesher = mesher;

	_vertices = mesher->get_vertices();
	_normals = mesher->get_normals();
	_colors = mesher->get_colors();
	_uvs = mesher->get_uvs();
	_uv2s = mesher->get_uv2s();
	_indices = mesher->get_indices();

	if ((_indices.size() % 3) != 0)
		ERR_FAIL_MSG("The index array length must be a multiple of 3 in order to represent triangles.");

	int triangle_count = _indices.size() / 3;
	_mu_triangles.resize(triangle_count);

	for (int i = 0; i < triangle_count; ++i) {
		int offset = i * 3;
		int v0 = _indices[offset];
		int v1 = _indices[offset + 1];
		int v2 = _indices[offset + 2];
		_mu_triangles[i] = MUTriangle(v0, v1, v2, 0);
	}

	for (int i = 0; i < _vertices.size(); ++i) {
		_mu_vertices.append(MUVertex(_vertices[i]));
	}
}

//private ResizableArray<Triangle> triangles = null;
//private ResizableArray<Vertex> vertices = null;

//Mesh Simplification
//Ported from https://github.com/Whinarn/UnityMeshSimplifier
//Original license: MIT License Copyright (c) 2017 Mattias Edlund
void MeshSimplifier::SimplifyMesh(float quality) {
	quality = CLAMP(quality, 0, 1);

	int deletedTris = 0;
	PoolVector<bool> deleted0;
	deleted0.resize(20);
	PoolVector<bool> deleted1;
	deleted1.resize(20);

	int startTrisCount = _mu_triangles.size();
	int targetTrisCount = static_cast<int>(_mu_triangles.size() * quality + 0.5);

	for (int iteration = 0; iteration < maxIterationCount; iteration++) {
		if ((startTrisCount - deletedTris) <= targetTrisCount)
			break;

		// Update mesh once in a while
		if ((iteration % 5) == 0) {
			UpdateMesh(iteration);
		}

		// Clear dirty flag
		for (int i = 0; i < _mu_triangles.size(); ++i) {
			_mu_triangles[i].dirty = false;
		}

		// All triangles with edges below the threshold will be removed
		//
		// The following numbers works well for most models.
		// If it does not, try to adjust the 3 parameters
		double threshold = 0.000000001 * Math::pow(iteration + 3, agressiveness);

		//print_verbose("iteration {0} - triangles {1} threshold {2}", iteration, (startTrisCount - deletedTris), threshold);

		// Remove vertices & mark deleted triangles
		deletedTris = RemoveVertexPass(startTrisCount, targetTrisCount, threshold, deleted0, deleted1, deletedTris);
	}

	CompactMesh();

	//print_verbose("Finished simplification with triangle count {0}", _mu_triangles.size());
}

//Mesh Simplification
//Ported from https://github.com/Whinarn/UnityMeshSimplifier
//Original license: MIT License Copyright (c) 2017 Mattias Edlund
void MeshSimplifier::SimplifyMeshLossless() {
	int deletedTris = 0;
	PoolVector<bool> deleted0;
	PoolVector<bool> deleted1;
	int startTrisCount = _mu_triangles.size();

	for (int iteration = 0; iteration < 9999; iteration++) {
		// Update mesh constantly
		UpdateMesh(iteration);

		// Clear dirty flag
		for (int i = 0; i < _mu_triangles.size(); ++i) {
			_mu_triangles[i].dirty = false;
		}

		// All triangles with edges below the threshold will be removed
		//
		// The following numbers works well for most models.
		// If it does not, try to adjust the 3 parameters
		double threshold = 1.0E-3;

		//Debug.LogFormat("Lossless iteration {0} - triangles {1}", iteration, triangleCount);

		// Remove vertices & mark deleted triangles
		deletedTris = RemoveVertexPass(startTrisCount, 0, threshold, deleted0, deleted1, deletedTris);

		if (deletedTris <= 0)
			break;

		deletedTris = 0;
	}

	CompactMesh();

	//Debug.LogFormat("Finished simplification with triangle count {0}", this.triangles.Length);
}

void MeshSimplifier::UpdateMesh(int iteration) {
	if (iteration > 0) // compact triangles
	{
		int dst = 0;
		for (int i = 0; i < _mu_triangles.size(); ++i) {
			if (!_mu_triangles[i].deleted) {
				if (dst != i) {
					_mu_triangles[dst] = _mu_triangles[i];
				}
				dst++;
			}
		}
		_mu_triangles.resize(dst);
	}

	UpdateReferences();

	// Identify boundary : vertices[].border=0,1
	if (iteration == 0) {
		PoolVector<int> vcount;
		vcount.resize(8);
		PoolVector<int> vids;
		vids.resize(8);

		int vsize = 0;
		for (int i = 0; i < _mu_vertices.size(); i++) {
			_mu_vertices[i].borderEdge = false;
			_mu_vertices[i].uvSeamEdge = false;
			_mu_vertices[i].uvFoldoverEdge = false;
		}

		int ofs;
		int id;
		int borderVertexCount = 0;
		double borderMinX = std::numeric_limits<double>::max();
		double borderMaxX = std::numeric_limits<double>::min();

		for (int i = 0; i < _mu_vertices.size(); i++) {
			int tstart = _mu_vertices[i].tstart;
			int tcount = _mu_vertices[i].tcount;
			vcount.resize(0);
			vids.resize(0);
			vsize = 0;

			for (int j = 0; j < tcount; j++) {
				int tid = _mu_refs[tstart + j].tid;
				for (int k = 0; k < 3; k++) {
					ofs = 0;
					id = _mu_triangles[tid].get(k);
					while (ofs < vsize) {
						if (vids[ofs] == id)
							break;

						++ofs;
					}

					if (ofs == vsize) {
						vcount.push_back(1);
						vids.push_back(id);
						++vsize;
					} else {
						vcount.set(ofs, vcount[ofs] + 1);
					}
				}
			}

			for (int j = 0; j < vsize; j++) {
				if (vcount[j] == 1) {
					id = vids[j];
					_mu_vertices[id].borderEdge = true;
					++borderVertexCount;

					if (enableSmartLink) {
						if (_mu_vertices[id].p.x < borderMinX) {
							borderMinX = _mu_vertices[id].p.x;
						}
						if (_mu_vertices[id].p.x > borderMaxX) {
							borderMaxX = _mu_vertices[id].p.x;
						}
					}
				}
			}
		}

		if (enableSmartLink) {
			// First find all border vertices
			Vector<BorderVertex> borderVertices;
			borderVertices.resize(borderVertexCount);
			int borderIndexCount = 0;
			double borderAreaWidth = borderMaxX - borderMinX;
			for (int i = 0; i < _mu_vertices.size(); i++) {
				if (_mu_vertices[i].borderEdge) {
					int vertexHash = (int)(((((_mu_vertices[i].p.x - borderMinX) / borderAreaWidth) * 2.0) - 1.0) * std::numeric_limits<int>::max());
					borderVertices.set(borderIndexCount, BorderVertex(i, vertexHash));
					++borderIndexCount;
				}
			}

			// Sort the border vertices by hash
			borderVertices.sort_custom<BorderVertexComparer>();

			// Calculate the maximum hash distance based on the maximum vertex link distance
			double vertexLinkDistance = Math::sqrt(vertexLinkDistanceSqr);
			int hashMaxDistance = MAX((int)((vertexLinkDistance / borderAreaWidth) * std::numeric_limits<int>::max()), 1);

			// Then find identical border vertices and bind them together as one
			for (int i = 0; i < borderIndexCount; i++) {
				int myIndex = borderVertices[i].index;
				if (myIndex == -1)
					continue;

				Vector3 myPoint = _mu_vertices[myIndex].p;
				for (int j = i + 1; j < borderIndexCount; j++) {
					int otherIndex = borderVertices[j].index;
					if (otherIndex == -1)
						continue;
					else if ((borderVertices[j].hash - borderVertices[i].hash) > hashMaxDistance) // There is no point to continue beyond this point
						break;

					Vector3 otherPoint = _mu_vertices[otherIndex].p;
					double sqrX = ((myPoint.x - otherPoint.x) * (myPoint.x - otherPoint.x));
					double sqrY = ((myPoint.y - otherPoint.y) * (myPoint.y - otherPoint.y));
					double sqrZ = ((myPoint.z - otherPoint.z) * (myPoint.z - otherPoint.z));
					double sqrMagnitude = sqrX + sqrY + sqrZ;

					if (sqrMagnitude <= vertexLinkDistanceSqr) {
						borderVertices.get(j).index = -1; // NOTE: This makes sure that the "other" vertex is not processed again
						_mu_vertices[myIndex].borderEdge = false;
						_mu_vertices[otherIndex].borderEdge = false;

						if (AreUVsTheSame(0, myIndex, otherIndex)) {
							_mu_vertices[myIndex].uvFoldoverEdge = true;
							_mu_vertices[otherIndex].uvFoldoverEdge = true;
						} else {
							_mu_vertices[myIndex].uvSeamEdge = true;
							_mu_vertices[otherIndex].uvSeamEdge = true;
						}

						int otherTriangleCount = _mu_vertices[otherIndex].tcount;
						int otherTriangleStart = _mu_vertices[otherIndex].tstart;
						for (int k = 0; k < otherTriangleCount; k++) {
							MURef r = _mu_refs[otherTriangleStart + k];
							_mu_triangles[r.tid].set(myIndex, r.tvertex);
						}
					}
				}
			}

			// Update the references again
			UpdateReferences();
		}

		// Init Quadrics by Plane & Edge Errors
		//
		// required at the beginning ( iteration == 0 )
		// recomputing during the simplification is not required,
		// but mostly improves the result for closed meshes
		for (int i = 0; i < _mu_vertices.size(); ++i) {
			_mu_vertices[i].q.reset();
		}

		int v0, v1, v2;
		Vector3 n, p0, p1, p2, p10, p20, dummy;
		SymmetricMatrix sm;
		for (int i = 0; i < _mu_triangles.size(); ++i) {
			v0 = _mu_triangles[i].v0;
			v1 = _mu_triangles[i].v1;
			v2 = _mu_triangles[i].v2;

			p0 = _mu_vertices[v0].p;
			p1 = _mu_vertices[v1].p;
			p2 = _mu_vertices[v2].p;
			p10 = p1 - p0;
			p20 = p2 - p0;

			n = p10.cross(p20);

			n.normalize();
			_mu_triangles[i].n = n;

			sm.from_plane(n.x, n.y, n.z, -n.dot(p0));
			_mu_vertices[v0].q += sm;
			_mu_vertices[v1].q += sm;
			_mu_vertices[v2].q += sm;
		}

		for (int i = 0; i < _mu_triangles.size(); ++i) {
			// Calc Edge Error
			MUTriangle triangle = _mu_triangles[i];
			_mu_triangles[i].err0 = CalculateError(_mu_vertices[triangle.v0], _mu_vertices[triangle.v1], &dummy);
			_mu_triangles[i].err1 = CalculateError(_mu_vertices[triangle.v1], _mu_vertices[triangle.v2], &dummy);
			_mu_triangles[i].err2 = CalculateError(_mu_vertices[triangle.v2], _mu_vertices[triangle.v0], &dummy);
			_mu_triangles[i].err3 = MeshSimplifier::Min3(_mu_triangles[i].err0, _mu_triangles[i].err1, _mu_triangles[i].err2);
		}
	}
}

void MeshSimplifier::UpdateReferences() {
	// Init Reference ID list
	for (int i = 0; i < _mu_vertices.size(); i++) {
		_mu_vertices[i].tstart = 0;
		_mu_vertices[i].tcount = 0;
	}

	for (int i = 0; i < _mu_triangles.size(); i++) {
		++_mu_vertices[_mu_triangles[i].v0].tcount;
		++_mu_vertices[_mu_triangles[i].v1].tcount;
		++_mu_vertices[_mu_triangles[i].v2].tcount;
	}

	int tstart = 0;
	for (int i = 0; i < _mu_vertices.size(); i++) {
		_mu_vertices[i].tstart = tstart;
		tstart += _mu_vertices[i].tcount;
		_mu_vertices[i].tcount = 0;
	}

	// Write References
	_mu_refs.resize(tstart);
	for (int i = 0; i < _mu_triangles.size(); i++) {
		int v0 = _mu_triangles[i].v0;
		int v1 = _mu_triangles[i].v1;
		int v2 = _mu_triangles[i].v2;
		int start0 = _mu_vertices[v0].tstart;
		int count0 = _mu_vertices[v0].tcount;
		int start1 = _mu_vertices[v1].tstart;
		int count1 = _mu_vertices[v1].tcount;
		int start2 = _mu_vertices[v2].tstart;
		int count2 = _mu_vertices[v2].tcount;

		_mu_refs[start0 + count0].Set(i, 0);
		_mu_refs[start1 + count1].Set(i, 1);
		_mu_refs[start2 + count2].Set(i, 2);

		++_mu_vertices[v0].tcount;
		++_mu_vertices[v1].tcount;
		++_mu_vertices[v2].tcount;
	}
}

/// <summary>
/// Finally compact mesh before exiting.
/// </summary>
void MeshSimplifier::CompactMesh() {
	int dst = 0;

	for (int i = 0; i < _mu_vertices.size(); i++) {
		_mu_vertices[i].tcount = 0;
	}

	for (int i = 0; i < _mu_triangles.size(); i++) {
		MUTriangle triangle = _mu_triangles[i];

		if (!triangle.deleted) {
			if (triangle.va0 != triangle.v0) {
				int iDest = triangle.va0;
				int iSrc = triangle.v0;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;

				triangle.v0 = triangle.va0;
			}

			if (triangle.va1 != triangle.v1) {
				int iDest = triangle.va1;
				int iSrc = triangle.v1;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;

				triangle.v1 = triangle.va1;
			}

			if (triangle.va2 != triangle.v2) {
				int iDest = triangle.va2;
				int iSrc = triangle.v2;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;

				triangle.v2 = triangle.va2;
			}

			int newTriangleIndex = ++dst;
			_mu_triangles[newTriangleIndex] = triangle;

			_mu_vertices[triangle.v0].tcount = 1;
			_mu_vertices[triangle.v1].tcount = 1;
			_mu_vertices[triangle.v2].tcount = 1;
		}
	}

	_mu_triangles.resize(dst);

	dst = 0;
	for (int i = 0; i < _mu_vertices.size(); i++) {
		MUVertex vert = _mu_vertices[i];

		if (vert.tcount > 0) {
			vert.tstart = dst;
			_mu_vertices[i] = vert;

			if (dst != i) {
				_mu_vertices[dst].p = vert.p;

				if (_normals.size() > 0) _normals[dst] = _normals[i];

				if (_colors.size() > 0) _colors.set(dst, _colors[i]);
				if (_uvs.size() > 0) _uvs.set(dst, _uvs[i]);
				if (_uv2s.size() > 0) _uv2s.set(dst, _uv2s[i]);
				if (_indices.size() > 0) _indices.set(dst, _indices[i]);
			}

			++dst;
		}
	}

	for (int i = 0; i < _mu_triangles.size(); i++) {
		MUTriangle triangle = _mu_triangles[i];
		triangle.v0 = _mu_vertices[triangle.v0].tstart;
		triangle.v1 = _mu_vertices[triangle.v1].tstart;
		triangle.v2 = _mu_vertices[triangle.v2].tstart;
		_mu_triangles[i] = triangle;
	}

	//vertexCount = dst;
	_vertices.resize(dst);
	if (_normals.size() > 0) _normals.resize(dst);
	if (_colors.size() > 0) _colors.resize(dst);
	if (_uvs.size() > 0) _uvs.resize(dst);
	if (_uv2s.size() > 0) _uv2s.resize(dst);
	if (_indices.size() > 0) _indices.resize(dst);
}

bool MeshSimplifier::AreUVsTheSame(int channel, int indexA, int indexB) {
	if (_uv2s.size() > 0) {
		Vector2 vertUV = _uv2s[channel];

		Vector2 uvA = _uv2s[indexA];
		Vector2 uvB = _uv2s[indexB];
		return uvA == uvB;
	}

	return false;
}

/// Remove vertices and mark deleted triangles
int MeshSimplifier::RemoveVertexPass(int startTrisCount, int targetTrisCount, double threshold, PoolVector<bool> deleted0, PoolVector<bool> deleted1, int deletedTris) {
	Vector3 p;
	Vector3 barycentricCoord;
	for (int tid = 0; tid < _mu_triangles.size(); tid++) {
		if (_mu_triangles[tid].dirty || _mu_triangles[tid].deleted || _mu_triangles[tid].err3 > threshold)
			continue;

		Vector3 errors = _mu_triangles[tid].GetErrors();
		Vector3 attrib_indices = _mu_triangles[tid].GetAttributeIndices();
		for (int edgeIndex = 0; edgeIndex < 3; edgeIndex++) {
			if (errors[edgeIndex] > threshold)
				continue;

			int nextEdgeIndex = ((edgeIndex + 1) % 3);
			int i0 = _mu_triangles[tid].get(edgeIndex);
			int i1 = _mu_triangles[tid].get(nextEdgeIndex);

			// Border check
			if (_mu_vertices[i0].borderEdge != _mu_vertices[i1].borderEdge)
				continue;

			// Seam check
			else if (_mu_vertices[i0].uvSeamEdge != _mu_vertices[i1].uvSeamEdge)
				continue;
			// Foldover check
			else if (_mu_vertices[i0].uvFoldoverEdge != _mu_vertices[i1].uvFoldoverEdge)
				continue;
			// If borders should be preserved
			else if (preserveBorderEdges && _mu_vertices[i0].borderEdge)
				continue;
			// If seams should be preserved
			else if (preserveUVSeamEdges && _mu_vertices[i0].uvSeamEdge)
				continue;
			// If foldovers should be preserved
			else if (preserveUVFoldoverEdges && _mu_vertices[i0].uvFoldoverEdge)
				continue;

			// Compute vertex to collapse to
			CalculateError(_mu_vertices[i0], _mu_vertices[i1], &p);
			deleted0.resize(_mu_vertices[i0].tcount); // normals temporarily
			deleted1.resize(_mu_vertices[i1].tcount); // normals temporarily

			// Don't remove if flipped
			if (Flipped(&p, i0, i1, &(_mu_vertices[i0]), deleted0))
				continue;
			if (Flipped(&p, i1, i0, &(_mu_vertices[i1]), deleted1))
				continue;

			// Calculate the barycentric coordinates within the triangle
			int nextNextEdgeIndex = ((edgeIndex + 2) % 3);
			int i2 = _mu_triangles[tid].get(nextNextEdgeIndex);
			barycentricCoord = CalculateBarycentricCoords(p, _mu_vertices[i0].p, _mu_vertices[i1].p, _mu_vertices[i2].p);

			// Not flipped, so remove edge
			_mu_vertices[i0].p = p;
			_mu_vertices[i0].q += _mu_vertices[i1].q;

			// Interpolate the vertex attributes
			int ia0 = attrib_indices[edgeIndex];
			int ia1 = attrib_indices[nextEdgeIndex];
			int ia2 = attrib_indices[nextNextEdgeIndex];
			InterpolateVertexAttributes(ia0, ia0, ia1, ia2, barycentricCoord);

			if (_mu_vertices[i0].uvSeamEdge) {
				ia0 = -1;
			}

			int tstart = _mu_refs.size();
			deletedTris = UpdateTriangles(i0, ia0, &(_mu_vertices[i0]), deleted0, deletedTris);
			deletedTris = UpdateTriangles(i0, ia0, &(_mu_vertices[i1]), deleted1, deletedTris);

			int tcount = _mu_refs.size() - tstart;
			if (tcount <= _mu_vertices[i0].tcount) {
				// save ram
				if (tcount > 0) {
					int dests = _mu_vertices[i0].tstart;
					for (int v = 0; v < tcount; ++v) {
						_mu_refs[v + tstart] = _mu_refs[v + dests];
					}
				}
			} else {
				// append
				_mu_vertices[i0].tstart = tstart;
			}

			_mu_vertices[i0].tcount = tcount;
			break;
		}

		// Check if we are already done
		if ((startTrisCount - deletedTris) <= targetTrisCount)
			break;
	}

	return deletedTris;
}

double MeshSimplifier::VertexError(SymmetricMatrix q, double x, double y, double z) {
	return q.m0 * x * x + 2 * q.m1 * x * y + 2 * q.m2 * x * z + 2 * q.m3 * x + q.m4 * y * y + 2 * q.m5 * y * z + 2 * q.m6 * y + q.m7 * z * z + 2 * q.m8 * z + q.m9;
}

double MeshSimplifier::CalculateError(MUVertex vert0, MUVertex vert1, Vector3 *result) {
	// compute interpolated vertex
	SymmetricMatrix q = (vert0.q + vert1.q);
	bool borderEdge = (vert0.borderEdge & vert1.borderEdge);
	double error = 0.0;
	double det = q.Determinant1();
	if (det != 0.0 && !borderEdge) {
		// q_delta is invertible
		result = new Vector3(
				-1.0 / det * q.Determinant2(), // vx = A41/det(q_delta)
				1.0 / det * q.Determinant3(), // vy = A42/det(q_delta)
				-1.0 / det * q.Determinant4()); // vz = A43/det(q_delta)
		error = VertexError(q, result->x, result->y, result->z);
	} else {
		// det = 0 -> try to find best result
		Vector3 p1 = vert0.p;
		Vector3 p2 = vert1.p;
		Vector3 p3 = (p1 + p2) * 0.5f;
		double error1 = VertexError(q, p1.x, p1.y, p1.z);
		double error2 = VertexError(q, p2.x, p2.y, p2.z);
		double error3 = VertexError(q, p3.x, p3.y, p3.z);

		error = MeshSimplifier::Min3(error1, error2, error3);
		if (error == error3) {
			result->x = p3.x;
			result->y = p3.y;
			result->z = p3.z;
		} else if (error == error2) {
			result->x = p2.x;
			result->y = p2.y;
			result->z = p2.z;
		} else if (error == error1) {
			result->x = p1.x;
			result->y = p1.y;
			result->z = p1.z;
		} else {
			result->x = p3.x;
			result->y = p3.y;
			result->z = p3.z;
		}
	}
	return error;
}

int MeshSimplifier::UpdateTriangles(int i0, int ia0, MUVertex *v, PoolVector<bool> deleted, int p_deletedTriangles) {
	Vector3 p;
	int deletedTriangles = p_deletedTriangles;
	int tcount = v->tcount;

	for (int k = 0; k < tcount; k++) {
		MURef r = _mu_refs[v->tstart + k];
		int tid = r.tid;
		MUTriangle t = _mu_triangles[tid];
		if (t.deleted)
			continue;

		if (deleted[k]) {
			_mu_triangles[tid].deleted = true;
			++deletedTriangles;
			continue;
		}

		t.set(r.tvertex, i0);
		if (ia0 != -1) {
			t.SetAttributeIndex(r.tvertex, ia0);
		}

		t.dirty = true;
		t.err0 = CalculateError(_mu_vertices[t.v0], _mu_vertices[t.v1], &p);
		t.err1 = CalculateError(_mu_vertices[t.v1], _mu_vertices[t.v2], &p);
		t.err2 = CalculateError(_mu_vertices[t.v2], _mu_vertices[t.v0], &p);
		t.err3 = MeshSimplifier::Min3(t.err0, t.err1, t.err2);

		_mu_triangles[tid] = t;
		_mu_refs.push_back(r);
	}

	return deletedTriangles;
}

bool MeshSimplifier::Flipped(Vector3 *p, int i0, int i1, MUVertex *v0, PoolVector<bool> &deleted) {
	int tcount = v0->tcount;

	for (int k = 0; k < tcount; k++) {
		MURef r = _mu_refs[v0->tstart + k];
		if (_mu_triangles[r.tid].deleted)
			continue;

		int s = r.tvertex;
		int id1 = _mu_triangles[r.tid].get((s + 1) % 3);
		int id2 = _mu_triangles[r.tid].get((s + 2) % 3);
		if (id1 == i1 || id2 == i1) {
			deleted.set(k, true);
			continue;
		}

		Vector3 d1 = _mu_vertices[id1].p - (*p);
		d1.normalize();
		Vector3 d2 = _mu_vertices[id2].p - (*p);
		d2.normalize();
		double dot = d1.dot(d2);
		if (Math::abs(dot) > 0.999)
			return true;

		Vector3 n = d1.cross(d2);
		n.normalize();
		deleted.set(k, false);
		dot = n.dot(_mu_triangles[r.tid].n);
		if (dot < 0.2)
			return true;
	}

	return false;
}

Vector3 MeshSimplifier::CalculateBarycentricCoords(Vector3 const &point, Vector3 const &a, Vector3 const &b, Vector3 const &c) {
	Vector3 v0 = (Vector3)(b - a), v1 = (Vector3)(c - a), v2 = (Vector3)(point - a);
	float d00 = v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0 - v - w;

	return Vector3(u, v, w);
}

void MeshSimplifier::InterpolateVertexAttributes(int dst, int i0, int i1, int i2, Vector3 &barycentricCoord) {
	if (_normals.size() > 0) {
		_normals[dst] = (_normals[i0] * barycentricCoord.x) + (_normals[i1] * barycentricCoord.y) + (_normals[i2] * barycentricCoord.z).normalized();
	}

	if (_uvs.size() > 0) {
		_uvs[dst] = (_uvs[i0] * barycentricCoord.x) + (_uvs[i1] * barycentricCoord.y) + (_uvs[i2] * barycentricCoord.z);
	}

	if (_uv2s.size() > 0) {
		_uv2s[dst] = (_uv2s[i0] * barycentricCoord.x) + (_uv2s[i1] * barycentricCoord.y) + (_uv2s[i2] * barycentricCoord.z);
	}

	if (_colors.size() > 0) {
		_colors[dst] = (_colors[i0] * barycentricCoord.x) + (_colors[i1] * barycentricCoord.y) + (_colors[i2] * barycentricCoord.z);
	}
}

MeshSimplifier::MeshSimplifier() {
	maxIterationCount = 100;
	agressiveness = 7.0;
	enableSmartLink = true;
	preserveBorderEdges = false;
	preserveUVSeamEdges = false;
	preserveUVFoldoverEdges = false;
}