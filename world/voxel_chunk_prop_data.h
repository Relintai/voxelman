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

#include "core/resource.h"

#include "core/math/transform.h"

#include "scene/main/node.h"

#include "scene/resources/packed_scene.h"

class VoxelChunk;

class VoxelChunkPropData : public Resource {
	GDCLASS(VoxelChunkPropData, Resource);

public:
	Ref<VoxelChunk> get_owner();
	void set_owner(const Ref<VoxelChunk> &chunk);

	int get_scene_id() const;
	void set_scene_id(const int id);

	Transform get_transform() const;
	void set_transform(const Transform &value);

	Ref<PackedScene> get_scene();
	void set_scene(const Ref<PackedScene> &value);

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
	Ref<VoxelChunk> _owner;
	int _scene_id;
	Transform _transform;
	Node *_spawned_prop;
	Ref<PackedScene> _scene;
};

#endif
