#include "GridItem.h"
#include "Grid.h"
#include "UfgGameplayFunctionLibrary.h"

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
      int32 CellIndex = UUfgGameplayFunctionLibrary::GetGridCellIndex(OriginCell.X + i, OriginCell.Y + j, Grid);
      OccupiedCells.Add(CellIndex);
    }
  }
}
