Design Document: Grid Manager

Overview

The Grid Manager is a core component responsible for managing a grid-based system in the game. It facilitates the creation, manipulation, and querying of grid cells and items placed on the grid. The Grid Manager supports both fully populated grids and sparse grids, making it adaptable to various use cases, such as farming plots, terrain management, and object placement.

Key Responsibilities
	1.	Grid Initialization:
	•	Define and create the grid structure.
	•	Populate cells with default attributes or specialized configurations.
	2.	Grid Cell Management:
	•	Store and update attributes for each cell (e.g., type, soil quality, water level).
	•	Provide fast access to grid cells via coordinates or world position.
	3.	Item Placement:
	•	Manage multi-cell items on the grid.
	•	Ensure collision checks and spatial updates are handled efficiently.
	4.	Updates and Growth:
	•	Iterate through and update managed items (e.g., plants growing, soil degrading).
	5.	Performance Optimization:
	•	Support both dense and sparse grid layouts.
	•	Minimize processing overhead for large grids or many items.
	6.	Debugging and Visualization:
	•	Provide tools to visualize the grid and its attributes in the editor and during gameplay.

Core Features
	1.	Grid Structure:
	•	Supports two main storage strategies:
	•	TArray for fully populated grids (dense).
	•	TMap for sparse grids or dynamic cell populations.
	•	Allows flexible cell sizing and dynamic dimensions.
	2.	Cell Attributes:
Each cell contains:
	•	Type: The kind of cell (e.g., Soil, Water, Rock).
	•	Soil Quality: Fertility level for planting.
	•	Water Level: Hydration level of the cell.
	•	Occupancy: Whether the cell is occupied by an item.
	3.	Multi-Cell Items:
	•	Supports items that span multiple cells.
	•	Handles placement, removal, and collision detection.
	•	Tracks attributes like size, position, and rotation.
	4.	Item Updates:
	•	Iterates through all managed items for updates (e.g., growth stages).
	•	Event-driven updates for specific changes (e.g., water added).
	5.	Debugging Tools:
	•	Visualization of grid structure and attributes in the editor.
	•	Color-coded debug overlays for different cell types.

System Design

Grid Cell Structure

```c++
USTRUCT(BlueprintType)
struct FGridCell
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
    FString CellType; // e.g., "Soil", "Water", "Rock"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
    float SoilQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
    float WaterLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
    bool bIsOccupied;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Cell")
    UGridItem* OccupyingItem;
};
```

Grid Item Structure

```c++
UCLASS(Blueprintable)
class YOURGAME_API UGridItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D OriginCell;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Width;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Height;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GrowthProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 GrowthStage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxGrowthStage;

    void Initialize(const FString& Name, FVector2D Origin, int32 InWidth, int32 InHeight);
};
```

Grid Manager API

Public Functions
	1.	Initialization
	•	void InitializeGrid(): Initializes a fully populated grid.
	•	void InitializeSparseGrid(): Initializes a sparse grid.
	2.	Cell Queries
	•	FGridCell* GetCellAt(int32 X, int32 Y): Retrieve a cell by its coordinates.
	•	FGridCell* GetCellAtWorldPosition(FVector WorldPosition): Retrieve a cell by its world position.
	3.	Item Management
	•	void AddItem(UGridItem* Item): Place an item on the grid.
	•	void RemoveItem(UGridItem* Item): Remove an item from the grid.
	•	bool CanPlaceItem(const UGridItem* Item): Check if an item can be placed at its specified location.
	4.	Updates
	•	void UpdateAllItems(float DeltaTime): Update all managed items (e.g., growth or decay).
	5.	Debugging
	•	void DebugDrawGrid(): Visualize the grid in the editor or during gameplay.
	•	void DebugDrawItem(const UGridItem* Item): Visualize an item on the grid.

Storage Strategies

Dense Grid (TArray)
	•	Use Case: Fully populated grids with predictable dimensions.
	•	Data Structure: TArray<FGridCell> with a size of GridWidth x GridHeight.

Sparse Grid (TMap)
	•	Use Case: Sparse or dynamically populated grids.
	•	Data Structure: TMap<int32, FGridCell> where the key is the cell index (Y * GridWidth + X).

Item Placement and Collision
	1.	Occupied Cell Validation:
	•	Calculate cells an item will occupy based on its size and origin.
	•	Check if these cells are free using bIsOccupied or TMap lookups.
	2.	Multi-Cell Management:
	•	Store references to the occupying item in all affected cells.

Growth Updates
	1.	Batched Updates:
	•	Divide ManagedItems into chunks for efficient updates.
	•	Process one chunk per frame to balance performance.
	2.	Event-Driven Updates:
	•	Trigger specific item updates based on grid changes (e.g., adding water).

Debugging Tools
	1.	Grid Visualization:
	•	Color-code cells based on type (e.g., green for soil, blue for water).
	•	Highlight occupied cells and their items.
	2.	Item Visualization:
	•	Draw bounding boxes for multi-cell items.
	•	Display growth progress or other attributes as overlays.

Performance Considerations
	1.	Spatial Partitioning:
	•	Divide the grid into chunks (e.g., 16x16 cells per chunk) for scalable updates and queries.
	2.	Hybrid Storage:
	•	Use TArray for dense regions and TMap for sparse or specialized areas.
	3.	Lazy Initialization:
	•	Initialize cells or chunks only when they are accessed.

Future Extensions
	1.	Dynamic Grid Expansion:
	•	Allow grids to grow dynamically based on player actions.
	2.	Advanced Interactions:
	•	Enable complex interactions between items and grid cells (e.g., soil depletion spreading across adjacent cells).
	3.	Save/Load Functionality:
	•	Serialize grid and item data for persistence.
