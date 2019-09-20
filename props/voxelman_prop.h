#ifndef VOXELMAN_PROP_H
#define VOXELMAN_PROP_H

#include "core/reference.h"
#include "core/vector.h"

#include "voxelman_prop_entry.h"

class VoxelmanProp : public Resource {
	GDCLASS(VoxelmanProp, Resource);

public:
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
	Vector<Ref<VoxelmanPropEntry> > _props;
};

#endif
