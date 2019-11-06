#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "scene/3d/spatial.h"

#include "core/engine.h"
#include "core/ustring.h"
#include "scene/3d/mesh_instance.h"
#include "scene/resources/packed_scene.h"
#include "core/array.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/physics_body.h"
#include "scene/resources/concave_polygon_shape.h"
#include "scene/3d/spatial.h"

#include "voxel_world.h"

#include "../data/voxel_light.h"

#include "../meshers/cubic_mesher/voxel_mesher_cubic.h"
#include "../meshers/voxel_mesher.h"

#include "../library/voxel_surface.h"
#include "../library/voxelman_library.h"

#include "voxel_buffer.h"

#include "../../entity_spell_system/meshes/mesh_data_resource.h"
#include "../props/voxelman_prop.h"
#include "../props/voxelman_prop_entry.h"
#include "../props/voxelman_prop_scene.h"
#include "../props/voxelman_prop_mesh.h"
#include "../props/voxelman_prop_light.h"
#include "voxel_chunk_prop_data.h"

class VoxelWorld;

class VoxelChunk : public Spatial {
	GDCLASS(VoxelChunk, Spatial);

public:
	//Properties
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

	Ref<VoxelmanLibrary> get_library();
	void set_library(Ref<VoxelmanLibrary> value);

	int get_lod_size() const;
	void set_lod_size(int lod_size);

	float get_voxel_scale() const;
	void set_voxel_scale(float value);

	int get_current_build_phase();
	void set_current_build_phase(int value);

	Ref<VoxelMesher> get_mesher() const;
	void set_mesher(Ref<VoxelMesher> mesher);

	VoxelWorld *get_voxel_world() const;
	void set_voxel_world(VoxelWorld *world);
	void set_voxel_world_bind(Node *world);

	bool get_create_collider() const;
	void set_create_collider(bool value);

	bool get_bake_lights() const;
	void set_bake_lights(bool value);

	Ref<VoxelBuffer> get_buffer() const;

	RID get_mesh_rid();
	RID get_mesh_instance_rid();
	RID get_shape_rid();
	RID get_body_rid();

	RID get_prop_mesh_rid();
	RID get_prop_mesh_instance_rid();
	RID get_prop_shape_rid();
	RID get_prop_body_rid();

	//Meshing
	void create_mesher();
	void _create_mesher();

	void finalize_mesh();

	void build();
	void build_phase(int phase);
	void _build_phase(int phase);
	void next_phase();
	
	void clear();

	//Colliders
	void create_colliders();
	void build_collider();
	void remove_colliders();

	//lights
	void add_lights(Array lights);
	void add_voxel_light(Ref<VoxelLight> light);
	void create_voxel_light(const Color color, const int size, const int x, const int y, const int z);
	void remove_voxel_light(Ref<VoxelLight> light);
	void clear_voxel_lights();

	void add_lights_into(Array target);
    void add_unique_lights_into(Array target);
    Array get_lights();

    void bake_lights();
	void bake_light(Ref<VoxelLight> light);
	void clear_baked_lights();

	//props
	void add_prop(Ref<VoxelChunkPropData> prop);
	Ref<VoxelChunkPropData> get_prop(int index);
	int get_prop_count();
	void remove_prop(int index);
	void clear_props();

    void process_props();
    
	void build_prop_meshes();
	void build_prop_collider();
    void free_spawn_props();

	//Meshes
    void allocate_main_mesh();
	void free_main_mesh();
    
	void allocate_prop_mesh();
	void free_prop_mesh();
    
    void allocate_prop_colliders();
	void free_prop_colliders();

	//Debug
	void create_debug_immediate_geometry();
	void free_debug_immediate_geometry();

	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels_fill(Vector3 pos, float fill);
	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();

	//free
	void free_chunk();

	VoxelChunk();
	~VoxelChunk();

public:
	enum {
		BUILD_PHASE_DONE = 0,
		BUILD_PHASE_SETUP = 1,
		BUILD_PHASE_LIGHTS = 2,
		BUILD_PHASE_TERRARIN_MESH = 3,
		BUILD_PHASE_TERRARIN_MESH_COLLIDER = 4,
		BUILD_PHASE_PROP_MESH = 5,
		BUILD_PHASE_PROP_COLLIDER = 6,
		BUILD_PHASE_MAX = 7
	};

protected:
	static void _bind_methods();
	int _current_build_phase;
	bool _enabled;

	VoxelWorld *_voxel_world;

	Vector3i _chunk_position;
	Vector3i _chunk_size;

	Ref<VoxelBuffer> _buffer;
	Vector<Ref<VoxelLight> > _voxel_lights;

	int _lod_size;
	float _voxel_scale;

	NodePath _library_path;
	Ref<VoxelmanLibrary> _library;

	//voxel mesh
	RID _mesh_rid;
	RID _mesh_instance_rid;

	RID _shape_rid;
	RID _body_rid;

	Ref<VoxelMesher> _mesher;

	//mergeable props
	Vector<Ref<VoxelChunkPropData> > _props;

	RID _prop_mesh_rid;
	RID _prop_mesh_instance_rid;

	RID _prop_shape_rid;
	RID _prop_body_rid;

	//spawned props
	Vector<Node *> _spawned_props;

	//debug
	ImmediateGeometry *_debug_drawer;

	bool _build_mesh;
	bool _create_collider;

	bool _bake_lights;
};

#endif
