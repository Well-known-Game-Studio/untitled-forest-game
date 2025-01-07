#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

class UGridItem;

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

    // Grid dimensions and settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    int32 GridWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
    float CellSize;

    // Grid cells
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FGridCell> GridCells;

    // List of all items managed by this grid
    UPROPERTY()
    TArray<UGridItem*> ManagedItems;

    static FGridCellAttributes RandomizeGridCellAttributes(EGroundType GroundType);

    // Initialize the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void InitializeGrid();

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
    FGridCell* GetGridCell(const FVector2D& GridPosition);
    FGridCell* GetGridCellAtXY(int32 X, int32 Y);
    FGridCell* GetGridCellAtIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetGridCellAttributes(int32 X, int32 Y, FGridCellAttributes &Attributes) const;

    // Can an item of the given size be placed in the given cell?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceInCell(const FVector &ItemSize, const FGridCell &Cell);
    // Can an item of the given size be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceAtGridPosition(const FVector &ItemSize, const FVector2D &GridPosition);
    // Can an item of the given size be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceAtWorldPosition(const FVector &WorldPosition, const FVector &ItemSize);

    // Can an item be placed in it's OriginCell?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItem(const UGridItem* Item);

    // Can an item be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemInCell(const UGridItem* Item, const FGridCell& GridCell);
    // Can an item be placed at the given grid position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtGridPosition(const UGridItem* Item, const FVector2D& GridPosition);
    // Can an item be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtWorldPosition(const UGridItem* Item, const FVector& WorldPosition);

    // Place an item at its OriginCell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItem(UGridItem* Item);
    // Place an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItemInCell(UGridItem* Item, const FGridCell& GridCell);
    // Place an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItemAtGridPosition(UGridItem* Item, const FVector2D& GridPosition);

    // Remove an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool RemoveItem(UGridItem* Item);

    // Rotate an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void RotateItem(UGridItem* Item, float NewRotation);

    // get an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridItem* GetItemAtXY(int32 X, int32 Y);
    // get an item from a cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridItem* GetItemAtCell(const FGridCell& Cell);
    // get an item at a specific grid position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridItem* GetItemAtGridPosition(const FVector2D& GridPosition);
    // get an item at a specific world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridItem* GetItemAtWorldPosition(const FVector& WorldPosition);

    // Get a cell by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetCellAtGridPosition(const FVector2D& GridPosition, FGridCell& Cell);
    // Get a cell by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetCellAtWorldPosition(const FVector& WorldPosition, FGridCell& Cell);
    // Get the grid cell in front of a location by the direction vector
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetCellInDirectionFromWorldPosition(const FVector& WorldPosition, const FVector& Direction, FGridCell& Cell);
    // Get the grid cell in front of a given actor, using the actor's forward vector
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetCellInFrontOfActor(const AActor* Actor, FGridCell& Cell);

    // Draw a cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DrawCell(const FGridCell& Cell, const FColor& Color, float Duration = 0.0f);

    // Debugging
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawGrid();
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawItem(const UGridItem* Item);
};
