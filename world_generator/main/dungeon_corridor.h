#ifndef DUNGEON_CORRIDOR_H
#define DUNGEON_CORRIDOR_H

#include "dungeon_room.h"


class DungeonCorridor : public DungeonRoom {
	GDCLASS(DungeonCorridor, DungeonRoom);
	
public:
	int get_max_connections();
	void set_max_connections(int value);

	//Rooms
	Ref<DungeonRoom> get_dungeon_room(const int index) const;
	void set_dungeon_room(const int index, const Ref<DungeonRoom> dungeon_room);
	void add_dungeon_room(const Ref<DungeonRoom> dungeon_room);
	void remove_dungeon_room(const int index);

	int get_dungeon_room_count() const;

	DungeonCorridor();
	~DungeonCorridor();

protected:
	static void _bind_methods();

private:
	int _max_connections;
	Vector<Ref<DungeonRoom> > _dungeon_rooms;
};

#endif
