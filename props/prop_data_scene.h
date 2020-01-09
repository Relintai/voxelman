#ifndef PROP_DATA_SCENE_H
#define PROP_DATA_SCENE_H

#include "core/math/vector3.h"
#include "prop_data_entry.h"

#include "scene/resources/packed_scene.h"

class PropDataScene : public PropDataEntry {
	GDCLASS(PropDataScene, PropDataEntry);

public:
	Ref<PackedScene> get_scene() const;
	void set_scene(const Ref<PackedScene> value);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	PropDataScene();
	~PropDataScene();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<PackedScene> _scene;
};

#endif
