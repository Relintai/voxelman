#ifndef VOXELMAN_PROP_SCENE_H
#define VOXELMAN_PROP_SCENE_H

#include "voxelman_prop_entry.h"

#include "scene/resources/packed_scene.h"

class VoxelmanPropScene : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropScene, VoxelmanPropEntry);
	
public:
	Ref<PackedScene> get_scene() const;
	void set_scene(const Ref<PackedScene> value);

	VoxelmanPropScene();
	~VoxelmanPropScene();

protected:
	static void _bind_methods();

private:
	Ref<PackedScene> _scene;
};

#endif
