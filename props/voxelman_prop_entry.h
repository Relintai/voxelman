#ifndef VOXELMAN_PROP_DATA_H
#define VOXELMAN_PROP_DATA_H

#include "core/reference.h"
#include "core/math/vector3.h"
#include "core/color.h"
#include "scene/resources/packed_scene.h"

#include "../../entity_spell_system/meshes/mesh_data_resource.h"

class VoxelmanProp;

class VoxelmanPropEntry : public Resource {
	GDCLASS(VoxelmanPropEntry, Resource);
	
public:
	Ref<MeshDataResource> get_mesh() const;
	void set_mesh(const Ref<MeshDataResource> mesh);

	int get_has_light() const;
	void set_has_light(const int value);

	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size() const;
	void set_light_size(const int value);

	Ref<PackedScene> get_scene() const;
	void set_scene(const Ref<PackedScene> value);

	Vector3 get_position() const;
	void set_position(const Vector3 value);

	Vector3 get_rotation() const;
	void set_rotation(const Vector3 value);

	Vector3 get_scale() const;
	void set_scale(const Vector3 value);

	Ref<VoxelmanProp> get_prop() const;
	void set_prop(const Ref<VoxelmanProp> prop);

	VoxelmanPropEntry();
	~VoxelmanPropEntry();

protected:
	static void _bind_methods();

private:
	Ref<MeshDataResource> _mesh;

	bool _has_light;
	Color _light_color;
	int _light_size;

	Ref<PackedScene> _scene;

	Ref<VoxelmanProp> _prop;

	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
};

#endif
