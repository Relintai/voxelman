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

#include "voxel_chunk_prop_data.h"

#include "voxel_chunk.h"

int VoxelChunkPropData::get_x() const {
	return _x;
}
void VoxelChunkPropData::set_x(const int value) {
	_x = value;
}

int VoxelChunkPropData::get_y() const {
	return _y;
}
void VoxelChunkPropData::set_y(const int value) {
	_y = value;
}

int VoxelChunkPropData::get_z() const {
	return _z;
}
void VoxelChunkPropData::set_z(const int value) {
	_z = value;
}

Vector3 VoxelChunkPropData::get_rotation() const {
	return _rotation;
}
void VoxelChunkPropData::set_rotation(const Vector3 &value) {
	_rotation = value;
}

Vector3 VoxelChunkPropData::get_scale() const {
	return _scale;
}
void VoxelChunkPropData::set_scale(const Vector3 &value) {
	_scale = value;
}

bool VoxelChunkPropData::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void VoxelChunkPropData::set_snap_to_mesh(const bool &value) {
	_snap_to_mesh = value;
}

Vector3 VoxelChunkPropData::get_snap_axis() const {
	return _snap_axis;
}
void VoxelChunkPropData::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

Ref<VoxelChunk> VoxelChunkPropData::get_owner() {
	return _owner;
}
void VoxelChunkPropData::set_owner(const Ref<VoxelChunk> &chunk) {
	_owner = chunk;
}

int VoxelChunkPropData::get_scene_id() const {
	return _scene_id;
}
void VoxelChunkPropData::set_scene_id(const int id) {
	_scene_id = id;
}

Transform VoxelChunkPropData::get_transform() const {
	return _transform;
}
void VoxelChunkPropData::set_transform(const Transform &value) {
	_transform = value;
}

Ref<PackedScene> VoxelChunkPropData::get_scene() {
	return _scene;
}
void VoxelChunkPropData::set_scene(const Ref<PackedScene> &value) {
	_scene = value;
}

Ref<MeshDataResource> VoxelChunkPropData::get_mesh() {
	return _mesh;
}
void VoxelChunkPropData::set_mesh(const Ref<MeshDataResource> &value) {
	_mesh = value;
}

Ref<Texture> VoxelChunkPropData::get_mesh_texture() {
	return _texture;
}
void VoxelChunkPropData::set_mesh_texture(const Ref<Texture> &value) {
	_texture = value;
}

Ref<PropDataLight> VoxelChunkPropData::get_light() {
	return _light;
}
void VoxelChunkPropData::set_light(const Ref<PropDataLight> &value) {
	_light = value;
}

Ref<PropData> VoxelChunkPropData::get_prop() {
	return _prop;
}
void VoxelChunkPropData::set_prop(const Ref<PropData> &value) {
	_prop = value;
}

Node *VoxelChunkPropData::get_spawned_prop() const {
	return _spawned_prop;
}
void VoxelChunkPropData::set_spawned_prop(Node *value) {
	_spawned_prop = value;
}
Node *VoxelChunkPropData::spawn_prop(Node *parent) {
	return NULL;
}

void VoxelChunkPropData::free_prop() {
}

void VoxelChunkPropData::set_translation_for_chunk(const Ref<VoxelChunk> &chunk, const int local_x, const int local_y, const int local_z) {
}

VoxelChunkPropData::VoxelChunkPropData() {
	_x = 0;
	_y = 0;
	_z = 0;
	_scale = Vector3(1, 1, 1);
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);

	_spawned_prop = NULL;
	_scene_id = 0;
}
VoxelChunkPropData::~VoxelChunkPropData() {
	_mesh.unref();
	_texture.unref();
	_light.unref();
	_prop.unref();
	_scene.unref();

	_owner.unref();
}

void VoxelChunkPropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_x"), &VoxelChunkPropData::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "value"), &VoxelChunkPropData::set_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");

	ClassDB::bind_method(D_METHOD("get_y"), &VoxelChunkPropData::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "value"), &VoxelChunkPropData::set_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");

	ClassDB::bind_method(D_METHOD("get_z"), &VoxelChunkPropData::get_z);
	ClassDB::bind_method(D_METHOD("set_z", "value"), &VoxelChunkPropData::set_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "z"), "set_z", "get_z");

	ClassDB::bind_method(D_METHOD("get_rotation"), &VoxelChunkPropData::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &VoxelChunkPropData::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("get_scale"), &VoxelChunkPropData::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &VoxelChunkPropData::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &VoxelChunkPropData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &VoxelChunkPropData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &VoxelChunkPropData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &VoxelChunkPropData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_owner"), &VoxelChunkPropData::get_owner);
	ClassDB::bind_method(D_METHOD("set_owner", "value"), &VoxelChunkPropData::set_owner);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "owner", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), "set_owner", "get_owner");

	ClassDB::bind_method(D_METHOD("get_scene_id"), &VoxelChunkPropData::get_scene_id);
	ClassDB::bind_method(D_METHOD("set_scene_id", "value"), &VoxelChunkPropData::set_scene_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "scene_id"), "set_scene_id", "get_scene_id");

	ClassDB::bind_method(D_METHOD("get_transform"), &VoxelChunkPropData::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &VoxelChunkPropData::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_spawned_prop"), &VoxelChunkPropData::get_spawned_prop);
	ClassDB::bind_method(D_METHOD("set_spawned_prop", "value"), &VoxelChunkPropData::set_spawned_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "spawned_prop", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "set_spawned_prop", "get_spawned_prop");

	ClassDB::bind_method(D_METHOD("spawn_prop", "parent"), &VoxelChunkPropData::spawn_prop);
	ClassDB::bind_method(D_METHOD("free_prop"), &VoxelChunkPropData::free_prop);

	ClassDB::bind_method(D_METHOD("set_translation_for_chunk", "chunk", "local_x", "local_y", "local_z"), &VoxelChunkPropData::set_translation_for_chunk);

	ClassDB::bind_method(D_METHOD("get_mesh"), &VoxelChunkPropData::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &VoxelChunkPropData::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_mesh_texture"), &VoxelChunkPropData::get_mesh_texture);
	ClassDB::bind_method(D_METHOD("set_mesh_texture", "value"), &VoxelChunkPropData::set_mesh_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_mesh_texture", "get_mesh_texture");

	ClassDB::bind_method(D_METHOD("get_light"), &VoxelChunkPropData::get_light);
	ClassDB::bind_method(D_METHOD("set_light", "value"), &VoxelChunkPropData::set_light);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "PropDataLight"), "set_light", "get_light");

	ClassDB::bind_method(D_METHOD("get_prop"), &VoxelChunkPropData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &VoxelChunkPropData::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelChunkPropData::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelChunkPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");
}
