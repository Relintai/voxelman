/*
Copyright (c) 2020 Péter Magyar

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

#include "prop_tool_mesh.h"

#ifdef TOOLS_ENABLED

Ref<MeshDataResource> PropToolMesh::get_mesh_data() {
	return _mesh_data;
}
void PropToolMesh::set_mesh_data(const Ref<MeshDataResource> &data) {
	_mesh_data = data;
}

Ref<PropDataMesh> PropToolMesh::get_data() {
	return _prop_mesh;

	if (!is_visible() || !_mesh_data.is_valid())
		return Ref<PropDataMesh>();

	if (!_prop_mesh.is_valid())
		_prop_mesh.instance();

	_prop_mesh->set_mesh(_mesh_data);
	_prop_mesh->set_texture(_texture);
	_prop_mesh->set_snap_to_mesh(_snap_to_mesh);
	_prop_mesh->set_snap_axis(_snap_axis);

	return _prop_mesh;
}
void PropToolMesh::set_data(const Ref<PropDataMesh> &data) {
	_prop_mesh = data;

	if (!_prop_mesh.is_valid())
		return;

	_texture = _prop_mesh->get_texture();

	_snap_to_mesh = _prop_mesh->get_snap_to_mesh();
	_snap_axis = _prop_mesh->get_snap_axis();

	set_generate(true);
}

Ref<Texture> PropToolMesh::get_texture() {
	return _texture;
}
void PropToolMesh::set_texture(const Ref<Texture> &tex) {
	_texture = tex;
}

bool PropToolMesh::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void PropToolMesh::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

Vector3 PropToolMesh::get_snap_axis() const {
	return _snap_axis;
}
void PropToolMesh::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

Ref<MeshDataResource> PropToolMesh::get_prop_mesh() {
	return _prop_mesh;
}
void PropToolMesh::set_prop_mesh(const Ref<MeshDataResource> &data) {
	_prop_mesh = data;
}

void PropToolMesh::generate() {
}

PropToolMesh::PropToolMesh() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropToolMesh::~PropToolMesh() {
	_prop_mesh.unref();
	_material.unref();
}

void PropToolMesh::set_generate(bool value) {
	if (!value)
		return;

	if (!_mesh_data.is_valid()) {
		mesh.unref();
		return;
	}

	Ref<ArrayMesh> m;
	m.instance();

	Array arr;
	arr.resize(ArrayMesh::ARRAY_MAX);

	Array mda = _mesh_data->get_array();

	PoolVector3Array v;
	v.append_array(mda[Mesh::ARRAY_VERTEX]);
	arr.set(Mesh::ARRAY_VERTEX, v);

	PoolVector3Array norm;
	norm.append_array(mda[Mesh::ARRAY_NORMAL]);
	arr.set(Mesh::ARRAY_NORMAL, norm);

	PoolVector2Array uv;
	uv.append_array(mda[Mesh::ARRAY_TEX_UV]);
	arr.set(Mesh::ARRAY_TEX_UV, uv);

	PoolIntArray ind;
	ind.append_array(mda[Mesh::ARRAY_INDEX]);
	arr.set(Mesh::ARRAY_INDEX, ind);

	m->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arr);

	mesh = m;

	if (_texture.is_valid()) {
		if (!_material.is_valid())
			_material.instance();

		_material->set_texture(SpatialMaterial::TEXTURE_ALBEDO, _texture);

		set_material_override(_material);
	}
}

void PropToolMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh_data"), &PropToolMesh::get_mesh_data);
	ClassDB::bind_method(D_METHOD("set_mesh_data", "value"), &PropToolMesh::set_mesh_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh_data", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh_data", "get_mesh_data");

	ClassDB::bind_method(D_METHOD("get_data"), &PropToolMesh::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropToolMesh::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "PropDataMesh"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_texture"), &PropToolMesh::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &PropToolMesh::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropToolMesh::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropToolMesh::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropToolMesh::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropToolMesh::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_prop_mesh"), &PropToolMesh::get_prop_mesh);
	ClassDB::bind_method(D_METHOD("set_prop_mesh", "value"), &PropToolMesh::set_prop_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop_mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_prop_mesh", "get_prop_mesh");

	ClassDB::bind_method(D_METHOD("generate"), &PropToolMesh::generate);

	ClassDB::bind_method(D_METHOD("get_generate"), &PropToolMesh::get_generate);
	ClassDB::bind_method(D_METHOD("set_generate"), &PropToolMesh::set_generate);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "generate"), "set_generate", "get_generate");
}

#endif