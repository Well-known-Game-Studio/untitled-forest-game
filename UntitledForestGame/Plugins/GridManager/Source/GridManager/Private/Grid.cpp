#include "Grid.h"
#include "GridComponent.h"
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

UGridCell* AGrid::GetGridCellAtXY(int32 X, int32 Y) const {
  if (!IsCellValid(X, Y)) {
    return nullptr;
  }

  return GridCells[GetGridCellIndex(X, Y)];
}

UGridCell* AGrid::GetGridCellAtGridPosition(const FVector2D& GridPosition) const {
  return GetGridCellAtXY(GridPosition.X, GridPosition.Y);
}

UGridCell* AGrid::GetGridCellAtIndex(int32 Index) const {
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

AActor* AGrid::GetItemAtXY(int32 X, int32 Y) {
  return GetItemAtGridPosition(FVector2D(X, Y));
}

AActor* AGrid::GetItemAtCell(const UGridCell* Cell) {
  if (!Cell) {
    return nullptr;
  }
  // Get the cell at the grid position
  return GetItemAtGridPosition(Cell->GridPosition);
}

AActor* AGrid::GetItemAtGridPosition(const FVector2D& GridPosition) {
  UGridCell *Cell = GetGridCellAtGridPosition(GridPosition);
  if (!Cell) {
    return nullptr;
  }

  return Cell->OccupyingItem;
}

// Get an item at a specific world position
AActor* AGrid::GetItemAtWorldPosition(const FVector& WorldPosition) {
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

///////// Actor Checks /////////

UGridComponent* AGrid::GetGridComponent(const AActor* Item) const {
  if (!Item) return nullptr;

  return Cast<UGridComponent>(Item->GetComponentByClass(UGridComponent::StaticClass()));
}

bool AGrid::IsPlaceableItem(const AActor* Item) const {
  if (!Item) return false;

  return GetGridComponent(Item) != nullptr;
}

///////// PLACEMENT Checks /////////

bool AGrid::CheckIfCellsAreFree(const FVector2D &GridPosition, const FVector2D &ItemSize, const AActor* Item) const {
  if (!IsCellValid(GridPosition.X, GridPosition.Y)) {
    return false;
  }

  // check the cell at grid position
  int32 Index = GetGridCellIndex(GridPosition.X, GridPosition.Y);
  if (GridCells[Index]->IsOccupied()) {
    if (Item == nullptr || GridCells[Index]->OccupyingItem != Item) {
      return false;
    }
  }

  // now check the cells around it
  int32 width = ItemSize.X;
  int32 height = ItemSize.Y;
  // for the size of the item (width / height)
  for (int32 y = -height/2; y < height/2; ++y) {
    for (int32 x = -width/2; x < width/2; ++x) {
      FVector2D CellPosition = GridPosition + FVector2D(x, y);
      if (!IsCellValid(CellPosition.X, CellPosition.Y)) {
        return false;
      }
      Index = GetGridCellIndex(CellPosition.X, CellPosition.Y);
      if (GridCells[Index]->IsOccupied()) {
        if (Item == nullptr || GridCells[Index]->OccupyingItem != Item) {
          return false;
        }
      }
    }
  }
  return true;
}

bool AGrid::CanPlaceInCell(const FVector2D &ItemSize, const UGridCell *Cell) const {
  if (!Cell) {
    return false;
  }
  return CheckIfCellsAreFree(Cell->GridPosition, ItemSize, nullptr);
}

bool AGrid::CanPlaceAtGridPosition(const FVector2D &ItemSize, const FVector2D &GridPosition) const {
  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceAtWorldPosition(const FVector2D &ItemSize, const FVector &WorldPosition) const {
  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceItemInCell_Implementation(const AActor* Item, const UGridCell* GridCell) const {
  if (!Item) return false;
  if (!GridCell) return false;
  if (!IsPlaceableItem(Item)) return false;
  auto GridComponent = GetGridComponent(Item);
  return CanPlaceInCell(GridComponent->Size, GridCell);
}

bool AGrid::CanPlaceItemAtGridPosition(const AActor* Item, const FVector2D& GridPosition) const {
  if (!Item) return false;
  if (!IsPlaceableItem(Item)) return false;
  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

bool AGrid::CanPlaceItemAtWorldPosition(const AActor* Item, const FVector& WorldPosition) const {
  if (!Item) return false;
  if (!IsPlaceableItem(Item)) return false;
  UGridCell *Cell = GetCellAtWorldPosition(WorldPosition);
  if (!Cell) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

// Check if an item can be placed
bool AGrid::CanPlaceItem(const AActor* Item) const {
  if (!Item) return false;
  if (!IsPlaceableItem(Item)) return false;
  // get the world position of the item
  FVector WorldPosition = Item->GetActorLocation();
  return CanPlaceItemAtWorldPosition(Item, WorldPosition);
}

///////// Placement Checks /////////

TArray<UGridCell*> AGrid::GetCells(const FVector2D& GridPosition, const FVector2D& GridSize) const {
  TArray<UGridCell*> Cells;
  for (int32 y = -GridSize.Y/2; y < GridSize.Y/2; ++y) {
    for (int32 x = -GridSize.X/2; x < GridSize.X/2; ++x) {
      FVector2D CellPosition = GridPosition + FVector2D(x, y);
      UGridCell *Cell = GetGridCellAtGridPosition(CellPosition);
      if (Cell) {
        Cells.Add(Cell);
      }
    }
  }

  return Cells;
}

///////// PLACEMENT /////////

// Place an item on the grid
bool AGrid::PlaceItem(AActor* Item) {
  if (!Item) return false;
  if (!IsPlaceableItem(Item)) return false;
  auto GridComponent = GetGridComponent(Item);

  // get the world position of the item
  FVector WorldPosition = Item->GetActorLocation();

  // get the grid position of the item
  FVector2D GridPosition = WorldToGrid(WorldPosition);

  // get the cell at the grid position for the item's origin cell
  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return PlaceItemInCell(Item, Cell);
}

bool AGrid::PlaceItemInCell(AActor* Item, UGridCell* GridCell) {
  if (!Item) return false;
  if (!GridCell) return false;
  if (!IsPlaceableItem(Item)) return false;
  auto GridComponent = GetGridComponent(Item);

  if (!CanPlaceItemInCell(Item, GridCell)) {
    UE_LOG(LogTemp, Warning, TEXT("Cannot place item: %s"), *Item->GetName());
    return false;
  }

  // Update the Item to the new position, which will update the occupied cells
  GridComponent->PlaceInGrid(this, GridCell->GridPosition, GridComponent->Rotation);

  UE_LOG(LogTemp, Log, TEXT("Item placed: %s"), *Item->GetName());

  // add the item to the grid
  ManagedItems.Add(Item);

  return true;
}

bool AGrid::PlaceItemAtGridPosition(AActor* Item, const FVector2D& GridPosition) {
  if (!Item) return false;

  UGridCell *Cell = GetCellAtGridPosition(GridPosition);
  if (!Cell) {
    return false;
  }

  return PlaceItemInCell(Item, Cell);
}

///////// REMOVAL /////////

// Remove an item from the grid
bool AGrid::RemoveItem(AActor* Item)
{
  if (!Item) return false;
  if (!IsPlaceableItem(Item)) return false;
  auto GridComponent = GetGridComponent(Item);

  TArray<UGridCell*> CellsToFree = GridComponent->OccupiedCells;
  for (UGridCell* Cell : CellsToFree) {
    if (!Cell) {
      continue;
    }
    if (!Cell->IsOccupied()) {
      continue;
    }
    Cell->SetOccupyingItem(nullptr);
  }

  UE_LOG(LogTemp, Log, TEXT("Item removed: %s"), *Item->GetName());

  // remove the item from the grid
  ManagedItems.Remove(Item);

  return true;
}

///////// MODIFICATION /////////

bool AGrid::CanRotateItem(AActor* Item, float NewRotation) {
  if (!Item) return false;
  if (!ManagedItems.Contains(Item)) return false;
  auto GridComponent = GetGridComponent(Item);
  if (!GridComponent) return false;
  if (GridComponent->Rotation == NewRotation) return false;
  // get the rotated size of the item
  FVector2D RotatedSize = GridComponent->GetSizeAtRotation(NewRotation);
  // now get the position of the item
  FVector2D GridPosition = GridComponent->Position;
  // now check to see if we can place the item
  return CheckIfCellsAreFree(GridPosition, RotatedSize, Item);
}

// Rotate an item
bool AGrid::RotateItem(AActor* Item, float NewRotation) {
  if (!Item) return false;
  if (!CanRotateItem(Item, NewRotation)) return false;
  auto GridComponent = GetGridComponent(Item);
  GridComponent->Update(GridComponent->Position, NewRotation);
  return true;
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
    // Set the color to blue by default
    auto Color = FColor::Blue;
    // Set the color to green if the cell is occupied
    if (Cell->IsOccupied()) {
      Color = FColor::Green;
    }
    DrawCell(Cell, Color, -1.0f);
  }
}

// Debug: Draw an item
void AGrid::DebugDrawItem(const AActor* Item) const {
  UWorld* World = GetWorld();
  if (!World) return;

  if (!Item) return;
  if (!ManagedItems.Contains(Item)) return;
  auto GridComponent = GetGridComponent(Item);
  if (!GridComponent) return;

  TArray<UGridCell*> Cells = GridComponent->OccupiedCells;
  for (UGridCell* Cell : Cells) {
    DrawCell(Cell, FColor::Green, -1.0f);
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
