#ifndef VOXEL_TOOLS_H
#define VOXEL_TOOLS_H

#include "core/reference.h"
#include "core/color.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/vector.h"
#include "scene/3d/mesh_instance.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/surface_tool.h"
#include "scene/3d/immediate_geometry.h"
#include "scene/3d/spatial.h"
#include "scene/main/node.h"
#include "scene/resources/concave_polygon_shape.h"

#include "../library/voxelman_library.h"
#include "../math/vector3i.h"
#include "../world/voxel_buffer.h"
#include "../../entity_spell_system/meshes/mesh_data_resource.h"

const double PI_2 = 3.141592653589793238463 / 2;
const double PI = 3.141592653589793238463;

class VoxelmanLibrary;
class Voxel;

class VoxelMesher : public Reference {
	GDCLASS(VoxelMesher, Reference);

public:
	Ref<VoxelmanLibrary> get_library() { return _library; }
	void set_library(Ref<VoxelmanLibrary> library) { _library = library; }

	void reset();

	void add_buffer(Ref<VoxelBuffer> voxels);
	void add_mesh_data_resource(Transform local_transform, Ref<MeshDataResource> mesh);
	void bake_colors(Ref<VoxelBuffer> voxels);

	float get_voxel_scale() const;
	void set_voxel_scale(const float voxel_scale);

	int get_lod_size() const;
	void set_lod_size(const int lod_size);

	void build_collider(RID shape) const;

	void bake_lights(MeshInstance *node, Vector<Ref<VoxelLight> > &lights);

	void build_mesh(RID mesh);

	Vector<Vector3> *get_vertices();
	int get_vertex_count();
	Vector3 get_vertex(int idx);
	void remove_vertex(int idx);
	void add_vertex(Vector3 vertex);

	Vector<Vector3> *get_normals();
	int get_normal_count();
	Vector3 get_normal(int idx);
	void remove_normal(int idx);
	void add_normal(Vector3 normal);

	Vector<Color> *get_colors();
	int get_color_count();
	Color get_color(int idx);
	void remove_color(int idx);
	void add_color(Color color);

	Vector<Vector2> *get_uvs();
	int get_uv_count();
	Vector2 get_uv(int idx);
	void remove_uv(int idx);
	void add_uv(Vector2 vector);

	Vector<int> *get_indices();
	int get_indices_count();
	int get_indice(int idx);
	void remove_indices(int idx);
	void add_indices(int index);

	VoxelMesher(Ref<VoxelmanLibrary> library);
	VoxelMesher();
	~VoxelMesher();

protected:
	static void _bind_methods();

	Vector<Vector3> _vertices;
	Vector<Vector3> _normals;
	Vector<Color> _colors;
	Vector<Vector2> _uvs;
	Vector<int> _indices;
	Vector<int> _bones;

	Ref<VoxelmanLibrary> _library;

	float _voxel_scale;
	int _lod_size;

	Ref<SurfaceTool> _surface_tool;
};

#endif
