#include "GridComponent.h"
#include "Grid.h"
#include "GridCell.h"

UGridComponent::UGridComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UGridComponent::BeginPlay() {
  Super::BeginPlay();
}

void UGridComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  Super::EndPlay(EndPlayReason);
}

void UGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGridComponent::SetGrid(AGrid* NewGrid) {
  if (Grid == NewGrid) {
    return;
  }
  if (Grid != nullptr) {
    Grid->RemoveItem(GetOwner());
  }
  // set the new grid
  Grid = NewGrid;
  // broadcast that the grid has changed
  OnGridChanged.Broadcast(Grid);
}

TArray<UGridCell*> UGridComponent::GetOccupiedCells() const {
  return OccupiedCells;
}

FVector2D UGridComponent::GetSize() const {
  return Size;
}

FVector2D UGridComponent::GetRotatedSize() const {
  FVector2D RotatedSize = Size;
  if (Rotation == 90.0f || Rotation == 270.0f) {
    RotatedSize = FVector2D(Size.Y, Size.X);
  }
  return RotatedSize;
}

FVector2D UGridComponent::GetSizeAtRotation(float NewRotation) const {
  FVector2D RotatedSize = Size;
  if (NewRotation == 90.0f || NewRotation == 270.0f) {
    RotatedSize = FVector2D(Size.Y, Size.X);
  }
  return RotatedSize;
}

FVector2D UGridComponent::GetPosition() const {
  return Position;
}

void UGridComponent::Update(FVector2D& NewPosition, float NewRotation) {
  if (Grid == nullptr) {
    return;
  }
  // set the new position and rotation
  Position = NewPosition;
  Rotation = NewRotation;
  // clear the occupied cells
  for (UGridCell* Cell : OccupiedCells) {
    if (Cell != nullptr) {
      Cell->SetOccupyingItem(nullptr);
    }
  }
  // Clear out the old cells
  OccupiedCells.Empty();
  // change the size that we use for occupying cells based on the rotation
  FVector2D RotatedSize = GetRotatedSize();
  // get the new cells
  OccupiedCells = Grid->GetCells(NewPosition, RotatedSize);
  // set the new cells' occupying item to be the owning actor of this component
  AActor* Owner = GetOwner();
  for (UGridCell* Cell : OccupiedCells) {
    if (Cell != nullptr) {
      Cell->SetOccupyingItem(Owner);
    }
  }
  // broadcast that the item has been updated
  OnGridPositionRotationChanged.Broadcast(NewPosition, NewRotation);
}

bool UGridComponent::PlaceInGrid(AGrid* NewGrid, FVector2D& GridPosition, float Rotation) {
  if (NewGrid == nullptr) {
    return false;
  }
  SetGrid(NewGrid);
  Update(GridPosition, Rotation);
  // now set the owning actor's transform to be the center of the occupied cells
  FTransform NewTransform = GetWorldTransform();
  GetOwner()->SetActorTransform(NewTransform);
  // broadcast that the item has been placed
  OnPlacedInGrid.Broadcast();
  return true;
}

FTransform UGridComponent::GetWorldTransform() const
{
  if (Grid == nullptr) {
    return FTransform();
  }
  if (OccupiedCells.Num() == 0) {
    return FTransform();
  }
  UGridCell* Cell = OccupiedCells[0];
  if (Cell == nullptr) {
    return FTransform();
  }
  FVector Location = Cell->WorldPosition;
  // if the width > 1 or the height > 1, then the position is the center of the
  // occupied cells, which is just midway between the first and last cells
  if (OccupiedCells.Num() > 1) {
    UGridCell* LastCell = OccupiedCells[OccupiedCells.Num() - 1];
    if (LastCell != nullptr) {
      Location = (Location + LastCell->WorldPosition) / 2.0f;
    }
  }
  // the rotation will be the same as the grid's
  FRotator Rotator = Grid->GetActorRotation();
  // but if we've rotated the item, then we need to add the rotation along the grid's z-axis
  Rotator.Add(0.0f, 0.0f, Rotation);
  return FTransform(Rotator, Location);
}
