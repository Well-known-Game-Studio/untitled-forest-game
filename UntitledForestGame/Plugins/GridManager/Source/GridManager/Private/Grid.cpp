#include "Grid.h"
#include "GridItem.h"
#include "DrawDebugHelpers.h"

// Initialize the grid
void AGrid::InitializeGrid() {
  GridCells.Empty();

  for (int32 y = 0; y < GridHeight; ++y) {
    for (int32 x = 0; x < GridWidth; ++x) {
      // Create a new grid cell, the array holds pointers to the cells
      UGridCell* NewCell = NewObject<UGridCell>(this, GridCellClass);
      // set the cell type to be Ground
      NewCell->CellType = EGridCellType::Ground;
      NewCell->GridPosition = FVector2D(x, y);
      NewCell->WorldPosition = GridToWorld(FVector2D(x, y));
      NewCell->bIsOccupied = false;
      NewCell->OccupyingItem = nullptr;
      // Add the cell to the grid
      GridCells.Add(NewCell);
    }
  }
}

bool AGrid::IsCellValid(int32 X, int32 Y) const {
  return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

int32 AGrid::GetGridCellIndex(int32 X, int32 Y) const {
  return Y * GridWidth + X;
}

FVector2D AGrid::GetGridSize() const {
  return FVector2D(GridWidth, GridHeight);
}

/////// Get Grid Cell ///////

UGridCell* AGrid::GetGridCellAtXY(int32 X, int32 Y) {
  if (!IsCellValid(X, Y)) {
    return nullptr;
  }

  return GridCells[GetGridCellIndex(X, Y)];
}

UGridCell* AGrid::GetGridCellAtGridPosition(const FVector2D& GridPosition) {
  return GetGridCellAtXY(GridPosition.X, GridPosition.Y);
}

UGridCell* AGrid::GetGridCellAtIndex(int32 Index) {
  if (Index < 0 || Index >= GridCells.Num()) {
    return nullptr;
  }

  return GridCells[Index];
}

///////// Get Grid Cell Attributes /////////

UGridCellAttributes* AGrid::GetGridCellAttributes(int32 X, int32 Y) {
  if (!IsCellValid(X, Y)) {
    return nullptr;
  }

  UGridCell* Cell = GetGridCellAtXY(X, Y);
  if (!Cell) {
    return nullptr;
  }

  return Cell->Attributes;
}

UGridCellAttributes* AGrid::GetGridCellAttributesAtGridPosition(const FVector2D& GridPosition) {
  return GetGridCellAttributes(GridPosition.X, GridPosition.Y);
}

UGridCellAttributes* AGrid::GetGridCellAttributesAtWorldPosition(const FVector& WorldPosition) {
  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return nullptr;
  }

  return Cell->Attributes;
}

/////// Converters ///////

FVector2D AGrid::ItemSizeToGridSize(const FVector& ItemSize) const {
  return FVector2D(FMath::CeilToInt(ItemSize.X / CellSize), FMath::CeilToInt(ItemSize.Y / CellSize));
}

FVector AGrid::GridToWorld(const FVector2D& GridPosition) const {
  FVector WorldPosition = FVector(GridPosition.X * CellSize, GridPosition.Y * CellSize, 0);
  // take into account the AGrid's rotation
  auto Rotation = GetActorRotation();
  WorldPosition = FRotator(Rotation.Pitch, Rotation.Yaw, Rotation.Roll).RotateVector(WorldPosition);

  return WorldPosition + GetActorLocation();
}

FVector2D AGrid::WorldToGrid(const FVector& WorldPosition) const {
  // Transform the world position into the grid's local space
  FTransform GridTransform = GetActorTransform().Inverse();
  FVector RelativePosition = GridTransform.TransformPosition(WorldPosition);

  // if the relative position Z value is > CellSize or < 0, then it
  // is not on the grid, return an invalid position
  if (RelativePosition.Z > CellSize || RelativePosition.Z < 0) {
    return FVector2D(-1, -1);
  }

  int32 x = FMath::RoundToInt(RelativePosition.X / CellSize);
  int32 y = FMath::RoundToInt(RelativePosition.Y / CellSize);

  return FVector2D(x, y);
}

//// GET ITEMS ////

UGridItem* AGrid::GetItemAtXY(int32 X, int32 Y) {
  return GetItemAtGridPosition(FVector2D(X, Y));
}

UGridItem* AGrid::GetItemAtCell(const UGridCell* Cell) {
  if (!Cell) {
    return nullptr;
  }
  // Get the cell at the grid position
  return GetItemAtGridPosition(Cell->GridPosition);
}

UGridItem* AGrid::GetItemAtGridPosition(const FVector2D& GridPosition) {
  UGridCell *Cell = GetGridCellAtGridPosition(GridPosition);
  if (!Cell) {
    return nullptr;
  }

  return Cell->OccupyingItem;
}

