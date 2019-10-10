#ifndef VOXELMAN_PROP_SCENE_H
#define VOXELMAN_PROP_SCENE_H

#include "voxelman_prop_entry.h"
#include "core/math/vector3.h"

#include "scene/resources/packed_scene.h"

class VoxelmanPropScene : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropScene, VoxelmanPropEntry);
	
public:
	Ref<PackedScene> get_scene() const;
	void set_scene(const Ref<PackedScene> value);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	VoxelmanPropScene();
	~VoxelmanPropScene();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<PackedScene> _scene;
};

#endif
