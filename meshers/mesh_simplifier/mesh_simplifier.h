#ifndef MESH_SIMPLIFIER_H
#define MESH_SIMPLIFIER_H

#include "mesh_utils.h"

#include "core/pool_vector.h"
#include "core/resource.h"

class VoxelMesher;

class MeshSimplifier {

public:
	void initialize(Ref<VoxelMesher> mesher);
	void refresh_vertices();
	void SimplifyMesh(float quality);
	void SimplifyMeshLossless();
	void UpdateMesh(int iteration);
	void UpdateReferences();
	int RemoveVertexPass(int startTrisCount, int targetTrisCount, double threshold, PoolVector<bool> deleted0, PoolVector<bool> deleted1, int deletedTris);
	void CompactMesh();
	bool AreUVsTheSame(int channel, int indexA, int indexB);
	double VertexError(SymmetricMatrix q, double x, double y, double z);
	double CalculateError(MUVertex vert0, MUVertex vert1, Vector3 *result);
	int UpdateTriangles(int i0, int ia0, MUVertex *v, PoolVector<bool> deleted, int deletedTriangles);
	bool Flipped(Vector3 *p, int i0, int i1, MUVertex *v0, PoolVector<bool> &deleted);
	static Vector3 CalculateBarycentricCoords(Vector3 const &point, Vector3 const &a, Vector3 const &b, Vector3 const &c);
	void InterpolateVertexAttributes(int dst, int i0, int i1, int i2, Vector3 &barycentricCoord);

	static double Min3(double val1, double val2, double val3) {
		return (val1 < val2 ? (val1 < val3 ? val1 : val3) : (val2 < val3 ? val2 : val3));
	}

	MeshSimplifier();

private:
	PoolVector<Vector3> _vertices;
	PoolVector<Vector3> _normals;
	PoolVector<Color> _colors;
	PoolVector<Vector2> _uvs;
	PoolVector<Vector2> _uv2s;
	PoolVector<int> _indices;

	PoolVector<MUTriangle> _mu_triangles;
	PoolVector<MUVertex> _mu_vertices;
	PoolVector<MURef> _mu_refs;

	Ref<VoxelMesher> _mesher;

	double vertexLinkDistanceSqr = std::numeric_limits<double>::epsilon();
	int maxIterationCount;
	double agressiveness;
	bool enableSmartLink;
	bool preserveBorderEdges;
	bool preserveUVSeamEdges;
	bool preserveUVFoldoverEdges;
};

#endif