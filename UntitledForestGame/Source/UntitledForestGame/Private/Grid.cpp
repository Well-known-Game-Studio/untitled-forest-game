#include "Grid.h"
#include "UfgGameplayFunctionLibrary.h"
#include "GridItem.h"
#include "DrawDebugHelpers.h"

// Constructor
AGrid::AGrid()
{
    GridWidth = 10;
    GridHeight = 10;
    CellSize = 100.0f;
    PrimaryActorTick.bCanEverTick = true;
}

// Initialize the grid
void AGrid::InitializeGrid()
{
    GridCells.Empty();

    for (int32 y = 0; y < GridHeight; ++y)
    {
        for (int32 x = 0; x < GridWidth; ++x)
        {
            FGridCell NewCell;
            // set the cell type to be Ground
            NewCell.CellType = EGridCellType::Ground;
            NewCell.WorldPosition = FVector(x * CellSize, y * CellSize, 0.0f);
            EGroundType ground_type = EGroundType::Grass;
            NewCell.Attributes = UUfgGameplayFunctionLibrary::RandomizeGridCellAttributes(ground_type);
            NewCell.bIsOccupied = false;
            NewCell.OccupyingActor = nullptr;

            GridCells.Add(NewCell);
        }
    }
}

FVector2D AGrid::GetGridSize() const
{
    return FVector2D(GridWidth, GridHeight);
}

FVector2D AGrid::WorldToGrid(const FVector& WorldPosition) const
{
    FVector RelativePosition = WorldPosition - GetActorLocation();

    int32 x = FMath::FloorToInt(RelativePosition.X / CellSize);
    int32 y = FMath::FloorToInt(RelativePosition.Y / CellSize);

    return FVector2D(x, y);
}

// Get a cell at a specific world position
bool AGrid::GetCellAtWorldPosition(const FVector& WorldPosition, FGridCell& Cell)
{
  // convert the position to be relative to the grid
  FVector2D GridPosition = WorldToGrid(WorldPosition);
  int32 x = GridPosition.X;
  int32 y = GridPosition.Y;
  int32 Index = x + y * GridWidth;

  if (Index < 0 || Index >= GridCells.Num())
  {
      return false;
  }

  Cell = GridCells[Index];

  return true;
}

bool AGrid::CanPlaceItemAtWorldPosition(const FVector& WorldPosition, const FVector& ItemSize)
{
  FGridCell Cell;
  if (!GetCellAtWorldPosition(WorldPosition, Cell))
  {
      return false;
  }

  FVector2D ItemGridSize = FVector2D(FMath::CeilToInt(ItemSize.X / CellSize), FMath::CeilToInt(ItemSize.Y / CellSize));

  int32 width = ItemGridSize.X;
  int32 height = ItemGridSize.Y;

  FVector2D GridPosition = WorldToGrid(WorldPosition);

  for (int32 y = -height/2; y < height/2; ++y)
  {
      for (int32 x = -width/2; x < width/2; ++x)
      {
        FVector2D CellPosition = GridPosition + FVector2D(x, y);
          int32 Index = CellPosition.X + CellPosition.Y * GridWidth;
          if (Index < 0 || Index >= GridCells.Num() || GridCells[Index].bIsOccupied)
          {
              return false;
          }
      }
  }

  return true;
}

// Check if an item can be placed
bool AGrid::CanPlaceItem(const UGridItem* Item)
{
  if (!Item) return false;

    TArray<int32> CellsToCheck = Item->OccupiedCells;

    for (int32 CellIndex : CellsToCheck)
    {
        if (CellIndex < 0 || CellIndex >= GridCells.Num() || GridCells[CellIndex].bIsOccupied)
        {
            return false; // Out of bounds or occupied
        }
    }

    return true;
}

// Place an item on the grid
bool AGrid::PlaceItem(UGridItem* Item)
{
    if (!Item) return false;

    if (!CanPlaceItem(Item))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot place item: %s"), *Item->ItemName);
        return false;
    }

    TArray<int32> CellsToOccupy = Item->OccupiedCells;

    for (int32 CellIndex : CellsToOccupy)
    {
        GridCells[CellIndex].bIsOccupied = true;
    }

    UE_LOG(LogTemp, Log, TEXT("Item placed: %s"), *Item->ItemName);

    return true;
}

// Remove an item from the grid
bool AGrid::RemoveItem(UGridItem* Item)
{
    if (!Item) return false;

    TArray<int32> CellsToFree = Item->OccupiedCells;

    for (int32 CellIndex : CellsToFree)
    {
        GridCells[CellIndex].bIsOccupied = false;
    }

    UE_LOG(LogTemp, Log, TEXT("Item removed: %s"), *Item->ItemName);

    return true;
}

// Rotate an item
void AGrid::RotateItem(UGridItem* Item, float NewRotation)
{
    if (!Item) return;

    RemoveItem(Item);

    Item->Update(Item->OriginCell, NewRotation);

    if (!CanPlaceItem(Item))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid rotation placement."));
        return;
    }

    PlaceItem(Item);
}

// Debug: Draw the grid
void AGrid::DebugDrawGrid()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (const FGridCell& Cell : GridCells)
    {
        FVector Center = Cell.WorldPosition + FVector(CellSize / 2, CellSize / 2, 0);
        DrawDebugBox(World, Center, FVector(CellSize / 2, CellSize / 2, 50), FColor::Blue, false, -1.0f);
    }
}

// Debug: Draw an item
void AGrid::DebugDrawItem(const UGridItem* Item)
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (!Item) return;

    TArray<int32> Cells = Item->OccupiedCells;
    for (int32 CellIndex : Cells)
    {
        FVector Center = GridCells[CellIndex].WorldPosition + FVector(CellSize / 2, CellSize / 2, 0);
        DrawDebugBox(World, Center, FVector(CellSize / 2, CellSize / 2, 50), FColor::Green, false, -1.0f);
    }
}
