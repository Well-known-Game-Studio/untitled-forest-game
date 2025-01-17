#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GRIDMANAGER_API AGrid : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AGrid();

#if WITH_EDITOR
    /** Tick that runs ONLY in the editor viewport.*/
    void EditorTick(float DeltaTime);
#endif

    /** Tick that runs ONLY in the editor viewport.*/
    UFUNCTION(BlueprintImplementableEvent, CallInEditor, Category = "Events")
    void BlueprintEditorTick(float DeltaTime);

    virtual void Tick(float DeltaTime) override;

    // Allows us to draw debug information in the editor
    virtual bool ShouldTickIfViewportsOnly() const override;

    // Grid cell attributes class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    TSubclassOf<UGridCellAttributes> CellAttributesClass;

    // Grid dimensions and settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    int32 GridWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    float CellSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    TSubclassOf<UGridCell> GridCellClass = UGridCell::StaticClass();

    // Grid cells
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced)
    TArray<UGridCell*> GridCells;

    // List of all items managed by this grid
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced)
    TArray<AActor*> ManagedItems;

    // Initialize the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void InitializeGrid();

    // Get the size of the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetGridSize() const;

    // Get the number of grid cells the item occupies
    FVector2D ItemSizeToGridSize(const FVector& ItemSize) const;

    // Convert a grid cell to a world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector GridToWorld(const FVector2D& GridPosition) const;
    // Convert a world position to a grid cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D WorldToGrid(const FVector& WorldPosition) const;

    // Check if a cell is valid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsCellValid(int32 X, int32 Y) const;

    int32 GetGridCellIndex(int32 X, int32 Y) const;
    UGridCell* GetGridCellAtXY(int32 X, int32 Y) const;
    UGridCell* GetGridCellAtGridPosition(const FVector2D& GridPosition) const;
    UGridCell* GetGridCellAtIndex(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCellAttributes* GetGridCellAttributes(int32 X, int32 Y);
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCellAttributes* GetGridCellAttributesAtGridPosition(const FVector2D& GridPosition);
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCellAttributes* GetGridCellAttributesAtWorldPosition(const FVector& WorldPosition);

    // Get the grid component of an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridComponent* GetGridComponent(const AActor* Item) const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    TArray<UGridCell*> GetCells(const FVector2D& GridPosition, const FVector2D& GridSize) const;

    // Is this actor a placeable item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsPlaceableItem(const AActor* Item) const;

    // Can the item be rotated to the new rotation
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanRotateItem(AActor* Item, float NewRotation);

    // Check if the cells at and around the GridPosition are empty. If Item is
    // not null, then the cells will be considered free if the item is the one
    // that is occupying them. If Item is nullptr, then any occupying item will
    // return false.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CheckIfCellsAreFree(const FVector2D &GridPosition, const FVector2D &ItemSize, const AActor *Item=nullptr) const;

    // Can an item of the given size be placed in the given cell?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceInCell(const FVector2D &ItemSize, const UGridCell *Cell) const;
    // Can an item of the given size be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceAtGridPosition(const FVector2D &ItemSize, const FVector2D &GridPosition) const;
    // Can an item of the given size be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceAtWorldPosition(const FVector2D &ItemSize, const FVector &WorldPosition) const;

    // Can an item be placed at its ActorLocation?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItem(const AActor* Item) const;

    // Can an item be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemInCell(const AActor* Item, const UGridCell* GridCell) const;
    // Can an item be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtGridPosition(const AActor* Item, const FVector2D& GridPosition) const;
    // Can an item be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtWorldPosition(const AActor* Item, const FVector& WorldPosition) const;

    // Place an item at its ActorLocation
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItem(AActor* Item);
    // Place an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItemInCell(AActor* Item, UGridCell* GridCell);
    // Place an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItemAtGridPosition(AActor* Item, const FVector2D& GridPosition);

    // Remove an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool RemoveItem(AActor* Item);

    // Rotate an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool RotateItem(AActor* Item, float NewRotation);

    // get an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    AActor* GetItemAtXY(int32 X, int32 Y);
    // get an item from a cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    AActor* GetItemAtCell(const UGridCell* Cell);
    // get an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    AActor* GetItemAtGridPosition(const FVector2D& GridPosition);
    // get an item at a specific world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    AActor* GetItemAtWorldPosition(const FVector& WorldPosition);

    // Get a cell by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCell* GetCellAtGridPosition(const FVector2D& GridPosition) const;
    // Get a cell by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCell* GetCellAtWorldPosition(const FVector& WorldPosition) const;
    // Get the grid cell in front of a location by the direction vector
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCell* GetCellInDirectionFromWorldPosition(const FVector& WorldPosition, const FVector& Direction) const;
    // Get the grid cell in front of a given actor, using the actor's forward vector
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridCell* GetCellInFrontOfActor(const AActor* Actor) const;

    // Draw a cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DrawCell(const UGridCell* Cell, const FColor& Color, float Duration = 0.0f) const;

    // Debugging
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawGrid() const;
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawItem(const AActor* Item) const;
};
