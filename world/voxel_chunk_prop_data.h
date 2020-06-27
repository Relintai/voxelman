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

#ifndef VOXEL_CHUNK_PROP_DATA_H
#define VOXEL_CHUNK_PROP_DATA_H

#include "core/math/transform.h"
#include "core/resource.h"
#include "scene/main/node.h"
#include "scene/resources/packed_scene.h"

#include "../../mesh_data_resource/mesh_data_resource.h"
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_light.h"
#include "scene/resources/texture.h"

class VoxelChunk;

class VoxelChunkPropData : public Resource {
	GDCLASS(VoxelChunkPropData, Resource);

public:
	int get_x() const;
	void set_x(const int value);

	int get_y() const;
	void set_y(const int value);

	int get_z() const;
	void set_z(const int value);

	Vector3 get_rotation() const;
	void set_rotation(const Vector3 &value);

	Vector3 get_scale() const;
	void set_scale(const Vector3 &value);

	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	Vector3 get_snap_axis() const;
	void set_snap_axis(const Vector3 &value);

	Ref<MeshDataResource> get_mesh();
	void set_mesh(const Ref<MeshDataResource> &value);

	Ref<Texture> get_mesh_texture();
	void set_mesh_texture(const Ref<Texture> &value);

	Ref<PropDataLight> get_light();
	void set_light(const Ref<PropDataLight> &value);

	Ref<PropData> get_prop();
	void set_prop(const Ref<PropData> &value);

	Ref<PackedScene> get_scene();
	void set_scene(const Ref<PackedScene> &value);

	Ref<VoxelChunk> get_owner();
	void set_owner(const Ref<VoxelChunk> &chunk);

	int get_scene_id() const;
	void set_scene_id(const int id);

	Transform get_transform() const;
	void set_transform(const Transform &value);

	Node *get_spawned_prop() const;
	void set_spawned_prop(Node *value);

	Node *spawn_prop(Node *parent);
	void free_prop();

	void set_translation_for_chunk(const Ref<VoxelChunk> &chunk, const int local_x, const int local_y, const int local_z);

	VoxelChunkPropData();
	~VoxelChunkPropData();

protected:
	static void _bind_methods();

private:
	int _id;

	int _x;
	int _y;
	int _z;
	Vector3 _rotation;
	Vector3 _scale;

	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Ref<VoxelChunk> _owner;
	int _scene_id;
	Transform _transform;
	Node *_spawned_prop;
	Ref<PackedScene> _scene;

	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
	Ref<PropDataLight> _light;
	Ref<PropData> _prop;
};

#endif
