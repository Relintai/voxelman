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

#include "core/math/vector3.h"
#include "core/reference.h"

#include "../../mesh_data_resource/mesh_data_resource.h"
#include "../props/prop_data.h"
#include "../props/prop_data_light.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/texture.h"

class VoxelChunkPropData : public Reference {
	GDCLASS(VoxelChunkPropData, Reference);

public:
	int get_x();
	void set_x(int value);

	int get_y();
	void set_y(int value);

	int get_z();
	void set_z(int value);

	Vector3 get_rotation();
	void set_rotation(Vector3 value);

	Vector3 get_scale();
	void set_scale(Vector3 value);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> value);

	Ref<Texture> get_mesh_texture() const;
	void set_mesh_texture(const Ref<Texture> value);

	Ref<PropDataLight> get_light() const;
	void set_light(const Ref<PropDataLight> value);

	Ref<PropData> get_prop() const;
	void set_prop(const Ref<PropData> value);

	Ref<PackedScene> get_scene() const;
	void set_scene(const Ref<PackedScene> value);

	VoxelChunkPropData();
	~VoxelChunkPropData();

protected:
	static void _bind_methods();

private:
	int _x;
	int _y;
	int _z;
	Vector3 _rotation;
	Vector3 _scale;

	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Ref<MeshDataResource> _mesh;
	Ref<Texture> _texture;
	Ref<PropDataLight> _light;
	Ref<PropData> _prop;
	Ref<PackedScene> _scene;
};

#endif
