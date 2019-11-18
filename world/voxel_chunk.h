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

#include "../../entity_spell_system/meshes/mesh_data_resource.h"
#include "../props/prop_data.h"
#include "../props/prop_data_entry.h"
#include "../props/prop_data_scene.h"
#include "../props/prop_data_mesh.h"
#include "../props/prop_data_light.h"
#include "voxel_chunk_prop_data.h"

class VoxelWorld;

class VoxelChunk : public Spatial {
	GDCLASS(VoxelChunk, Spatial);

public:
	enum {
		VOXEL_CHUNK_STATE_OK = 0,
		VOXEL_CHUNK_STATE_GENERATION_QUEUED = 1,
		VOXEL_CHUNK_STATE_GENERATION = 2,
		VOXEL_CHUNK_STATE_MESH_GENERATION_QUEUED = 3,
		VOXEL_CHUNK_STATE_MESH_GENERATION = 4,
		VOXEL_CHUNK_STATE_MAX = 5,
	};

	enum {
		BUILD_PHASE_DONE = 0,
		BUILD_PHASE_SETUP = 1,
		BUILD_PHASE_TERRARIN_MESH_SETUP = 2,
		BUILD_PHASE_TERRARIN_MESH_COLLIDER = 3,
		BUILD_PHASE_LIGHTS = 4,
		BUILD_PHASE_TERRARIN_MESH = 5,
		BUILD_PHASE_PROP_MESH = 6,
		BUILD_PHASE_PROP_COLLIDER = 7,
		BUILD_PHASE_LIQUID = 8,
		BUILD_PHASE_CLUTTER = 9,
		BUILD_PHASE_MAX = 10
	};

	enum DefaultChannels {
		DEFAULT_CHANNEL_TYPE = 0,
		DEFAULT_CHANNEL_ISOLEVEL,
		DEFAULT_CHANNEL_LIGHT_COLOR_R,
		DEFAULT_CHANNEL_LIGHT_COLOR_G,
		DEFAULT_CHANNEL_LIGHT_COLOR_B,
        DEFAULT_CHANNEL_AO,
		DEFAULT_CHANNEL_RANDOM_AO,
		DEFAULT_CHANNEL_LIQUID_TYPES,
		DEFAULT_CHANNEL_LIQUID_FILL,
		DEFAULT_CHANNEL_LIQUID_FLOW,
		MAX_DEFAULT_CHANNELS
	};

public:
	bool get_dirty() const;
	void set_dirty(bool value);

	int get_state() const;
	void set_state(int value);

	int get_position_x();
	int get_position_y();
	int get_position_z();

	int get_size_x();
	int get_size_y();
	int get_size_z();

	int get_data_size_x();
	int get_data_size_y();
	int get_data_size_z();

	Vector3 get_position() const;
	Vector3 get_size() const;
	Vector3 get_data_size() const;

	int get_margin_start() const;
	int get_margin_end() const;

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

	RID get_mesh_rid();
	RID get_mesh_instance_rid();
	RID get_shape_rid();
	RID get_body_rid();

	RID get_prop_mesh_rid();
	RID get_prop_mesh_instance_rid();
	RID get_prop_shape_rid();
	RID get_prop_body_rid();

	RID get_liquid_mesh_rid();
	RID get_liquid_mesh_instance_rid();

	RID get_clutter_mesh_rid();
	RID get_clutter_mesh_instance_rid();

	//Voxel Data`
	void set_size(int size_x, int size_y, int siye_z, int margin_start = 0, int margin_end = 0);

	bool validate_channel_position(int x, int y, int z) const;

	uint8_t get_voxel(int x, int y, int z, int channel_index) const;
	void set_voxel(uint8_t value, int x, int y, int z, int channel_index);

	void set_channel_count(int count);
	void allocate_channel(int channel_index, uint8_t value = 0);
	void fill_channel(uint8_t value, int channel_index);
	void dealloc_channel(int channel_index);

	uint8_t *get_channel(int channel_index);
	uint8_t *get_valid_channel(int channel_index);
	
	//Data Management functions
	void generate_ao();

    void add_light(int local_x, int local_y, int local_z, int size, Color color);
	void clear_baked_lights();

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
    
    void add_prop_light(Ref<VoxelLight> light);

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

	void allocate_liquid_mesh();
	void free_liquid_mesh();

	void allocate_clutter_mesh();
	void free_clutter_mesh();

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

protected:
	static void _bind_methods();

	bool _dirty;
	int _state;

	int _current_build_phase;
	bool _enabled;

	VoxelWorld *_voxel_world;

	int _position_x;
	int _position_y;
	int _position_z;

	uint32_t _size_x;
	uint32_t _size_y;
	uint32_t _size_z;

	uint32_t _data_size_x;
	uint32_t _data_size_y;
	uint32_t _data_size_z;

	uint32_t _margin_start;
	uint32_t _margin_end;

	Vector<uint8_t *> _channels;

	Vector<Ref<VoxelLight> > _voxel_lights;

	int _lod_size;
	float _voxel_scale;

	NodePath _library_path;
	Ref<VoxelmanLibrary> _library;
	Ref<VoxelMesher> _mesher;

	//voxel mesh
	RID _mesh_rid;
	RID _mesh_instance_rid;

	RID _shape_rid;
	RID _body_rid;

	//mergeable props
	Vector<Ref<VoxelChunkPropData> > _props;

	RID _prop_mesh_rid;
	RID _prop_mesh_instance_rid;

	RID _prop_shape_rid;
	RID _prop_body_rid;

	//liquids
	RID _liquid_mesh_rid;
	RID _liquid_mesh_instance_rid;

	//clutter
	RID _clutter_mesh_rid;
	RID _clutter_mesh_instance_rid;

	//spawned props
	Vector<Node *> _spawned_props;

	//debug
	ImmediateGeometry *_debug_drawer;

	bool _build_mesh;
	bool _create_collider;

	bool _bake_lights;
};

VARIANT_ENUM_CAST(VoxelChunk::DefaultChannels);

#endif
