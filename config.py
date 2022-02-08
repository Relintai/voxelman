

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "WorldArea",

        "VoxelLight",
        "VoxelLightNode",

        "VoxelLevelGenerator",
        "VoxelLevelGeneratorFlat",

        "VoxelSurfaceMerger",
        "VoxelSurfaceSimple",
        "VoxelSurface",
        "VoxelLibraryMerger",
        "VoxelLibrarySimple",
        "VoxelLibrary",
        "VoxelLibraryMergerPCM",

        "VoxelMaterialCache",
        "VoxelMaterialCachePCM",

        "VoxelCubePoints",
        "VoxelMesherCubic",
        "VoxelMeshData",

        "MarchingCubesCellData",
        "VoxelMesherMarchingCubes",

        "VoxelMesher",

        "EnvironmentData",
        "VoxelChunk",
        "VoxelChunkDefault",
        "VoxelStructure",
        "BlockVoxelStructure",
        "VoxelWorld",

        "VoxelMesherBlocky",
        "VoxelWorldBlocky",
        "VoxelChunkBlocky",
        "VoxelMesherLiquidBlocky",

        "VoxelWorldMarchingCubes",
        "VoxelChunkMarchingCubes",

        "VoxelMesherCubic",
        "VoxelWorldCubic",
        "VoxelChunkCubic",

        "VoxelMesherDefault",
        "VoxelWorldDefault",

        "VoxelJob",
        "VoxelTerrainJob",
        "VoxelLightJob",
        "VoxelPropJob",
        "VoxelMesherJobStep",
    ]


def get_doc_path():
    return "doc_classes"
