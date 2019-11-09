#ifndef DUNGEON_CORRIDOR_DATA_H
#define DUNGEON_CORRIDOR_DATA_H

#include "dungeon_room_data.h"

#include "../main/dungeon_corridor.h"

class DungeonCorridorData : public DungeonRoomData {
	GDCLASS(DungeonCorridorData, DungeonRoomData);
	
public:
	int get_max_connections();
	void set_max_connections(int value);

	Ref<DungeonCorridor> setup_corridor(int seed);

	DungeonCorridorData();
	~DungeonCorridorData();

protected:
	static void _bind_methods();

private:
	int _max_connections;
};

#endif
