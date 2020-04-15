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
	_spawned_prop = NULL;
	_scene_id = 0;
}
VoxelChunkPropData::~VoxelChunkPropData() {
	_owner.unref();
	_scene.unref();
}

void VoxelChunkPropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_owner"), &VoxelChunkPropData::get_owner);
	ClassDB::bind_method(D_METHOD("set_owner", "value"), &VoxelChunkPropData::set_owner);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "owner", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), "set_owner", "get_owner");

	ClassDB::bind_method(D_METHOD("get_scene_id"), &VoxelChunkPropData::get_scene_id);
	ClassDB::bind_method(D_METHOD("set_scene_id", "value"), &VoxelChunkPropData::set_scene_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "scene_id"), "set_scene_id", "get_scene_id");

	ClassDB::bind_method(D_METHOD("get_transform"), &VoxelChunkPropData::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &VoxelChunkPropData::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_scene"), &VoxelChunkPropData::get_scene);
	ClassDB::bind_method(D_METHOD("set_scene", "value"), &VoxelChunkPropData::set_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_scene", "get_scene");

	ClassDB::bind_method(D_METHOD("get_spawned_prop"), &VoxelChunkPropData::get_spawned_prop);
	ClassDB::bind_method(D_METHOD("set_spawned_prop", "value"), &VoxelChunkPropData::set_spawned_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "spawned_prop", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "set_spawned_prop", "get_spawned_prop");

	ClassDB::bind_method(D_METHOD("spawn_prop", "parent"), &VoxelChunkPropData::spawn_prop);
	ClassDB::bind_method(D_METHOD("free_prop"), &VoxelChunkPropData::free_prop);

	ClassDB::bind_method(D_METHOD("set_translation_for_chunk", "chunk", "local_x", "local_y", "local_z"), &VoxelChunkPropData::set_translation_for_chunk);
}
