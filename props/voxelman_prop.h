#ifndef VOXELMAN_PROP_H
#define VOXELMAN_PROP_H

#include "core/reference.h"
#include "core/vector.h"

#include "voxelman_prop_entry.h"

class VoxelmanProp : public Resource {
	GDCLASS(VoxelmanProp, Resource);

public:
	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	Ref<VoxelmanPropEntry> get_prop(const int index) const;
	void set_prop(const int index, const Ref<VoxelmanPropEntry> prop);
	void add_prop(const Ref<VoxelmanPropEntry> prop);
	void remove_prop(const int index);

	int get_prop_count() const;

	Vector<Variant> get_props();
	void set_props(const Vector<Variant> &props);

	VoxelmanProp();
	~VoxelmanProp();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Vector<Ref<VoxelmanPropEntry> > _props;
};

#endif
