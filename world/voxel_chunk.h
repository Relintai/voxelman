#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "core/engine.h"
#include "core/object.h"
#include "core/reference.h"
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
#include "../props/voxelman_prop_data.h"

class VoxelChunk : public Reference {
	GDCLASS(VoxelChunk, Reference);

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

	Ref<VoxelmanLibrary> get_library();
	void set_library(Ref<VoxelmanLibrary> value);

	int get_lod_size() const;
	void set_lod_size(int lod_size);

	float get_voxel_scale() const;
	void set_voxel_scale(float value);

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

	void create_mesher();
	void _create_mesher();

	void finalize_mesh();

	void clear();
	void build();

	void create_colliders();
	void build_collider();
	void remove_colliders();

	void add_lights(Array lights);
	void add_voxel_light(Ref<VoxelLight> light);
	void remove_voxel_light(Ref<VoxelLight> light);
	void clear_voxel_lights();

	void append_lights(Array lights);
    Array get_lights();

    void bake_lights();
	void bake_light(Ref<VoxelLight> light);
	void clear_baked_lights();
    
	void add_prop_mesh(const Ref<MeshDataResource> mesh, const Vector3 position = Vector3(), const Vector3 rotation = Vector3(), const Vector3 scale = Vector3(1.0, 1.0, 1.0));
    void add_prop_spawned(const Ref<PackedScene> scene, const Vector3 position = Vector3(), const Vector3 rotation = Vector3(), const Vector3 scale = Vector3(1.0, 1.0, 1.0));
    void add_prop(const Ref<VoxelmanProp> prop, const Vector3 position = Vector3(), const Vector3 rotation = Vector3(), const Vector3 scale = Vector3(1.0, 1.0, 1.0));
	void clear_props();
    
    void build_props();
    
    void build_prop_mesh();
	void build_prop_collider();
	
	void spawn_spawned_props();
    void free_spawned_props();

    void allocate_main_mesh();
	void free_main_mesh();
    
	void allocate_prop_mesh();
	void free_prop_mesh();
    
    void allocate_prop_colliders();
	void free_prop_colliders();
    
	void create_debug_immediate_geometry();
	void free_debug_immediate_geometry();

	void free();

	void draw_debug_voxels(int max, Color color = Color(1, 1, 1));
	void draw_debug_voxel_lights();
	void draw_cross_voxels(Vector3 pos);
	void draw_cross_voxels(Vector3 pos, float fill);

	VoxelChunk();
	~VoxelChunk();

protected:
	struct VCPropData {
		Vector3 position;
		Vector3 rotation;
        Vector3 scale;
        
		Ref<MeshDataResource> mesh;
        Ref<VoxelmanProp> prop;
        Ref<PackedScene> scene;
	};
    
protected:
	static void _bind_methods();

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
	Vector<VCPropData> _props;

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
