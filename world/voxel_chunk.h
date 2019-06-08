#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "core/engine.h"
#include "core/object.h"
#include "core/ustring.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/spatial.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/physics_body.h"
#include "scene/resources/concave_polygon_shape.h"

#include "../data/voxel_light.h"

#include "../meshers/voxel_mesher.h"

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

#include "voxel_buffer.h"


class VoxelChunk : public Spatial {
	GDCLASS(VoxelChunk, Spatial);

public:
	NodePath get_library_path();
	void set_library_path(NodePath value);

	NodePath get_mesh_instance_path();
	void set_mesh_instance_path(NodePath value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(Ref<VoxelmanLibrary> value);

	float get_voxel_scale();
	void set_voxel_scale(float value);

	Ref<VoxelMesher> get_mesher() const;
	void set_mesher(Ref<VoxelMesher> mesher);

	bool get_build_mesh();
	void set_build_mesh(bool value);

	bool get_create_collider();
	void set_create_collider(bool value);

	bool get_bake_lights();
	void set_bake_lights(bool value);

	NodePath get_debug_drawer_path();
	void set_debug_drawer_path(NodePath value);

	Ref<VoxelBuffer> get_buffer() const;

	void _create_mesher();

	void finalize_mesh();

	void clear();
	void build();

	void update_collider();

	void set_enabled(bool p_enabled);
	bool is_enabled() const;

	Ref<VoxelLight> add_voxel_light(Vector3i position, Color color, float strength, Vector3 offset = Vector3());
	void add_voxel_light_bind(Vector3 position, Color color, float strength);

	StaticBody *create_trimesh_collision_node();

	VoxelChunk();
	virtual ~VoxelChunk();

	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights(int max, bool localPosition = false);
	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels(Vector3 pos, float fill);

protected:
	static void _bind_methods();

	bool _enabled;

	Ref<VoxelBuffer> _buffer;
	Vector<Ref<VoxelLight> > _voxel_lights;
	float _voxel_scale;

	NodePath _library_path;
	Ref<VoxelmanLibrary> _library;

	NodePath _mesh_instance_path;
	MeshInstance *_mesh_instance;
	Ref<ArrayMesh> _mesh;

	Ref<VoxelMesher> _mesher;

	ImmediateGeometry *_debug_drawer;
	NodePath _debug_drawer_path;

	bool _build_mesh;
	bool _create_collider;

	bool _bake_lights;
};

#endif
