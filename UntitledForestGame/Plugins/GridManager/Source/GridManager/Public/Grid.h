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

    // Check if a cell is valid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsCellValid(int32 X, int32 Y) const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    int32 GetGridCellIndex(int32 X, int32 Y) const;

    FGridCell* GetGridCell(int32 X, int32 Y);

    bool GetGridCellAttributes(int32 X, int32 Y, FGridCellAttributes &Attributes) const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetGridSize() const;

    // Convert a grid cell to a world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector GridToWorld(const FVector2D& GridPosition) const;

    // Convert a world position to a grid cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D WorldToGrid(const FVector& WorldPosition) const;

    // Can an item be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtWorldPosition(const FVector& WorldPosition, const FVector& ItemSize, FVector2D& OutGridPosition, FTransform& SpawnTransform);

    // Can an item be placed at the given cell?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItem(const UGridItem* Item);

    // Place an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceItem(UGridItem* Item);

    // Remove an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool RemoveItem(UGridItem* Item);

    // Rotate an item
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void RotateItem(UGridItem* Item, float NewRotation);

    // get an item by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    UGridItem* GetItemAtWorldPosition(const FVector& WorldPosition);

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
