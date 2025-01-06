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

void UGridItem::Initialize(AGrid *NewGrid, const FString &Name, const FVector2D &Origin, int32 InWidth, int32 InHeight)
{
  Grid = NewGrid;
  ItemName = Name;
  OriginCell = Origin;
  Width = InWidth;
  Height = InHeight;
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
  // TODO: Update the origin cell
  // Update the occupied cells
  UpdateOccupiedCells();
}

// Get the actor's transform
FTransform UGridItem::GetActorTransform() const {
  if (Actor == nullptr)
  {
    return FTransform();
  }
  FVector Location = Actor->GetActorLocation();
  FRotator Rotator(0.0f, 0.0f, Rotation);
  return FTransform(Rotator, Location);
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
  int32 width = Width;
  int32 height = Height;
  // swap width and height if the item is rotated 90 or 270 degrees
  if (FMath::IsNearlyEqual(FMath::Fmod(Rotation, 180.0f), 90.0f, 0.01f))
  {
    width = Height;
    height = Width;
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
