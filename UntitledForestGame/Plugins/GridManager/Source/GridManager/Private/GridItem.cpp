#include "GridItem.h"
#include "Grid.h"

EGridCellType GetGridCellTypeFromString(const FString& CellTypeString)
{
    if (CellTypeString == "Empty")
    {
        return EGridCellType::Empty;
    }
    else if (CellTypeString == "Ground")
    {
        return EGridCellType::Ground;
    }
    else if (CellTypeString == "Unusable")
    {
        return EGridCellType::Unusable;
    }
    else
    {
        return EGridCellType::Empty;
    }
}

EGroundType GetGroundTypeFromString(const FString& GroundTypeString)
{
    if (GroundTypeString == "Grass")
    {
        return EGroundType::Grass;
    }
    else if (GroundTypeString == "Dirt")
    {
        return EGroundType::Dirt;
    }
    else if (GroundTypeString == "Sand")
    {
        return EGroundType::Sand;
    }
    else if (GroundTypeString == "Stone")
    {
        return EGroundType::Stone;
    }
    else if (GroundTypeString == "Water")
    {
        return EGroundType::Water;
    }
    else if (GroundTypeString == "Snow")
    {
        return EGroundType::Snow;
    }
    else if (GroundTypeString == "Ice")
    {
        return EGroundType::Ice;
    }
    else if (GroundTypeString == "Mud")
    {
        return EGroundType::Mud;
    }
    else if (GroundTypeString == "Swamp")
    {
        return EGroundType::Swamp;
    }
    else
    {
        return EGroundType::Empty;
    }
}

void UGridItem::Initialize(AGrid *NewGrid, const FString &Name, const FVector2D &Origin, const FVector &Size)
{
  Grid = NewGrid;
  ItemName = Name;
  ItemSize = Size;
  OriginCell = Origin;
  GridSize = Grid->ItemSizeToGridSize(ItemSize);
  UpdateOccupiedCells();
}

// Spawn the actor
void UGridItem::SpawnActor(TSubclassOf<AActor> ActorClass) {
  if (Grid == nullptr)
  {
    return;
  }
  if (ActorClass == nullptr)
  {
    return;
  }
  if (Actor != nullptr)
  {
    Actor->Destroy();
  }
  FActorSpawnParameters SpawnParams;
  SpawnParams.Owner = Grid;
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  Actor = Grid->GetWorld()->SpawnActor<AActor>(ActorClass, GetActorTransform(), SpawnParams);
}

// Destroy the actor
void UGridItem::DestroyActor() {
  if (Actor != nullptr)
  {
    Actor->Destroy();
    Actor = nullptr;
  }
}

// Set the Actor pointer
void UGridItem::SetActor(AActor* NewActor) {
  Actor = NewActor;
}

// Set the Grid pointer
void UGridItem::SetGrid(AGrid* NewGrid) {
  Grid = NewGrid;
}

// Get the actor's transform
FTransform UGridItem::GetActorTransform() const {
  if (Actor == nullptr)
  {
    return FTransform();
  }
  return Actor->GetActorTransform();
}

// Get the actor's location
FVector UGridItem::GetActorLocation() const {
  if (Actor == nullptr)
  {
    return FVector();
  }
  return Actor->GetActorLocation();
}

// Get the actor's rotation
FRotator UGridItem::GetActorRotation() const {
  if (Actor == nullptr)
  {
    return FRotator();
  }
  return Actor->GetActorRotation();
}

FTransform UGridItem::GetSpawnTransform() const {
  if (Grid == nullptr) {
    return FTransform();
  }
  // if width & height == 1, then the position is simply the same as the origin
  // cell
  FGridCell Cell;
  if (!Grid->GetCellAtGridPosition(OriginCell, Cell)) {
    return FTransform();
  }
  FVector Location = Cell.WorldPosition;
  // but if width & height > 1, then the position is the center of the item,
  // which is the middle between the first and last occupied cells
  if (GridSize.X > 1 || GridSize.Y > 1) {
    int32 FirstCellIndex = OccupiedCells[0];
    int32 LastCellIndex = OccupiedCells[OccupiedCells.Num() - 1];
    FGridCell* FirstCell = Grid->GetGridCellAtIndex(FirstCellIndex);
    FGridCell* LastCell = Grid->GetGridCellAtIndex(LastCellIndex);
    if (FirstCell == nullptr || LastCell == nullptr) {
      return FTransform();
    }
    FVector FirstCellLocation = LastCell->WorldPosition;
    FVector LastCellLocation = FirstCell->WorldPosition;
    Location = (FirstCellLocation + LastCellLocation) / 2.0f;
  }
  // the rotation will be the same as the grid's
  FRotator Rotator = Grid->GetActorRotation();
  // but if we've rotated the item, then we need to add the rotation
  Rotator.Add(0.0f, 0.0f, Rotation);
  return FTransform(Rotator, Location);
}

// Set the actor's location
void UGridItem::SetActorLocation(const FVector &NewLocation) {
  if (Actor == nullptr)
  {
    return;
  }
  Actor->SetActorLocation(NewLocation);
}

// Set the actor's rotation
void UGridItem::SetActorRotation(const FRotator &NewRotation) {
  if (Actor == nullptr)
  {
    return;
  }
  Actor->SetActorRotation(NewRotation);
}


void UGridItem::Update(const FVector2D &NewOrigin, float NewRotation)
{
  OriginCell = NewOrigin;
  Rotation = NewRotation;
  UpdateOccupiedCells();
}

void UGridItem::UpdateOccupiedCells()
{
  OccupiedCells.Empty();
  if (Grid == nullptr)
  {
    return;
  }
  int32 width = GridSize.X;
  int32 height = GridSize.Y;
  // swap width and height if the item is rotated 90 or 270 degrees
  if (FMath::IsNearlyEqual(FMath::Fmod(Rotation, 180.0f), 90.0f, 0.01f))
  {
    std::swap(width, height);
  }
  OccupiedCells.Reserve(width * height);
  OccupiedCells.Add(Grid->GetGridCellIndex(OriginCell.X, OriginCell.Y));
  for (int32 i = -width / 2; i < width / 2; i++)
  {
    for (int32 j = -height / 2; j < height / 2; j++)
    {
      if (i == 0 && j == 0)
      {
        continue;
      }
      int32 CellIndex = Grid->GetGridCellIndex(OriginCell.X + i, OriginCell.Y + j);
      OccupiedCells.Add(CellIndex);
    }
  }
}
