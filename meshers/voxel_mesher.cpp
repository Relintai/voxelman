#include "voxel_mesher.h"

#include "../world/voxel_chunk.h"

Ref<VoxelmanLibrary> VoxelMesher::get_library() {
	return _library;
}
void VoxelMesher::set_library(Ref<VoxelmanLibrary> library) {
	_library = library;
}

Ref<Material> VoxelMesher::get_material() {
	return _material;
}
void VoxelMesher::set_material(Ref<Material> material) {
	_material = material;
}

float VoxelMesher::get_ao_strength() const {
	return _ao_strength;
}
void VoxelMesher::set_ao_strength(float value) {
	_ao_strength = value;
}

float VoxelMesher::get_base_light_value() const {
	return _base_light_value;
}
void VoxelMesher::set_base_light_value(float value) {
	_base_light_value = value;
}

float VoxelMesher::get_voxel_scale() const {
	return _voxel_scale;
}
void VoxelMesher::set_voxel_scale(const float voxel_scale) {
	_voxel_scale = voxel_scale;
}

int VoxelMesher::get_lod_size() const {
	return _lod_size;
}
void VoxelMesher::set_lod_size(const int lod_size) {
	_lod_size = lod_size;
}

Rect2 VoxelMesher::get_uv_margin() const {
	return _uv_margin;
}
void VoxelMesher::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

void VoxelMesher::build_mesh(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	VS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

	//if (_material.is_valid())
	//	_surface_tool->set_material(_material);

	if (_colors.size() != _vertices.size()) {
		print_error("Colors.size() != vertices.size() -> " + String::num(_colors.size()) + " " + String::num(_vertices.size()));

		_colors.resize(0);
	}

	int len = _vertices.size();

	for (int i = 0; i < len; ++i) {
		if (_normals.size() > 0) {
			_surface_tool->add_normal(_normals.get(i));
		}

		if (_colors.size() > 0) {
			_surface_tool->add_color(_colors.get(i));
		}

		if (_uvs.size() > 0) {
			_surface_tool->add_uv(_uvs.get(i));
		}

		_surface_tool->add_vertex(_vertices.get(i));
	}

	for (int i = 0; i < _indices.size(); ++i) {
		_surface_tool->add_index(_indices.get(i));
	}

	if (_normals.size() == 0) {
		_surface_tool->generate_normals();
	}

	Array arr = _surface_tool->commit_to_arrays();

	VS::get_singleton()->mesh_add_surface_from_arrays(mesh, VisualServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid())
		VS::get_singleton()->mesh_surface_set_material(mesh, 0, _library->get_material()->get_rid());
}

//Mesh Simplification
//Mesh Simplification
//Ported from https://github.com/Whinarn/UnityMeshSimplifier
//Original license: MIT License Copyright (c) 2017 Mattias Edlund
void VoxelMesher::initialize_mesh_simplify() {
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
}

//private ResizableArray<Triangle> triangles = null;
//private ResizableArray<Vertex> vertices = null;

//Mesh Simplification
//Ported from https://github.com/Whinarn/UnityMeshSimplifier
//Original license: MIT License Copyright (c) 2017 Mattias Edlund
void VoxelMesher::SimplifyMesh(float quality) {
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
void VoxelMesher::SimplifyMeshLossless() {
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

void VoxelMesher::UpdateMesh(int iteration) {
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
			_mu_triangles[i].err3 = VoxelMesher::Min3(_mu_triangles[i].err0, _mu_triangles[i].err1, _mu_triangles[i].err2);
		}
	}
}

