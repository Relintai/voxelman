#ifndef VOXEL_SURFACE_H
#define VOXEL_SURFACE_H

#include "core/resource.h"

#include "core/color.h"
#include "core/math/rect2.h"
#include "core/vector.h"
#include "scene/resources/material.h"

#include "../clutter/ground_clutter.h"
#include "voxelman_library.h"

class VoxelmanLibrary;
class GroundClutter;

class VoxelSurface : public Resource {
	GDCLASS(VoxelSurface, Resource)

public:
	/*
	   _____
	 /_____/I
	I     I I  <- BACK
	I  F  I I
	I_____I/


	    I y 
	    I        I
	    O----- x I B
	  /  I F  I /
	/ z+ I ---I/
	*/

	enum VoxelSurfaceSides {
		VOXEL_SIDE_TOP = 0,
		VOXEL_SIDE_BOTTOM = 1,
		VOXEL_SIDE_SIDE = 2,
	};

	enum {
		VOXEL_SIDES_COUNT = 3,
		VOXEL_SIDES_ARRAY_SIZE = VOXEL_SIDES_COUNT * 2,
	};

	int get_id() const;
	void set_id(const int value);

	bool is_transparent() const;
	void set_transparent(const bool transparent);

	Rect2 get_rect(const VoxelSurfaceSides side) const;
	void set_rect(const VoxelSurfaceSides side, const Rect2 rect);

	Ref<GroundClutter> get_clutter();
	void set_clutter(Ref<GroundClutter> clutter);

	Ref<VoxelmanLibrary> get_library() const;
	void set_library(Ref<VoxelmanLibrary> library);

	Vector2 transform_uv(const VoxelSurfaceSides side, const Vector2 uv) const;

	virtual void refresh_rects();

	VoxelSurface();
	~VoxelSurface();

protected:
	static void _bind_methods();

	VoxelmanLibrary *_library;

	int _id;
	bool _is_transparent;
	Rect2 _rects[VOXEL_SIDES_COUNT];
	Ref<GroundClutter> _clutter;
};

VARIANT_ENUM_CAST(VoxelSurface::VoxelSurfaceSides);

#endif
