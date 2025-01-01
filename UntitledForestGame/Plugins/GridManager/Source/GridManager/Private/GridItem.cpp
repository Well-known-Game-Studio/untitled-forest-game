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
  for (int32 i = -width / 2; i < width / 2; i++)
  {
    for (int32 j = -height / 2; j < height / 2; j++)
    {
      int32 CellIndex = Grid->GetGridCellIndex(OriginCell.X + i, OriginCell.Y + j);
      OccupiedCells.Add(CellIndex);
    }
  }
}
