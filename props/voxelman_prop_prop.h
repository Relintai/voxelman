#ifndef VOXELMAN_PROP_PROP_H
#define VOXELMAN_PROP_PROP_H

#include "voxelman_prop_entry.h"
#include "core/math/vector3.h"

#include "voxelman_prop.h"

class VoxelmanPropProp : public VoxelmanPropEntry {
	GDCLASS(VoxelmanPropProp, VoxelmanPropEntry);
	
public:
	Ref<VoxelmanProp> get_prop() const;
	void set_prop(const Ref<VoxelmanProp> value);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	VoxelmanPropProp();
	~VoxelmanPropProp();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<VoxelmanProp> _prop;
};

#endif