void VoxelMesher::UpdateReferences() {
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
void VoxelMesher::CompactMesh() {
	int dst = 0;

	for (int i = 0; i < _mu_vertices.size(); i++) {
		_mu_vertices[i].tcount = 0;
	}

	//int lastSubMeshIndex = -1;
	//subMeshOffsets = new int[subMeshCount];

	for (int i = 0; i < _mu_triangles.size(); i++) {
		MUTriangle triangle = _mu_triangles[i];

		if (!triangle.deleted) {
			if (triangle.va0 != triangle.v0) {
				int iDest = triangle.va0;
				int iSrc = triangle.v0;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;
				if (vertBoneWeights != null) {
					vertBoneWeights[iDest] = vertBoneWeights[iSrc];
				}
				triangle.v0 = triangle.va0;
			}

			if (triangle.va1 != triangle.v1) {
				int iDest = triangle.va1;
				int iSrc = triangle.v1;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;
				if (vertBoneWeights != null) {
					vertBoneWeights[iDest] = vertBoneWeights[iSrc];
				}
				triangle.v1 = triangle.va1;
			}

			if (triangle.va2 != triangle.v2) {
				int iDest = triangle.va2;
				int iSrc = triangle.v2;
				_mu_vertices[iDest].p = _mu_vertices[iSrc].p;
				if (vertBoneWeights != null) {
					vertBoneWeights[iDest] = vertBoneWeights[iSrc];
				}
				triangle.v2 = triangle.va2;
			}

			int newTriangleIndex = dst++;
			_mu_triangles[newTriangleIndex] = triangle;

			_mu_vertices[triangle.v0].tcount = 1;
			_mu_vertices[triangle.v1].tcount = 1;
			_mu_vertices[triangle.v2].tcount = 1;

			//if (triangle.subMeshIndex > lastSubMeshIndex) {
			//	for (int j = lastSubMeshIndex + 1; j < triangle.subMeshIndex; j++) {
			//		subMeshOffsets[j] = newTriangleIndex;
			//	}
			//	subMeshOffsets[triangle.subMeshIndex] = newTriangleIndex;
			//	lastSubMeshIndex = triangle.subMeshIndex;
			//}
		}
	}

	//triangleCount = dst;
	//for (int i = lastSubMeshIndex + 1; i < subMeshCount; i++) {
	//	subMeshOffsets[i] = triangleCount;
	//}

	_mu_triangles.resize(dst);

	dst = 0;
	for (int i = 0; i < vertexCount; i++) {
		var vert = vertices[i];
		if (vert.tcount > 0) {
			vert.tstart = dst;
			vertices[i] = vert;

			if (dst != i) {
				vertices[dst].p = vert.p;
				if (vertNormals != null) vertNormals[dst] = vertNormals[i];
				if (vertTangents != null) vertTangents[dst] = vertTangents[i];
				if (vertUV2D != null) {
					for (int j = 0; j < UVChannelCount; j++) {
						var vertUV = vertUV2D[j];
						if (vertUV != null) {
							vertUV[dst] = vertUV[i];
						}
					}
				}
				if (vertUV3D != null) {
					for (int j = 0; j < UVChannelCount; j++) {
						var vertUV = vertUV3D[j];
						if (vertUV != null) {
							vertUV[dst] = vertUV[i];
						}
					}
				}
				if (vertUV4D != null) {
					for (int j = 0; j < UVChannelCount; j++) {
						var vertUV = vertUV4D[j];
						if (vertUV != null) {
							vertUV[dst] = vertUV[i];
						}
					}
				}
				if (vertColors != null) vertColors[dst] = vertColors[i];
				if (vertBoneWeights != null) vertBoneWeights[dst] = vertBoneWeights[i];

				if (blendShapes != null) {
					for (int shapeIndex = 0; shapeIndex < this.blendShapes.Length; shapeIndex++) {
						blendShapes[shapeIndex].MoveVertexElement(dst, i);
					}
				}
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
	this.vertices.Resize(dst);
	if (vertNormals != null) this.vertNormals.Resize(vertexCount, true);
	if (vertTangents != null) this.vertTangents.Resize(vertexCount, true);
	if (vertUV2D != null) this.vertUV2D.Resize(vertexCount, true);
	if (vertUV3D != null) this.vertUV3D.Resize(vertexCount, true);
	if (vertUV4D != null) this.vertUV4D.Resize(vertexCount, true);
	if (vertColors != null) this.vertColors.Resize(vertexCount, true);
	if (vertBoneWeights != null) this.vertBoneWeights.Resize(vertexCount, true);

	if (blendShapes != null) {
		for (int i = 0; i < this.blendShapes.Length; i++) {
			blendShapes[i].Resize(vertexCount, false);
		}
	}
}

bool VoxelMesher::AreUVsTheSame(int channel, int indexA, int indexB) {
	if (_uv2s.size() > 0) {
		Vector2 vertUV = _uv2s[channel];

		Vector2 uvA = _uv2s[indexA];
		Vector2 uvB = _uv2s[indexB];
		return uvA == uvB;
	}

	return false;
}

/// Remove vertices and mark deleted triangles
int VoxelMesher::RemoveVertexPass(int startTrisCount, int targetTrisCount, double threshold, PoolVector<bool> deleted0, PoolVector<bool> deleted1, int deletedTris) {
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
					var refsArr = refs.Data;
					Array.Copy(refsArr, tstart, refsArr, _mu_vertices[i0].tstart, tcount);
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

double VoxelMesher::VertexError(SymmetricMatrix q, double x, double y, double z) {
	return q.m0 * x * x + 2 * q.m1 * x * y + 2 * q.m2 * x * z + 2 * q.m3 * x + q.m4 * y * y + 2 * q.m5 * y * z + 2 * q.m6 * y + q.m7 * z * z + 2 * q.m8 * z + q.m9;
}

double VoxelMesher::CalculateError(MUVertex vert0, MUVertex vert1, Vector3 *result) {
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

		error = VoxelMesher::Min3(error1, error2, error3);
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

int VoxelMesher::UpdateTriangles(int i0, int ia0, MUVertex *v, PoolVector<bool> deleted, int p_deletedTriangles) {
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
		t.err3 = VoxelMesher::Min3(t.err0, t.err1, t.err2);

		_mu_triangles[tid] = t;
		_mu_refs.push_back(r);
	}

	return deletedTriangles;
}

bool VoxelMesher::Flipped(Vector3 *p, int i0, int i1, MUVertex *v0, PoolVector<bool> &deleted) {
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

Vector3 VoxelMesher::CalculateBarycentricCoords(Vector3 const &point, Vector3 const &a, Vector3 const &b, Vector3 const &c) {
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

void VoxelMesher::InterpolateVertexAttributes(int dst, int i0, int i1, int i2, Vector3 &barycentricCoord) {
	if (vertNormals != null) {
		vertNormals[dst] = Vector3.Normalize((vertNormals[i0] * barycentricCoord.x) + (vertNormals[i1] * barycentricCoord.y) + (vertNormals[i2] * barycentricCoord.z));
	}

	if (vertTangents != null) {
		vertTangents[dst] = NormalizeTangent((vertTangents[i0] * barycentricCoord.x) + (vertTangents[i1] * barycentricCoord.y) + (vertTangents[i2] * barycentricCoord.z));
	}

	if (vertUV2D != null) {
		for (int i = 0; i < UVChannelCount; i++) {
			var vertUV = vertUV2D[i];
			if (vertUV != null) {
				vertUV[dst] = (vertUV[i0] * barycentricCoord.x) + (vertUV[i1] * barycentricCoord.y) + (vertUV[i2] * barycentricCoord.z);
			}
		}
	}

	if (vertColors != null) {
		vertColors[dst] = (vertColors[i0] * barycentricCoord.x) + (vertColors[i1] * barycentricCoord.y) + (vertColors[i2] * barycentricCoord.z);
	}

	if (blendShapes != null) {
		for (int i = 0; i < blendShapes.Length; i++) {
			blendShapes[i].InterpolateVertexAttributes(dst, i0, i1, i2, barycentricCoord);
		}
	}

	// TODO: How do we interpolate the bone weights? Do we have to?
}

void VoxelMesher::reset() {
	_vertices.resize(0);
	_normals.resize(0);
	_colors.resize(0);
	_uvs.resize(0);
	_uv2s.resize(0);
	_indices.resize(0);
	_bones.resize(0);

	_surface_tool->clear();
}

void VoxelMesher::add_chunk_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	add_chunk(vchunk);
}
void VoxelMesher::add_chunk(VoxelChunk *chunk) {
	ERR_FAIL_COND(!has_method("_add_chunk"));
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	call("_add_chunk", chunk);
}

void VoxelMesher::add_chunk_liquid_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	add_chunk_liquid(vchunk);
}
void VoxelMesher::add_chunk_liquid(VoxelChunk *chunk) {
	ERR_FAIL_COND(!has_method("_add_chunk_liquid"));
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	call("_add_chunk_liquid", chunk);
}

void VoxelMesher::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale, const Rect2 uv_rect) {
	Transform transform = Transform(Basis(rotation).scaled(scale), position);

	add_mesh_data_resource_transform(mesh, transform, uv_rect);
}

void VoxelMesher::add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect) {
	ERR_FAIL_COND(mesh->get_array().size() == 0);

	Array verts = mesh->get_array().get(Mesh::ARRAY_VERTEX);

	for (int i = 0; i < verts.size(); ++i) {
		Vector3 vert = verts[i];

		vert = transform.xform(vert);

		add_vertex(vert);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_NORMAL)
		return;

	Array normals = mesh->get_array().get(Mesh::ARRAY_NORMAL);

	for (int i = 0; i < normals.size(); ++i) {
		Vector3 normal = normals[i];

		normal = transform.basis.xform(normal);

		add_normal(normal);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TANGENT)
		return;

	Array tangents = mesh->get_array().get(Mesh::ARRAY_TANGENT);

	for (int i = 0; i < verts.size(); ++i) {

		Plane p(tangents[i * 4 + 0], tangents[i * 4 + 1], tangents[i * 4 + 2], tangents[i * 4 + 3]);

		Vector3 tangent = p.normal;
		Vector3 binormal = p.normal.cross(tangent).normalized() * p.d;

		tangent = local_transform.basis.xform(tangent);
		binormal = local_transform.basis.xform(binormal);

		add_t(normal);
		add_binorm
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_COLOR)
		return;

	Array colors = mesh->get_array().get(Mesh::ARRAY_COLOR);

	for (int i = 0; i < colors.size(); ++i) {
		Color color = colors[i];

		add_color(color);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV)
		return;

	Array tex_uv = mesh->get_array().get(Mesh::ARRAY_TEX_UV);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv[i];

		uv.x *= uv_rect.size.x;
		uv.y *= uv_rect.size.y;

		uv.x += uv_rect.position.x;
		uv.y += uv_rect.position.y;

		add_uv(uv);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV2)
		return;

	Array tex_uv2 = mesh->get_array().get(Mesh::ARRAY_TEX_UV2);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv2[i];

		add_uv2(uv);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_BONES)
		return;

	Array bones = mesh->get_array().get(Mesh::ARRAY_BONES);

	for (int i = 0; i < bones.size(); ++i) {
		int bone = bones[i];

		add_bone(bone);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_WEIGHTS)
		return;

	Array weights = mesh->get_array().get(Mesh::ARRAY_WEIGHTS);

	for (int i = 0; i < weights.size(); ++i) {
		float weight = weights[i];

		add_weight(weight);
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_INDEX)
		return;

	Array indices = mesh->get_array().get(Mesh::ARRAY_INDEX);
	int ic = get_vertex_count() - verts.size();

	for (int i = 0; i < indices.size(); ++i) {
		int index = indices[i];

		add_indices(ic + index);
	}
}

void VoxelMesher::bake_colors_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	bake_colors(vchunk);
}
void VoxelMesher::bake_colors(VoxelChunk *chunk) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_bake_colors"))
		call("_bake_colors", chunk);
}
void VoxelMesher::_bake_colors(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	ERR_FAIL_COND(_vertices.size() != _normals.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vector3 vert = _vertices[i];

		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}

			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_channel_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			if (_colors.size() < _vertices.size()) {
				_colors.push_back(light);
			} else {
				_colors.set(i, light);
			}
		} else {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}
		}
	}
}

