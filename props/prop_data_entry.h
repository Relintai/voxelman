#ifndef PROP_DATA_DATA_H
#define PROP_DATA_DATA_H

#include "core/math/transform.h"
#include "core/resource.h"

class PropDataEntry : public Resource {
	GDCLASS(PropDataEntry, Resource);

public:
	Transform get_transform() const;
	void set_transform(const Transform value);

	PropDataEntry();
	~PropDataEntry();

protected:
	static void _bind_methods();

private:
	Transform _transform;
};

#endif
