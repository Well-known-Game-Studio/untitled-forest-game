#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "Grid.generated.h"

class UGridItem;

UCLASS()
class UNTITLEDFORESTGAME_API AGrid : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AGrid();

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

    // Initialize the grid
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void InitializeGrid();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetGridSize() const;

    // Convert a world position to a grid cell
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D WorldToGrid(const FVector& WorldPosition) const;

    // Can an item be placed at the given world position?
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool CanPlaceItemAtWorldPosition(const FVector& WorldPosition, const FVector& ItemSize);

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

    // Get a cell by world position
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool GetCellAtWorldPosition(const FVector& WorldPosition, FGridCell& Cell);

    // Debugging
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawGrid();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void DebugDrawItem(const UGridItem* Item);
};
