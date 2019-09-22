#include "voxelman_prop_mesh.h"

Ref<MeshDataResource> VoxelmanPropMesh::get_mesh() const {
	return _mesh;
}
void VoxelmanPropMesh::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

VoxelmanPropMesh::VoxelmanPropMesh() {
}
VoxelmanPropMesh::~VoxelmanPropMesh() {
	if (_mesh.is_valid())
		_mesh.unref();
}

void VoxelmanPropMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelmanPropMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelmanPropMesh::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");
}
