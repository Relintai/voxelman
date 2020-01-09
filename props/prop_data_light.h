#ifndef PROP_DATA_LIGHT_H
#define PROP_DATA_LIGHT_H

#include "prop_data_entry.h"

#include "core/color.h"

class PropDataLight : public PropDataEntry {
	GDCLASS(PropDataLight, PropDataEntry);

public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size() const;
	void set_light_size(const int value);

	PropDataLight();
	~PropDataLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size;
};

#endif
