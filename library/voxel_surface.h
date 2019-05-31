#ifndef VOXEL_SURFACE_DEFINITION_H
#define VOXEL_SURFACE_DEFINITION_H

#include "core/color.h"
#include "core/resource.h"
#include "core/vector.h"
#include "scene/resources/material.h"

#include "voxelman_library.h"

class VoxelmanLibrary;

class VoxelSurface : public Resource {
	GDCLASS(VoxelSurface, Resource)

public:
	int get_id() { return _id; }
	void set_id(int value) { _id = value; }

	Color get_voxel_color() { return _voxel_color; }
	void set_voxel_color(Color value) { _voxel_color = value; }

	int get_atlas_texture_id() { return _atlas_texture_id; }
	void set_atlas_texture_id(int value) { _atlas_texture_id = value; }

	bool has_face_color() {
		return false;
	}

	int get_mesh_id() { return _mesh_id; }
	void set_mesh_id(int value) { _mesh_id = value; }

	Vector3 get_mesh_offset() { return _mesh_offset; }
	void set_mesh_offset(Vector3 value) { _mesh_offset = value; }

	int get_prefab_id() { return _prefab_id; }
	void set_prefab_id(int value) { _prefab_id = value; }

	Vector3 get_prefab_offset() { return _prefab_offset; }
	void set_prefab_offset(Vector3 value) { _prefab_offset = value; }

	Vector3 get_prefab_rotation() { return _prefab_rotation; }
	void set_prefab_rotation(Vector3 value) { _prefab_rotation = value; }

	bool get_light() { return _light; }
	void set_light(bool value) { _light = value; }

	float get_light_strength() { return _light_strength; }
	void set_light_strength(float value) { _light_strength = value; }

	Color get_light_color() { return _light_color; }
	void set_light_color(Color value) { _light_color = value; }

	Vector3 get_light_offset() { return _light_offset; }
	void set_light_offset(Vector3 value) { _light_offset = value; }

	void set_transparent(bool t = true);
	bool is_transparent() const { return _is_transparent; }

	void set_voxel_name(String name);
	String get_voxel_name() const { return _name; }

	void set_library(Ref<VoxelmanLibrary> lib);
	Ref<VoxelmanLibrary> get_library();

	VoxelSurface();
	VoxelSurface(int id);
	~VoxelSurface();

protected:
	static void _bind_methods();

private:
	VoxelmanLibrary *_library;

	int _id;
	String _name;
	Color _voxel_color;
	int _atlas_texture_id;

	//These should probably be in a separate data class, which isn't going to be limited to 256 entries.
	int _mesh_id;
	Vector3 _mesh_offset;

	int _prefab_id;
	Vector3 _prefab_offset;
	Vector3 _prefab_rotation;

	bool _light;
	float _light_strength;
	Color _light_color;
	Vector3 _light_offset;

	bool _is_transparent;
};

#endif
