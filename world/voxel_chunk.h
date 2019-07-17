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
#include "core/array.h"

#include "../data/voxel_light.h"

#include "../meshers/voxel_mesher.h"
#include "../meshers/cubic_mesher/voxel_mesher_cubic.h"

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

#include "voxel_buffer.h"


class VoxelChunk : public Spatial {
	GDCLASS(VoxelChunk, Spatial);

public:
	int get_chunk_position_x();
	void set_chunk_position_x(int value);
	int get_chunk_position_y();
	void set_chunk_position_y(int value);
	int get_chunk_position_z();
	void set_chunk_position_z(int value);

	Vector3i get_chunk_position() const;
	void set_chunk_position(int x, int y, int z);

	int get_chunk_size_x();
	void set_chunk_size_x(int value);
	int get_chunk_size_y();
	void set_chunk_size_y(int value);
	int get_chunk_size_z();
	void set_chunk_size_z(int value);

	Vector3i get_chunk_size() const;
	void set_chunk_size(int x, int y, int z);

	NodePath get_mesh_instance_path() const;
	void set_mesh_instance_path(NodePath value);

	Ref<VoxelmanLibrary> get_library();
	void set_library(Ref<VoxelmanLibrary> value);

	int get_lod_size() const;
	void set_lod_size(int lod_size);

	float get_voxel_scale() const;
	void set_voxel_scale(float value);

	Ref<VoxelMesher> get_mesher() const;
	void set_mesher(Ref<VoxelMesher> mesher);

	bool get_build_mesh() const;
	void set_build_mesh(bool value);

	bool get_create_collider() const;
	void set_create_collider(bool value);

	bool get_bake_lights() const;
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

	void add_lights(Array lights);
	void add_voxel_light(Ref<VoxelLight> light);
	void remove_voxel_light(Ref<VoxelLight> light);
	void clear_voxel_lights();

	void get_lights(Array lights);

	void bake_lights();
	void bake_light(Ref<VoxelLight> light);
	void clear_baked_lights();

	StaticBody *create_trimesh_collision_node();

	VoxelChunk();
	~VoxelChunk();

	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();
	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels(Vector3 pos, float fill);

protected:
	static void _bind_methods();

	bool _enabled;

	Vector3i _chunk_position;
	Vector3i _chunk_size;

	Ref<VoxelBuffer> _buffer;
	Vector<Ref<VoxelLight> > _voxel_lights;

	int _lod_size;
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
