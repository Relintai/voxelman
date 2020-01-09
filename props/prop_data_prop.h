#ifndef PROP_DATA_PROP_H
#define PROP_DATA_PROP_H

#include "core/math/vector3.h"
#include "prop_data_entry.h"

#include "prop_data.h"

class PropDataProp : public PropDataEntry {
	GDCLASS(PropDataProp, PropDataEntry);

public:
	Ref<PropData> get_prop() const;
	void set_prop(const Ref<PropData> value);

	bool get_snap_to_mesh();
	void set_snap_to_mesh(bool value);

	Vector3 get_snap_axis();
	void set_snap_axis(Vector3 value);

	PropDataProp();
	~PropDataProp();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<PropData> _prop;
};

#endif
