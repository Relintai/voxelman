#ifndef PROP_DATA_ENTITY_H
#define PROP_DATA_ENTITY_H

#include "prop_data_entry.h"

class PropDataEntity : public PropDataEntry {
	GDCLASS(PropDataEntity, PropDataEntry);
	
public:
	int get_entity_data_id() const;
	void set_entity_data_id(const int value);

	int get_level() const;
	void set_level(const int value);

	PropDataEntity();
	~PropDataEntity();

protected:
	static void _bind_methods();

private:
	int _level;
	int _entity_data_id;
};

#endif
