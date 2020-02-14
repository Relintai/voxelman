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

#ifndef VOXEL_TOOLS_H
#define VOXEL_TOOLS_H

#include "core/color.h"
#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/pool_vector.h"
#include "core/reference.h"
#include "core/vector.h"
#include "scene/3d/immediate_geometry.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/spatial.h"
#include "scene/main/node.h"
#include "scene/resources/concave_polygon_shape.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/surface_tool.h"

#include "../../mesh_data_resource/mesh_data_resource.h"
#include "../library/voxelman_library.h"

const double PI_2 = 3.141592653589793238463 / 2;
const double PI = 3.141592653589793238463;

class VoxelmanLibrary;
class VoxelChunk;

class VoxelMesher : public Reference {
	GDCLASS(VoxelMesher, Reference);

public:
	int get_mesher_index() const;
	void set_mesher_index(const int value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(const Ref<VoxelmanLibrary> &library);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	float get_ao_strength() const;
	void set_ao_strength(const float value);

	float get_base_light_value() const;
	void set_base_light_value(const float value);

	float get_voxel_scale() const;
	void set_voxel_scale(const float voxel_scale);

	int get_lod_size() const;
	void set_lod_size(const int lod_size);

	Rect2 get_uv_margin() const;
	void set_uv_margin(const Rect2 margin);

	void reset();

	void add_chunk_bind(Node *chunk);
	void add_chunk(VoxelChunk *chunk);

	void add_chunk_liquid_bind(Node *chunk);
	void add_chunk_liquid(VoxelChunk *chunk);

	void add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position = Vector3(0, 0, 0), const Vector3 rotation = Vector3(0, 0, 0), const Vector3 scale = Vector3(1.0, 1.0, 1.0), const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect = Rect2(0, 0, 1, 1));

	void add_mesher(const Ref<VoxelMesher> &mesher);
	void _add_mesher(const Ref<VoxelMesher> &mesher);

	void bake_colors_bind(Node *chunk);
	void bake_colors(VoxelChunk *chunk);
	void _bake_colors(Node *p_chunk);

	void bake_liquid_colors_bind(Node *chunk);
	void bake_liquid_colors(VoxelChunk *chunk);
	void _bake_liquid_colors(Node *p_chunk);

	void build_collider(RID shape) const;

	void bake_lights(MeshInstance *node, Vector<Ref<VoxelLight> > &lights);

	void build_mesh(RID mesh);

	PoolVector<Vector3> get_vertices();
	void set_vertices(const PoolVector<Vector3> &values);
	int get_vertex_count();
	Vector3 get_vertex(int idx);
	void remove_vertex(int idx);
	void add_vertex(Vector3 vertex);

	PoolVector<Vector3> get_normals();
	void set_normals(const PoolVector<Vector3> &values);
	int get_normal_count();
	Vector3 get_normal(int idx);
	void remove_normal(int idx);
	void add_normal(Vector3 normal);

	PoolVector<Color> get_colors();
	void set_colors(const PoolVector<Color> &values);
	int get_color_count();
	Color get_color(int idx);
	void remove_color(int idx);
	void add_color(Color color);

	PoolVector<Vector2> get_uvs();
	void set_uvs(const PoolVector<Vector2> &values);
	int get_uv_count();
	Vector2 get_uv(int idx);
	void remove_uv(int idx);
	void add_uv(Vector2 vector);

	PoolVector<Vector2> get_uv2s();
	void set_uv2s(const PoolVector<Vector2> &values);
	int get_uv2_count();
	Vector2 get_uv2(int idx);
	void remove_uv2(int idx);
	void add_uv2(Vector2 vector);

	PoolVector<int> get_indices();
	void set_indices(const PoolVector<int> &values);
	int get_indices_count();
	int get_indice(int idx);
	void remove_indices(int idx);
	void add_indices(int index);

	VoxelMesher(const Ref<VoxelmanLibrary> &library);
	VoxelMesher();
	~VoxelMesher();

protected:
	static void _bind_methods();

	int _mesher_index;

	PoolVector<Vector3> _vertices;
	PoolVector<Vector3> _normals;
	PoolVector<Color> _colors;
	PoolVector<Vector2> _uvs;
	PoolVector<Vector2> _uv2s;
	PoolVector<int> _indices;
	PoolVector<int> _bones;

	Ref<VoxelmanLibrary> _library;
	Ref<Material> _material;

	float _voxel_scale;
	int _lod_size;

	Ref<SurfaceTool> _surface_tool;

	float _ao_strength;
	float _base_light_value;
	Rect2 _uv_margin;
};

#endif