void VoxelMesher::bake_liquid_colors_bind(Node *chunk) {
	VoxelChunk *vchunk = Object::cast_to<VoxelChunk>(chunk);

	bake_liquid_colors(vchunk);
}
void VoxelMesher::bake_liquid_colors(VoxelChunk *chunk) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	if (has_method("_bake_liquid_colors"))
		call("_bake_liquid_colors", chunk);
}
void VoxelMesher::_bake_liquid_colors(Node *p_chunk) {
	VoxelChunk *chunk = Object::cast_to<VoxelChunk>(p_chunk);

	ERR_FAIL_COND(!ObjectDB::instance_validate(chunk));

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	ERR_FAIL_COND(_vertices.size() != _normals.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vector3 vert = _vertices[i];

		if (vert.x < 0 || vert.y < 0 || vert.z < 0) {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}

			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _voxel_scale);
		unsigned int y = (unsigned int)(vert.y / _voxel_scale);
		unsigned int z = (unsigned int)(vert.z / _voxel_scale);

		if (chunk->validate_channel_data_position(x, y, z)) {
			Color light = Color(
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_AO) / 255.0) * _ao_strength;
			float rao = chunk->get_voxel(x, y, z, VoxelChunk::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;
			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			if (_colors.size() < _vertices.size()) {
				_colors.push_back(light);
			} else {
				_colors.set(i, light);
			}
		} else {
			if (_colors.size() < _vertices.size()) {
				_colors.push_back(base_light);
			}
		}
	}
}