// Get an item at a specific world position
UGridItem* AGrid::GetItemAtWorldPosition(const FVector& WorldPosition) {
  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return nullptr;
  }

  return Cell->OccupyingItem;
}

//// GET CELLS ////

UGridCell* AGrid::GetCellAtGridPosition(const FVector2D& GridPosition) const {
  int32 x = GridPosition.X;
  int32 y = GridPosition.Y;
  int32 Index = GetGridCellIndex(x, y);

  if (!IsCellValid(x,y) || Index >= GridCells.Num()) {
    return nullptr;
  }

  return GridCells[Index];
}

// Get a cell at a specific world position
UGridCell* AGrid::GetCellAtWorldPosition(const FVector& WorldPosition) const {
  // convert the position to be relative to the grid
  FVector2D GridPosition = WorldToGrid(WorldPosition);
  int32 x = GridPosition.X;
  int32 y = GridPosition.Y;
  int32 Index = GetGridCellIndex(x, y);

  if (!IsCellValid(x,y) || Index >= GridCells.Num()) {
    return nullptr;
  }

  return GridCells[Index];
}

UGridCell* AGrid::GetCellInDirectionFromWorldPosition(const FVector& WorldPosition, const FVector& Direction) const {
  auto CellPosition = WorldPosition + Direction * CellSize;
  return GetCellAtWorldPosition(CellPosition);
}

UGridCell* AGrid::GetCellInFrontOfActor(const AActor* Actor) const {
  if (!Actor) {
    return nullptr;
  }

  FVector ActorLocation = Actor->GetActorLocation();
  FVector ActorForwardVector = Actor->GetActorForwardVector();

  // get the up vector for the grid
  FVector GridUpVector = GetActorUpVector();
  // project the actor's forward vector onto the plane defined by the grid's up vector
  ActorForwardVector = ActorForwardVector - FVector::DotProduct(ActorForwardVector, GridUpVector) * GridUpVector;
  // re-normalize the vector
  ActorForwardVector.Normalize();

  return GetCellInDirectionFromWorldPosition(ActorLocation, ActorForwardVector);
}

///////// PLACEMENT Checks /////////

bool AGrid::CanPlaceInCell(const FVector &ItemSize, const UGridCell *Cell) const {
  auto cell_x = Cell->GridPosition.X;
  auto cell_y = Cell->GridPosition.Y;
  if (!IsCellValid(cell_x, cell_y) || !Cell) {
    return false;
  }

  int32 Index = GetGridCellIndex(cell_x, cell_y);
  auto *GridCell = GridCells[Index];

  if (GridCell->bIsOccupied) {
    return false;
  }

  // for the size of the item (width / height)
  FVector2D ItemGridSize = ItemSizeToGridSize(ItemSize);
  int32 width = ItemGridSize.X;
  int32 height = ItemGridSize.Y;

  for (int32 y = -height/2; y < height/2; ++y) {
    for (int32 x = -width/2; x < width/2; ++x) {
      FVector2D CellPosition = Cell->GridPosition + FVector2D(x, y);
      if (!IsCellValid(CellPosition.X, CellPosition.Y)) {
        return false;
      }
      Index = GetGridCellIndex(CellPosition.X, CellPosition.Y);
      if (Index < 0 || Index >= GridCells.Num() || GridCells[Index]->bIsOccupied) {
        return false;
      }
    }
  }

  return true;
}

