#include "voxelman_prop_mesh.h"

Ref<MeshDataResource> VoxelmanPropMesh::get_mesh() const {
	return _mesh;
}
void VoxelmanPropMesh::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

bool VoxelmanPropMesh::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void VoxelmanPropMesh::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 VoxelmanPropMesh::get_snap_axis() {
	return _snap_axis;
}
void VoxelmanPropMesh::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

VoxelmanPropMesh::VoxelmanPropMesh() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
VoxelmanPropMesh::~VoxelmanPropMesh() {
	if (_mesh.is_valid())
		_mesh.unref();
}

void VoxelmanPropMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelmanPropMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelmanPropMesh::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelmanPropMesh::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelmanPropMesh::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelmanPropMesh::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelmanPropMesh::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