void VoxelMesher::build_collider(RID shape) const {
	ERR_FAIL_COND(shape == RID());

	if (_vertices.size() == 0)
		return;

	PoolVector<Vector3> face_points;

	if (_indices.size() == 0) {

		int len = (_vertices.size() / 4);

		for (int i = 0; i < len; ++i) {

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 2));
			face_points.push_back(_vertices.get((i * 4) + 1));

			face_points.push_back(_vertices.get(i * 4));
			face_points.push_back(_vertices.get((i * 4) + 3));
			face_points.push_back(_vertices.get((i * 4) + 2));
		}

		PhysicsServer::get_singleton()->shape_set_data(shape, face_points);

		return;
	}

	face_points.resize(_indices.size());
	for (int i = 0; i < face_points.size(); i++) {
		face_points.set(i, _vertices.get(_indices.get(i)));
	}

	PhysicsServer::get_singleton()->shape_set_data(shape, face_points);
}

void VoxelMesher::bake_lights(MeshInstance *node, Vector<Ref<VoxelLight> > &lights) {
	ERR_FAIL_COND(node == NULL);

	Color darkColor(0, 0, 0, 1);

	for (int v = 0; v < _vertices.size(); ++v) {

		Vector3 vet = _vertices.get(v);
		Vector3 vertex = node->to_global(vet);

		//grab normal
		Vector3 normal = _normals.get(v);

		Vector3 v_lightDiffuse;

		//calculate the lights value
		for (int i = 0; i < lights.size(); ++i) {
			Ref<VoxelLight> light = lights.get(i);

			Vector3 lightDir = light->get_world_position() - vertex;

			float dist2 = lightDir.dot(lightDir);
			//inverse sqrt
			lightDir *= (1.0 / sqrt(dist2));

			float NdotL = normal.dot(lightDir);

			if (NdotL > 1.0) {
				NdotL = 1.0;
			} else if (NdotL < 0.0) {
				NdotL = 0.0;
			}

			Color cc = light->get_color();
			Vector3 cv(cc.r, cc.g, cc.b);

			Vector3 value = cv * (NdotL / (1.0 + dist2));

			value *= light->get_size();
			v_lightDiffuse += value;

			/*
                    float dist2 = Mathf.Clamp(Vector3.Distance(transformedLights[i], vertices), 0f, 15f);
                    dist2 /= 35f;

                    Vector3 value = Vector3.one;
                    value *= ((float) lights[i].Strength) / 255f;
                    value *= (1 - dist2);
                    v_lightDiffuse += value;*/
		}

		Color f = _colors.get(v);
		//Color f = darkColor;

		Vector3 cv2(f.r, f.g, f.b);
		cv2 += v_lightDiffuse;

		if (cv2.x > 1)
			cv2.x = 1;

		if (cv2.y > 1)
			cv2.y = 1;

		if (cv2.y > 1)
			cv2.y = 1;

		// cv2.x = Mathf.Clamp(cv2.x, 0f, 1f);
		//cv2.y = Mathf.Clamp(cv2.y, 0f, 1f);
		// cv2.z = Mathf.Clamp(cv2.z, 0f, 1f);

		f.r = cv2.x;
		f.g = cv2.y;
		f.b = cv2.z;

		//f.r = v_lightDiffuse.x;
		//f.g = v_lightDiffuse.y;
		//f.b = v_lightDiffuse.z;

		_colors.set(v, f);
	}

	//	for (int i = 0; i < _colors->size(); ++i) {
	//		print_error(_colors->get(i));
	//	}
}

