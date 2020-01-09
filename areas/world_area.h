#ifndef WORLD_AREA_H
#define WORLD_AREA_H

#include "core/reference.h"

#include "core/math/aabb.h"
#include "core/ustring.h"
#include "scene/resources/texture.h"

class WorldArea : public Reference {
	GDCLASS(WorldArea, Reference);

public:
	AABB get_aabb() const;
	void set_aabb(const AABB value);

	Ref<Texture> get_map_texture() const;
	void set_map_texture(const Ref<Texture> value);

	Ref<Texture> get_fov_texture() const;
	void set_fov_texture(const Ref<Texture> value);

	String get_name() const;
	void set_name(const String value);

	int get_level() const;
	void set_level(const int value);

	WorldArea();
	~WorldArea();

private:
	static void _bind_methods();

private:
	AABB _aabb;
	Ref<Texture> _map_texture;
	Ref<Texture> _fov_texture;
	String _name;
	int _level;
};

#endif
