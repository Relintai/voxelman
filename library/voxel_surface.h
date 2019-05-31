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
	int get_id() const;
	void set_id(int value);

	Color get_voxel_color() const;
	void set_voxel_color(Color value);

	int get_atlas_texture_id() const;
	void set_atlas_texture_id(int value);

	bool has_face_color() const;

	int get_mesh_id() const;
	void set_mesh_id(int value);

	Vector3 get_mesh_offset() const;
	void set_mesh_offset(Vector3 value);

	int get_prefab_id() const;
	void set_prefab_id(int value);

	Vector3 get_prefab_offset() const;
	void set_prefab_offset(Vector3 value);

	Vector3 get_prefab_rotation()const;
	void set_prefab_rotation(Vector3 value);

	bool get_light() const;
	void set_light(bool value);

	float get_light_strength() const;
	void set_light_strength(float value);

	Color get_light_color() const;
	void set_light_color(Color value);

	Vector3 get_light_offset() const;
	void set_light_offset(Vector3 value);

    bool is_transparent() const;
	void set_transparent(bool transparent);
	
    String get_voxel_name() const;
	void set_voxel_name(String name);
	
    Ref<VoxelmanLibrary> get_library() const;
	void set_library(Ref<VoxelmanLibrary> library);
	
	VoxelSurface();
	VoxelSurface(int id);
	~VoxelSurface();

protected:
	static void _bind_methods();

private:
	Ref<VoxelmanLibrary> _library;

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
