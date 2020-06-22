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

#include "prop_data_mesh.h"

Ref<MeshDataResource> PropDataMesh::get_mesh() const {
	return _mesh;
}
void PropDataMesh::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

Ref<Texture> PropDataMesh::get_texture() const {
	return _texture;
}
void PropDataMesh::set_texture(const Ref<Texture> texture) {
	_texture = texture;
}

bool PropDataMesh::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataMesh::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataMesh::get_snap_axis() {
	return _snap_axis;
}
void PropDataMesh::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

PropDataMesh::PropDataMesh() {
	_snap_to_mesh = true;
	_snap_axis = Vector3(0, 1, 0);
}
PropDataMesh::~PropDataMesh() {
	if (_mesh.is_valid())
		_mesh.unref();
}

void PropDataMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &PropDataMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &PropDataMesh::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_texture"), &PropDataMesh::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &PropDataMesh::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataMesh::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataMesh::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataMesh::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataMesh::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");
}