PoolVector<Vector3> VoxelMesher::get_vertices() {
	return _vertices;
}

void VoxelMesher::set_vertices(PoolVector<Vector3> values) {
	_vertices = values;
}

int VoxelMesher::get_vertex_count() {
	return _vertices.size();
}

void VoxelMesher::add_vertex(Vector3 vertex) {
	_vertices.push_back(vertex);
}

Vector3 VoxelMesher::get_vertex(int idx) {
	return _vertices.get(idx);
}

void VoxelMesher::remove_vertex(int idx) {
	_vertices.remove(idx);
}

PoolVector<Vector3> VoxelMesher::get_normals() {
	return _normals;
}

void VoxelMesher::set_normals(PoolVector<Vector3> values) {
	_normals = values;
}

int VoxelMesher::get_normal_count() {
	return _normals.size();
}

void VoxelMesher::add_normal(Vector3 normal) {
	_normals.push_back(normal);
}

Vector3 VoxelMesher::get_normal(int idx) {
	return _normals.get(idx);
}

void VoxelMesher::remove_normal(int idx) {
	_normals.remove(idx);
}

PoolVector<Color> VoxelMesher::get_colors() {
	return _colors;
}

void VoxelMesher::set_colors(PoolVector<Color> values) {
	_colors = values;
}

