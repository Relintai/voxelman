#ifndef VOXEL_MESH_DATA_H
#define VOXEL_MESH_DATA_H

#include "core/reference.h"
#include "core/vector.h"
#include "scene/resources/mesh.h"
#include "scene/resources/surface_tool.h"
#include "scene/resources/material.h"

class VoxelMeshData : public Reference {
	GDCLASS(VoxelMeshData, Reference)

public:
	Ref<Material> get_material();
	void set_material(Ref<Material> material);

	Ref<ArrayMesh> create_mesh();

	void add_vertex(Vector3 vertex);
	void add_normal(Vector3 normal);
	void add_index(int iundex);

	void add_vertices_from(Vector<Vector3> &vertices);
	void add_normals_from(Vector<Vector3> &normals);
	void add_indices_from(Vector<int> &indices);

	void clear();

	VoxelMeshData();
	~VoxelMeshData();

protected:
	static void _bind_methods();

private:
	Ref<Material> _material;

	Vector<Vector3> *_vertices;
	Vector<Vector3> *_normals;
	Vector<int> *_indices;

	Ref<SurfaceTool> _surface_tool;
};

#endif
