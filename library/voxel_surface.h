#ifndef VOXEL_SURFACE_DEFINITION_H
#define VOXEL_SURFACE_DEFINITION_H

#include "core/color.h"
#include "core/resource.h"
#include "core/vector.h"
#include "scene/resources/material.h"

#include "voxelman_library.h"

class VoxelmanLibrary;

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
		VOXEL_SIDE_TOP = 1 << 0,
		VOXEL_SIDE_BOTTOM = 1 << 1,
		VOXEL_SIDE_LEFT = 1 << 2,
		VOXEL_SIDE_FRONT = 1 << 3,
		VOXEL_SIDE_RIGHT = 1 << 4,
		VOXEL_SIDE_BACK = 1 << 5,

		VOXEL_SIDES_COUNT = 6,
	};

	int get_id() const;
	void set_id(int value);

	int get_atlas_x() const;
	void set_atlas_x(int value);

	int get_atlas_y() const;
	void set_atlas_y(int value);

    bool is_transparent() const;
	void set_transparent(bool transparent);
	
    String get_voxel_name() const;
	void set_voxel_name(String name);
	
    Ref<VoxelmanLibrary> get_library() const;
	void set_library(Ref<VoxelmanLibrary> library);
	
	VoxelSurface();
	VoxelSurface(int id);
	~VoxelSurface();

protected:
	static void _bind_methods();

private:
	Ref<VoxelmanLibrary> _library;

	int _id;
	String _name;
	int _atlas_x;
	int _atlas_y;

	bool _is_transparent;
};

VARIANT_ENUM_CAST(VoxelSurface::VoxelSurfaceSides);

#endif