int VoxelMesher::get_color_count() {
	return _colors.size();
}

void VoxelMesher::add_color(Color color) {
	_colors.push_back(color);
}

Color VoxelMesher::get_color(int idx) {
	return _colors.get(idx);
}

void VoxelMesher::remove_color(int idx) {
	_colors.remove(idx);
}

PoolVector<Vector2> VoxelMesher::get_uvs() {
	return _uvs;
}

void VoxelMesher::set_uvs(PoolVector<Vector2> values) {
	_uvs = values;
}

int VoxelMesher::get_uv_count() {
	return _uvs.size();
}

void VoxelMesher::add_uv(Vector2 uv) {
	_uvs.push_back(uv);
}

Vector2 VoxelMesher::get_uv(int idx) {
	return _uvs.get(idx);
}

void VoxelMesher::remove_uv(int idx) {
	_uvs.remove(idx);
}

PoolVector<Vector2> VoxelMesher::get_uv2s() {
	return _uv2s;
}

void VoxelMesher::set_uv2s(PoolVector<Vector2> values) {
	_uv2s = values;
}

int VoxelMesher::get_uv2_count() {
	return _uv2s.size();
}

void VoxelMesher::add_uv2(Vector2 uv) {
	_uv2s.push_back(uv);
}

Vector2 VoxelMesher::get_uv2(int idx) {
	return _uv2s.get(idx);
}

void VoxelMesher::remove_uv2(int idx) {
	_uv2s.remove(idx);
}

PoolVector<int> VoxelMesher::get_indices() {
	return _indices;
}

void VoxelMesher::set_indices(PoolVector<int> values) {
	_indices = values;
}

int VoxelMesher::get_indices_count() {
	return _indices.size();
}

void VoxelMesher::add_indices(int index) {
	_indices.push_back(index);
}

int VoxelMesher::get_indice(int idx) {
	return _indices.get(idx);
}

void VoxelMesher::remove_indices(int idx) {
	_indices.remove(idx);
}

VoxelMesher::VoxelMesher(Ref<VoxelmanLibrary> library) {
	_library = library;

	_voxel_scale = 1;
	_lod_size = 1;

	_surface_tool.instance();
}

VoxelMesher::VoxelMesher() {

	_voxel_scale = 1;
	_lod_size = 1;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);

	_surface_tool.instance();

	maxIterationCount = 100;
	agressiveness = 7.0;
	enableSmartLink = true;
	preserveBorderEdges = false;
	preserveUVSeamEdges = false;
	preserveUVFoldoverEdges = false;
}

VoxelMesher::~VoxelMesher() {
	_surface_tool.unref();

	if (_library.is_valid()) {
		_library.unref();
	}
}

