#ifndef TRANSVOXEL_CELL_DATA_H
#define TRANSVOXEL_CELL_DATA_H

#include "core/reference.h"

#include "transvoxel_tables.h"

using namespace Transvoxel;

class TransvoxelCellData : public Reference {
	GDCLASS(TransvoxelCellData, Reference)

public:
	int get_vertex_index(int index) const;
    void set_vertex_index(int index, int value);
	int get_vertex_count() const;
    void set_vertex_count(int value);
	int get_triangle_count() const;
    void set_triangle_count(int value);

	TransvoxelCellData();
	TransvoxelCellData(const RegularCellData &cell_data);
    TransvoxelCellData(const TransitionCellData &cell_data);
	~TransvoxelCellData();

protected:
	static void _bind_methods();

private:
	long geometryCounts; // High nibble is vertex count, low nibble is triangle count.
	unsigned char vertexIndex[36]; // Groups of 3 indexes giving the triangulation.
};

#endif
