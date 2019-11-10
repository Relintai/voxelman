#ifndef GROUND_CLUTTER_FOLIAGE_H
#define GROUND_CLUTTER_FOLIAGE_H

#include "ground_clutter.h"

#include "core/vector.h"

#include "scene/resources/texture.h"

class GroundClutterFoliage : public GroundClutter {
	GDCLASS(GroundClutterFoliage, GroundClutter);

public:
	int get_texture_count() const;
	Ref<Texture> get_texture(const int index);
	void remove_texture(const int index);
	void add_texture(Ref<Texture> texture);

	GroundClutterFoliage();
	~GroundClutterFoliage();

private:
	static void _bind_methods();

private:
	Vector<Ref<Texture> > _textures;
};

#endif