void VoxelMesher::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_add_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_add_chunk_liquid", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_bake_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_bake_liquid_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("get_library"), &VoxelMesher::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &VoxelMesher::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "VoxelmanLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_material"), &VoxelMesher::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &VoxelMesher::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &VoxelMesher::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &VoxelMesher::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &VoxelMesher::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &VoxelMesher::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &VoxelMesher::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &VoxelMesher::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &VoxelMesher::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &VoxelMesher::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &VoxelMesher::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &VoxelMesher::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk"), &VoxelMesher::add_chunk_bind);
	ClassDB::bind_method(D_METHOD("add_chunk_liquid", "chunk"), &VoxelMesher::add_chunk_liquid_bind);

	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "position", "rotation", "scale", "uv_rect"), &VoxelMesher::add_mesh_data_resource, DEFVAL(Rect2(0, 0, 1, 1)), DEFVAL(Vector3(1.0, 1.0, 1.0)), DEFVAL(Vector3()), DEFVAL(Vector3()));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform", "mesh", "transform", "uv_rect"), &VoxelMesher::add_mesh_data_resource_transform, DEFVAL(Rect2(0, 0, 1, 1)));

	ClassDB::bind_method(D_METHOD("bake_colors", "chunk"), &VoxelMesher::bake_colors_bind);
	ClassDB::bind_method(D_METHOD("_bake_colors", "chunk"), &VoxelMesher::_bake_colors);

	ClassDB::bind_method(D_METHOD("bake_liquid_colors", "chunk"), &VoxelMesher::bake_liquid_colors_bind);
	ClassDB::bind_method(D_METHOD("_bake_liquid_colors", "chunk"), &VoxelMesher::_bake_liquid_colors);

	ClassDB::bind_method(D_METHOD("get_vertices"), &VoxelMesher::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &VoxelMesher::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &VoxelMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &VoxelMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &VoxelMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &VoxelMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_normals"), &VoxelMesher::get_normals);
	ClassDB::bind_method(D_METHOD("set_normals", "values"), &VoxelMesher::set_normals);
	ClassDB::bind_method(D_METHOD("get_normal_count"), &VoxelMesher::get_normal_count);
	ClassDB::bind_method(D_METHOD("get_normal", "idx"), &VoxelMesher::get_normal);
	ClassDB::bind_method(D_METHOD("remove_normal", "idx"), &VoxelMesher::remove_normal);
	ClassDB::bind_method(D_METHOD("add_normal", "normal"), &VoxelMesher::add_normal);

	ClassDB::bind_method(D_METHOD("get_colors"), &VoxelMesher::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &VoxelMesher::set_colors);
	ClassDB::bind_method(D_METHOD("get_color_count"), &VoxelMesher::get_color_count);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &VoxelMesher::get_color);
	ClassDB::bind_method(D_METHOD("remove_color", "idx"), &VoxelMesher::remove_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &VoxelMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &VoxelMesher::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &VoxelMesher::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv_count"), &VoxelMesher::get_uv_count);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &VoxelMesher::get_uv);
	ClassDB::bind_method(D_METHOD("remove_uv", "idx"), &VoxelMesher::remove_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &VoxelMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_uv2s"), &VoxelMesher::get_uv2s);
	ClassDB::bind_method(D_METHOD("set_uv2s", "values"), &VoxelMesher::set_uv2s);
	ClassDB::bind_method(D_METHOD("get_uv2_count"), &VoxelMesher::get_uv2_count);
	ClassDB::bind_method(D_METHOD("get_uv2", "idx"), &VoxelMesher::get_uv2);
	ClassDB::bind_method(D_METHOD("remove_uv2", "idx"), &VoxelMesher::remove_uv2);
	ClassDB::bind_method(D_METHOD("add_uv2", "uv"), &VoxelMesher::add_uv2);

	ClassDB::bind_method(D_METHOD("get_indices"), &VoxelMesher::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &VoxelMesher::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &VoxelMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_indice", "idx"), &VoxelMesher::get_indice);
	ClassDB::bind_method(D_METHOD("remove_indices", "idx"), &VoxelMesher::remove_indices);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &VoxelMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &VoxelMesher::reset);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &VoxelMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh", "mesh_rid"), &VoxelMesher::build_mesh);
}
