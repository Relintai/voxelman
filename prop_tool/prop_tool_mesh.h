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

#ifndef PROP_TOOL_MESH_H
#define PROP_TOOL_MESH_H

#include "core/math/vector3.h"

#include "../props/prop_data_mesh.h"
#include "scene/3d/mesh_instance.h"

class PropToolMesh : public MeshInstance {
	GDCLASS(PropToolMesh, MeshInstance);

#ifdef TOOLS_ENABLED
public:
	Ref<MeshDataResource> get_mesh_data();
	void set_mesh_data(const Ref<MeshDataResource> &data);

	Ref<PropDataMesh> get_data();
	void set_data(const Ref<PropDataMesh> &data);

	Ref<Texture> get_texture();
	void set_texture(const Ref<Texture> &tex);

	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	Vector3 get_snap_axis() const;
	void set_snap_axis(const Vector3 &value);

	Ref<MeshDataResource> get_prop_mesh();
	void set_prop_mesh(const Ref<MeshDataResource> &data);

	void generate();

	PropToolMesh();
	~PropToolMesh();

protected:
	bool get_generate() { return false; }
	void set_generate(bool value);

	static void _bind_methods();

private:
	Ref<MeshDataResource> _mesh_data;
	Ref<PropDataMesh> _prop_mesh;
	Ref<Texture> _texture;
	Ref<SpatialMaterial> _material;
	bool _snap_to_mesh;
	Vector3 _snap_axis;
#endif
};

#endif
