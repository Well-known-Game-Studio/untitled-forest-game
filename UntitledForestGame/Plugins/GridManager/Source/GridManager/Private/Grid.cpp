#include "Grid.h"
#include "GridItem.h"
#include "DrawDebugHelpers.h"

// Initialize the grid
void AGrid::InitializeGrid() {
  GridCells.Empty();

  for (int32 y = 0; y < GridHeight; ++y) {
    for (int32 x = 0; x < GridWidth; ++x) {
      FGridCell NewCell;
      // set the cell type to be Ground
      NewCell.CellType = EGridCellType::Ground;
      NewCell.GridPosition = FVector2D(x, y);
      NewCell.WorldPosition = GridToWorld(FVector2D(x, y));
      EGroundType ground_type = EGroundType::Grass;
      NewCell.Attributes = RandomizeGridCellAttributes(ground_type);
      NewCell.bIsOccupied = false;
      NewCell.OccupyingItem = nullptr;

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

FGridCell* AGrid::GetGridCellAtXY(int32 X, int32 Y) {
  if (!IsCellValid(X, Y)) {
    return nullptr;
  }

  return &GridCells[GetGridCellIndex(X, Y)];
}

FGridCell* AGrid::GetGridCell(const FVector2D& GridPosition) {
  return GetGridCellAtXY(GridPosition.X, GridPosition.Y);
}

FGridCell* AGrid::GetGridCellAtIndex(int32 Index) {
  if (Index < 0 || Index >= GridCells.Num()) {
    return nullptr;
  }

  return &GridCells[Index];
}

bool AGrid::GetGridCellAttributes(int32 X, int32 Y, FGridCellAttributes &Attributes) const {
  if (!IsCellValid(X, Y)) {
    return false;
  }

  Attributes = GridCells[GetGridCellIndex(X, Y)].Attributes;
  return true;
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
  FVector RelativePosition = WorldPosition - GetActorLocation();

  // take into account the AGrid's rotation
  auto Rotation = GetActorRotation();
  RelativePosition = FRotator(-Rotation.Pitch, -Rotation.Yaw, -Rotation.Roll).RotateVector(RelativePosition);

  int32 x = FMath::RoundToInt(RelativePosition.X / CellSize);
  int32 y = FMath::RoundToInt(RelativePosition.Y / CellSize);

  return FVector2D(x, y);
}

//// GET ITEMS ////

UGridItem* AGrid::GetItemAtXY(int32 X, int32 Y) {
  return GetItemAtGridPosition(FVector2D(X, Y));
}

UGridItem* AGrid::GetItemAtCell(const FGridCell& Cell) {
  // Get the cell at the grid position
  return GetItemAtGridPosition(Cell.GridPosition);
}

UGridItem* AGrid::GetItemAtGridPosition(const FVector2D& GridPosition) {
  FGridCell Cell;
  if (!GetCellAtGridPosition(GridPosition, Cell)) {
    return nullptr;
  }

  return Cell.OccupyingItem;
}

// Get an item at a specific world position
UGridItem* AGrid::GetItemAtWorldPosition(const FVector& WorldPosition) {
  FGridCell Cell;
  if (!GetCellAtWorldPosition(WorldPosition, Cell)) {
    return nullptr;
  }

  return Cell.OccupyingItem;
}

//// GET CELLS ////

bool AGrid::GetCellAtGridPosition(const FVector2D& GridPosition, FGridCell& Cell) {
  int32 x = GridPosition.X;
  int32 y = GridPosition.Y;
  int32 Index = GetGridCellIndex(x, y);

  if (!IsCellValid(x,y) || Index >= GridCells.Num()) {
    return false;
  }

  Cell = GridCells[Index];

  return true;
}

// Get a cell at a specific world position
bool AGrid::GetCellAtWorldPosition(const FVector& WorldPosition, FGridCell& Cell) {
  // convert the position to be relative to the grid
  FVector2D GridPosition = WorldToGrid(WorldPosition);
  int32 x = GridPosition.X;
  int32 y = GridPosition.Y;
  int32 Index = GetGridCellIndex(x, y);

  if (!IsCellValid(x,y) || Index >= GridCells.Num()) {
    return false;
  }

  Cell = GridCells[Index];

  return true;
}

bool AGrid::GetCellInDirectionFromWorldPosition(const FVector& WorldPosition, const FVector& Direction, FGridCell& Cell) {
  auto CellPosition = WorldPosition + Direction * CellSize;
  return GetCellAtWorldPosition(CellPosition, Cell);
}

bool AGrid::GetCellInFrontOfActor(const AActor* Actor, FGridCell& Cell) {
  if (!Actor) {
    return false;
  }

  FVector ActorLocation = Actor->GetActorLocation();
  FVector ActorForwardVector = Actor->GetActorForwardVector();
  return GetCellInDirectionFromWorldPosition(ActorLocation, ActorForwardVector, Cell);
}

///////// PLACEMENT Checks /////////

bool AGrid::CanPlaceInCell(const FVector &ItemSize, const FGridCell &Cell) {
  if (!IsCellValid(Cell.GridPosition.X, Cell.GridPosition.Y)) {
    return false;
  }

  auto GridCell = GetGridCellAtXY(Cell.GridPosition.X, Cell.GridPosition.Y);

  if (!GridCell || GridCell->bIsOccupied) {
    return false;
  }

  // for the size of the item (width / height)
  FVector2D ItemGridSize = ItemSizeToGridSize(ItemSize);
  int32 width = ItemGridSize.X;
  int32 height = ItemGridSize.Y;

  for (int32 y = -height/2; y < height/2; ++y) {
    for (int32 x = -width/2; x < width/2; ++x) {
      FVector2D CellPosition = Cell.GridPosition + FVector2D(x, y);
      if (!IsCellValid(CellPosition.X, CellPosition.Y)) {
        return false;
      }
      int32 Index = GetGridCellIndex(CellPosition.X, CellPosition.Y);
      if (Index < 0 || Index >= GridCells.Num() || GridCells[Index].bIsOccupied) {
        return false;
      }
    }
  }

  return true;
}

bool AGrid::CanPlaceAtGridPosition(const FVector &ItemSize, const FVector2D &GridPosition) {
  FGridCell Cell;
  if (!GetCellAtGridPosition(GridPosition, Cell)) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceAtWorldPosition(const FVector &ItemSize, const FVector &WorldPosition) {
  FGridCell Cell;
  if (!GetCellAtWorldPosition(WorldPosition, Cell)) {
    return false;
  }

  return CanPlaceInCell(ItemSize, Cell);
}

bool AGrid::CanPlaceItemInCell(const UGridItem* Item, const FGridCell& GridCell) {
  if (!Item) return false;
  return CanPlaceInCell(Item->ItemSize, GridCell);
}

bool AGrid::CanPlaceItemAtGridPosition(const UGridItem* Item, const FVector2D& GridPosition) {
  if (!Item) return false;

  FGridCell Cell;
  if (!GetCellAtGridPosition(GridPosition, Cell)) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

bool AGrid::CanPlaceItemAtWorldPosition(const UGridItem* Item, const FVector& WorldPosition) {
  if (!Item) return false;

  FGridCell Cell;
  if (!GetCellAtWorldPosition(WorldPosition, Cell)) {
    return false;
  }

  return CanPlaceItemInCell(Item, Cell);
}

// Check if an item can be placed
bool AGrid::CanPlaceItem(const UGridItem* Item)
{
  if (!Item) return false;

  TArray<int32> CellsToCheck = Item->OccupiedCells;

  for (int32 CellIndex : CellsToCheck) {
    if (CellIndex < 0 || CellIndex >= GridCells.Num() || GridCells[CellIndex].bIsOccupied) {
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
  FGridCell Cell;
  if (!GetCellAtGridPosition(Item->OriginCell, Cell)) {
    return false;
  }

  return PlaceItemInCell(Item, Cell);
}

bool AGrid::PlaceItemInCell(UGridItem* Item, const FGridCell& GridCell) {
  if (!Item) return false;

  if (!CanPlaceItemInCell(Item, GridCell)) {
    UE_LOG(LogTemp, Warning, TEXT("Cannot place item: %s"), *Item->ItemName);
    return false;
  }

  // Update the Item to the new position, which will update the occupied cells
  Item->Update(GridCell.GridPosition, Item->Rotation);

  TArray<int32> CellsToOccupy = Item->OccupiedCells;

  for (int32 CellIndex : CellsToOccupy) {
    GridCells[CellIndex].bIsOccupied = true;
    GridCells[CellIndex].OccupyingItem = Item;
  }

  UE_LOG(LogTemp, Log, TEXT("Item placed: %s"), *Item->ItemName);

  // add the item to the grid
  ManagedItems.Add(Item);

  return true;
}

bool AGrid::PlaceItemAtGridPosition(UGridItem* Item, const FVector2D& GridPosition) {
  if (!Item) return false;

  FGridCell Cell;
  if (!GetCellAtGridPosition(GridPosition, Cell)) {
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
    GridCells[CellIndex].bIsOccupied = false;
    GridCells[CellIndex].OccupyingItem = nullptr;
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

void AGrid::DrawCell(const FGridCell& Cell, const FColor &Color, float Duration) {
  UWorld* World = GetWorld();
  if (!World) return;

  FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
  FVector Center = Cell.WorldPosition;
  // Move the center up by half the size so the box is drawn at the correct location
  // we use the actor's up vector for this to handle the grid's rotation
  Center += GetActorUpVector() * CellSize / 2;
  // ensure the box is drawn at the correct location with the correct rotation
  DrawDebugBox(World, Center, HalfSize, GetActorQuat(), Color, false, Duration);
}

// Debug: Draw the grid
void AGrid::DebugDrawGrid()
{
  UWorld* World = GetWorld();
  if (!World) return;

  for (const FGridCell& Cell : GridCells) {
    FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
    FVector Center = Cell.WorldPosition;
    // Move the center up by half the size so the box is drawn at the correct location
    // we use the actor's up vector for this to handle the grid's rotation
    Center += GetActorUpVector() * CellSize / 2;
    // Set the color to blue by default
    auto Color = FColor::Blue;
    // Set the color to green if the cell is occupied
    if (Cell.bIsOccupied) {
      Color = FColor::Green;
    }
    // ensure the box is drawn at the correct location with the correct rotation
    DrawDebugBox(World, Center, HalfSize, GetActorQuat(), Color, false, -1.0f);
  }
}

// Debug: Draw an item
void AGrid::DebugDrawItem(const UGridItem* Item)
{
  UWorld* World = GetWorld();
  if (!World) return;

  if (!Item) return;

  TArray<int32> Cells = Item->OccupiedCells;
  for (int32 CellIndex : Cells) {
    FVector HalfSize = FVector(CellSize / 2, CellSize / 2, CellSize / 2);
    FVector Center = GridCells[CellIndex].WorldPosition;
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


FGridCellAttributes AGrid::RandomizeGridCellAttributes(EGroundType GroundType) {
  FGridCellAttributes Attributes;
  Attributes.GroundType = GroundType;

  float min_soil_quality = 0.0f;
  float max_soil_quality = 1.0f;
  float min_water_level = 0.0f;
  float max_water_level = 1.0f;

  // update the min / max values based on the ground type
  switch (Attributes.GroundType) {
  case EGroundType::Empty:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.0f;
    min_water_level = 0.0f;
    max_water_level = 0.0f;
    break;
  case EGroundType::Grass:
    min_soil_quality = 0.5f;
    max_soil_quality = 1.0f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  case EGroundType::Dirt:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.0f;
    max_water_level = 0.5f;
    break;
  case EGroundType::Sand:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.0f;
    max_water_level = 0.5f;
    break;
  case EGroundType::Stone:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.0f;
    max_water_level = 0.5f;
    break;
  case EGroundType::Water:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  case EGroundType::Snow:
    min_soil_quality = 0.5f;
    max_soil_quality = 1.0f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  case EGroundType::Ice:
    min_soil_quality = 0.5f;
    max_soil_quality = 1.0f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  case EGroundType::Mud:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  case EGroundType::Swamp:
    min_soil_quality = 0.0f;
    max_soil_quality = 0.5f;
    min_water_level = 0.5f;
    max_water_level = 1.0f;
    break;
  default:
    break;
  }

  // Randomize the grid cell attributes
  Attributes.SoilQuality = FMath::RandRange(min_soil_quality, max_soil_quality);
  Attributes.WaterLevel = FMath::RandRange(min_water_level, max_water_level);

  return Attributes;
}