bool AGrid::CanPlaceAtGridPosition(const FVector &ItemSize, const FVector2D &GridPosition) const {
  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceAtWorldPosition(const FVector &ItemSize, const FVector &WorldPosition) const {
  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceItemInCell(const UGridItem* Item, const UGridCell* GridCell) const {
  if (!Item) return false;
  if (!GridCell) return false;
  return CanPlaceInCell(Item->ItemSize, GridCell);
}

bool AGrid::CanPlaceItemAtGridPosition(const UGridItem* Item, const FVector2D& GridPosition) const {
  if (!Item) return false;

  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

bool AGrid::CanPlaceItemAtWorldPosition(const UGridItem* Item, const FVector& WorldPosition) const {
  if (!Item) return false;

  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

// Check if an item can be placed
bool AGrid::CanPlaceItem(const UGridItem* Item) const {
  if (!Item) return false;

  TArray<int32> CellsToCheck = Item->OccupiedCells;

  for (int32 CellIndex : CellsToCheck) {
    if (CellIndex < 0 || CellIndex >= GridCells.Num() || GridCells[CellIndex]->bIsOccupied) {
      return false; // Out of bounds or occupied
    }
  }

  return true;
}

///////// PLACEMENT /////////

// Place an item on the grid
bool AGrid::PlaceItem(UGridItem* Item)
{
  if (!Item) return false;

  // get the cell at the grid position for the item's origin cell
  UGridCell *Cell = GetCellAtGridPosition(Item->OriginCell);
  if (!Cell) {
    return false;
  }

  return PlaceItemInCell(Item, Cell);
}

bool AGrid::PlaceItemInCell(UGridItem* Item, UGridCell* GridCell) {
  if (!Item) return false;
  if (!GridCell) return false;

  if (!CanPlaceItemInCell(Item, GridCell)) {
    UE_LOG(LogTemp, Warning, TEXT("Cannot place item: %s"), *Item->ItemName);
    return false;
  }

  // Update the Item to the new position, which will update the occupied cells
  Item->Update(GridCell->GridPosition, Item->Rotation);

  TArray<int32> CellsToOccupy = Item->OccupiedCells;

  for (int32 CellIndex : CellsToOccupy) {
    GridCells[CellIndex]->bIsOccupied = true;
    GridCells[CellIndex]->OccupyingItem = Item;
  }

  UE_LOG(LogTemp, Log, TEXT("Item placed: %s"), *Item->ItemName);

  // add the item to the grid
  ManagedItems.Add(Item);

  return true;
}

bool AGrid::PlaceItemAtGridPosition(UGridItem* Item, const FVector2D& GridPosition) {
  if (!Item) return false;

  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return PlaceItemInCell(Item, Cell);
}

///////// REMOVAL /////////

// Remove an item from the grid
bool AGrid::RemoveItem(UGridItem* Item)
{
  if (!Item) return false;

  TArray<int32> CellsToFree = Item->OccupiedCells;

  for (int32 CellIndex : CellsToFree) {
    GridCells[CellIndex]->bIsOccupied = false;
    GridCells[CellIndex]->OccupyingItem = nullptr;
  }

  UE_LOG(LogTemp, Log, TEXT("Item removed: %s"), *Item->ItemName);

  // remove the item from the grid
  ManagedItems.Remove(Item);

  return true;
}

///////// MODIFICATION /////////

// Rotate an item
void AGrid::RotateItem(UGridItem* Item, float NewRotation) {
  if (!Item) return;

  RemoveItem(Item);

  Item->Update(Item->OriginCell, NewRotation);

  if (!CanPlaceItem(Item)) {
    UE_LOG(LogTemp, Warning, TEXT("Invalid rotation placement."));
    return;
  }

  PlaceItem(Item);
}

void AGrid::DrawCell(const UGridCell* Cell, const FColor &Color, float Duration) const {
  UWorld* World = GetWorld();
  if (!World) return;
  if (!Cell) return;

  FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
  FVector Center = Cell->WorldPosition;
  // Move the center up by half the size so the box is drawn at the correct location
  // we use the actor's up vector for this to handle the grid's rotation
  Center += GetActorUpVector() * CellSize / 2;
  // ensure the box is drawn at the correct location with the correct rotation
  DrawDebugBox(World, Center, HalfSize, GetActorQuat(), Color, false, Duration);
}

// Debug: Draw the grid
void AGrid::DebugDrawGrid() const {
  UWorld* World = GetWorld();
  if (!World) return;

  for (const UGridCell* Cell : GridCells) {
    FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
    FVector Center = Cell->WorldPosition;
    // Move the center up by half the size so the box is drawn at the correct location
    // we use the actor's up vector for this to handle the grid's rotation
    Center += GetActorUpVector() * CellSize / 2;
    // Set the color to blue by default
    auto Color = FColor::Blue;
    // Set the color to green if the cell is occupied
    if (Cell->bIsOccupied) {
      Color = FColor::Green;
    }
    // ensure the box is drawn at the correct location with the correct rotation
    DrawDebugBox(World, Center, HalfSize, GetActorQuat(), Color, false, -1.0f);
  }
}

// Debug: Draw an item
void AGrid::DebugDrawItem(const UGridItem* Item) const {
  UWorld* World = GetWorld();
  if (!World) return;

  if (!Item) return;

  TArray<int32> Cells = Item->OccupiedCells;
  for (int32 CellIndex : Cells) {
    FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
    FVector Center = GridCells[CellIndex]->WorldPosition;
    // Move the center up by half the size so the box is drawn at the correct location
    // we use the actor's up vector for this to handle the grid's rotation
    Center += GetActorUpVector() * CellSize / 2;
    // ensure the box is drawn at the correct location with the correct rotation
    DrawDebugBox(World, Center, HalfSize, GetActorQuat(), FColor::Green, false, -1.0f);
  }
}

// Constructor
AGrid::AGrid() {
  GridWidth = 10;
  GridHeight = 10;
  CellSize = 100.0f;
  PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
void AGrid::EditorTick(float DeltaTime) {
  DebugDrawGrid();
}
#endif

// This ultimately is what controls whether or not it can even tick at all in the editor view port. But, it is EVERY view port so it still needs to be blocked from preview windows and junk.
bool AGrid::ShouldTickIfViewportsOnly() const {
  if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor) {
    return true;
  } else {
    return false;
  }
}

void AGrid::Tick(float DeltaTime) {
  if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor) {
#if WITH_EDITOR
    EditorTick(DeltaTime);
    BlueprintEditorTick(DeltaTime);
#endif
  } else {
    Super::Tick(DeltaTime);
  }
}
