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

Ref<PropDataMesh> PropToolMesh::get_data() {
	return _prop_mesh;
}
void PropToolMesh::set_data(const Ref<PropDataMesh> &data) {
	_prop_mesh = data;
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
	bool _snap_to_mesh;
}
PropToolMesh::~PropToolMesh() {
	_prop_mesh.unref();
	_material.unref();
}

void PropToolMesh::set_generate(bool value) {
}

void PropToolMesh::_bind_methods() {
}

/*
tool
extends MeshInstance
class_name PropToolMesh

export(MeshDataResource) var mesh_data : MeshDataResource setget set_prop_mesh
export(Texture) var texture : Texture setget set_texture
export(bool) var snap_to_mesh : bool = false
export(Vector3) var snap_axis : Vector3 = Vector3(0, -1, 0)

export(bool) var generate : bool setget set_generate

var _prop_mesh : PropDataMesh
var _material : SpatialMaterial

func get_data() -> PropDataMesh:
	if not visible or mesh_data == null:
		return null
	
	if _prop_mesh == null:
		_prop_mesh = PropDataMesh.new()
		
	_prop_mesh.mesh = mesh_data
	_prop_mesh.texture = texture
	_prop_mesh.snap_to_mesh = snap_to_mesh
	_prop_mesh.snap_axis = snap_axis

	return _prop_mesh

func set_data(data : PropDataMesh) -> void:
	_prop_mesh = data
	
	set_texture(_prop_mesh.texture)
	set_prop_mesh(_prop_mesh.mesh)
	
	snap_to_mesh = _prop_mesh.snap_to_mesh
	snap_axis = _prop_mesh.snap_axis

func set_prop_mesh(md : MeshDataResource) -> void:
	mesh_data = md
	
	set_generate(true)
	
func set_texture(tex : Texture) -> void:
	texture = tex
	
func set_generate(val : bool) -> void:
	if val:
		if !mesh_data:
			mesh = null
			return
			
		var m : ArrayMesh = ArrayMesh.new()
		
		var arr = []
		arr.resize(ArrayMesh.ARRAY_MAX)
		
		var v : PoolVector3Array = PoolVector3Array()
		v.append_array(mesh_data.array[Mesh.ARRAY_VERTEX])
		arr[Mesh.ARRAY_VERTEX] = v
		
		var norm : PoolVector3Array = PoolVector3Array()
		norm.append_array(mesh_data.array[Mesh.ARRAY_NORMAL])
		arr[Mesh.ARRAY_NORMAL] = norm
		
		var uv : PoolVector2Array = PoolVector2Array()
		uv.append_array(mesh_data.array[Mesh.ARRAY_TEX_UV])
		arr[Mesh.ARRAY_TEX_UV] = uv

		var ind : PoolIntArray = PoolIntArray()
		ind.append_array(mesh_data.array[Mesh.ARRAY_INDEX])
		arr[Mesh.ARRAY_INDEX] = ind
		
		m.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arr)

		mesh = m
		
		if texture != null:
			if _material == null:
				_material = SpatialMaterial.new()
			
			_material.albedo_texture = texture
			
			material_override = _material
		



*/