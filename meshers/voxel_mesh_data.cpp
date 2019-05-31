#include "voxel_mesh_data.h"

Ref<Material> VoxelMeshData::get_material() {
	return _material;
}

void VoxelMeshData::set_material(Ref<Material> material) {
	_material = material;
}

Ref<ArrayMesh> VoxelMeshData::create_mesh() {
	_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
	_surface_tool->set_material(_material);

	int len = _vertices->size();

	for (int i = 0; i < len; ++i) {
		_surface_tool->add_normal(_normals->get(i));
		//_surface_tool->add_color(_colors->get(i));
		//_surface_tool->add_uv(_uvs->get(i));

		_surface_tool->add_vertex(_vertices->get(i));
	}

	for (int i = 0; i < _indices->size(); ++i) {
		_surface_tool->add_index(_indices->get(i));
	}

	Ref<ArrayMesh> m = _surface_tool->commit();

	return m;
}

void VoxelMeshData::add_vertices_from(Vector<Vector3> &vertices) {
	_vertices->append_array(vertices);
}

void VoxelMeshData::add_normals_from(Vector<Vector3> &normals) {
	_normals->append_array(normals);
}

void VoxelMeshData::add_indices_from(Vector<int> &indices) {
	_indices->append_array(indices);
}

void VoxelMeshData::add_vertex(Vector3 vertex) {
	_vertices->push_back(vertex);
}

void VoxelMeshData::add_normal(Vector3 normal) {
	_normals->push_back(normal);
}

void VoxelMeshData::add_index(int iundex) {
	_indices->push_back(iundex);
}

void VoxelMeshData::clear() {
	_vertices->clear();
	_normals->clear();
	_indices->clear();
}

VoxelMeshData::VoxelMeshData() {
	_vertices = memnew(Vector<Vector3>());
	_normals = memnew(Vector<Vector3>());
	_indices = memnew(Vector<int>());

	_surface_tool.instance();
}

VoxelMeshData::~VoxelMeshData() {
	memdelete(_vertices);
	memdelete(_normals);
	memdelete(_indices);

	_surface_tool.unref();
}

void VoxelMeshData::_bind_methods() {
}
