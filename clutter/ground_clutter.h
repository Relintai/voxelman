#ifndef GROUND_CLUTTER_H
#define GROUND_CLUTTER_H

#include "core/resource.h"

class GroundClutter : public Resource {
	GDCLASS(GroundClutter, Resource);

public:
	GroundClutter();
	~GroundClutter();

private:
	static void _bind_methods();

private:
};

#endif
