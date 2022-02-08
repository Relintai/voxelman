
#ifndef VOXEL_DEFINES_H
#define VOXEL_DEFINES_H

#include "core/version.h"

#if VERSION_MAJOR >= 4
#define GODOT4 true
#endif

//includes
#if GODOT4
#define core_input_h "core/input/input_event.h"
#define spatial_editor_plugin_h "editor/plugins/node_3d_editor_plugin.h"
#define camera_h "scene/3d/camera_3d.h"
#define spatial_h "scene/3d/node_3d.h"
#define navigation_h "scene/3d/node_3d.h"
#define Navigation3D Node3D
#define light_h "scene/3d/light_3d.h"
#define visual_server_h "servers/rendering_server.h"
#define mesh_instance_h "scene/3d/mesh_instance_3d.h"
#define pool_vector_h "core/templates/vector.h"
#define physics_server_h "servers/physics_server_3d.h"
#define immediate_geometry_h "scene/3d/immediate_geometry_3d.h"
#define include_pool_vector \
	template <class N>      \
	class Vector;           \
	template <class N>      \
	using PoolVector = Vector<N>;
#else
#define core_input_h "core/os/input.h"
#define spatial_editor_plugin_h "editor/plugins/spatial_editor_plugin.h"
#define camera_h "scene/3d/camera.h"
#define spatial_h "scene/3d/spatial.h"
#define navigation_h "scene/3d/navigation.h"
#define light_h "scene/3d/light.h"
#define visual_server_h "servers/visual_server.h"
#define mesh_instance_h "scene/3d/mesh_instance.h"
#define pool_vector_h "core/pool_vector.h"
#define physics_server_h "servers/physics_server.h"
#define immediate_geometry_h "scene/3d/immediate_geometry.h"
#define include_pool_vector
#endif

//Type Defines
#if GODOT4
#define PhysicsDirectSpaceState PhysicsDirectSpaceState3D
#define SpatialEditor Node3DEditor
#define SpatialEditorPlugin Node3DEditorPlugin
#define SpatialEditorViewport Node3DEditorViewport
#define PoolStringArray PackedStringArray
#define REAL FLOAT
#define POOL_STRING_ARRAY PACKED_STRING_ARRAY
#define POOL_BYTE_ARRAY PACKED_BYTE_ARRAY
#define Spatial Node3D
#define SpatialMaterial StandardMaterial3D
#define PoolVector3Array PackedVector3Array
#define PoolVector2Array PackedVector2Array
#define PoolColorArray PackedColorArray
#define PoolIntArray PackedInt64Array
#define PoolRealArray PackedFloat32Array
#define PoolByteArray PackedByteArray
#define MeshInstance MeshInstance3D
#define Camera Camera3D
#define ToolButton Button
#define Shape Shape3D
#define Reference RefCounted

typedef class World3D World;

#define DirectionalLight DirectionalLight3D

typedef class ImmediateGeometry3D ImmediateGeometry;
typedef class Navigation3D Navigation;
typedef class PhysicsServer3D PhysicsServer;
typedef class RenderingServer VisualServer;
typedef class RenderingServer VS;
//typedef class StandardMaterial3D SpatialMaterial;

//toodo figure out a way to have this
//#define Variant::CallError Callable::CallError
#endif

#if GODOT4

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i]);               \
	}                                      \
	return r;

#else

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

#endif

#define VARIANT_ARRAY_SET(arr, arr_into, type) \
	arr_into.clear();                          \
	for (int i = 0; i < arr.size(); i++) {     \
		Ref<type> e = Ref<type>(arr[i]);       \
		arr_into.push_back(e);                 \
	}

#if GODOT4
//TODO do this properly
#define INSTANCE_VALIDATE(var) var
#define CONNECT(sig, obj, target_method_class, method) connect(sig, callable_mp(obj, &target_method_class::method))
#define DISCONNECT(sig, obj, target_method_class, method) disconnect(sig, callable_mp(obj, &target_method_class::method))
#define GET_WORLD get_world_3d
#define INSTANCE instantiate
#define VREMOVE remove_at
#else
#define INSTANCE_VALIDATE(var) ObjectDB::instance_validate(var)
#define CONNECT(sig, obj, target_method_class, method) connect(sig, obj, #method)
#define DISCONNECT(sig, obj, target_method_class, method) disconnect(sig, obj, #method)
#define GET_WORLD get_world
#define INSTANCE instance
#define VREMOVE remove
#endif

#endif