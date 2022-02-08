/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VOXEL_SURFACE_H
#define VOXEL_SURFACE_H


#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/io/resource.h"
#include "core/templates/vector.h"
#include "core/math/color.h"
#else
#include "core/resource.h"
#include "core/vector.h"
#include "core/color.h"
#endif

#include "core/math/rect2.h"
#include "scene/resources/material.h"

#include "voxel_library.h"

class VoxelLibrary;

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

	int get_mesher_index() const;
	void set_mesher_index(const int value);

	bool get_transparent() const;
	void set_transparent(const bool value);

	bool get_liquid() const;
	void set_liquid(const bool value);

	Rect2 get_rect(const VoxelSurfaceSides side) const;
	void set_rect(const VoxelSurfaceSides side, const Rect2 &rect);

	Ref<VoxelLibrary> get_library() const;
	void set_library(Ref<VoxelLibrary> library);

	Vector2 transform_uv(const VoxelSurfaceSides p_side, const Vector2 &p_uv) const;
	Vector2 transform_uv_scaled(const VoxelSurfaceSides p_side, const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const;

	virtual void refresh_rects();

	VoxelSurface();
	~VoxelSurface();

protected:
	static void _bind_methods();

	VoxelLibrary *_library;

	int _id;
	int _mesher_index;
	bool _transparent;
	bool _liquid;
	Rect2 _rects[VOXEL_SIDES_COUNT];
};

VARIANT_ENUM_CAST(VoxelSurface::VoxelSurfaceSides);

#endif
