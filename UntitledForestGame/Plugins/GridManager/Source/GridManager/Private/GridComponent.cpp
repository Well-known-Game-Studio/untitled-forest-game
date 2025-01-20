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
  // now set the owning actor's transform to be the center of the occupied cells
  FTransform NewTransform = GetWorldTransform();
  GetOwner()->SetActorTransform(NewTransform);
  // broadcast that the item has been updated
  OnGridPositionRotationChanged.Broadcast(NewPosition, NewRotation);
}

bool UGridComponent::RotateTo(float NewRotation) {
  if (Grid == nullptr) {
    return false;
  }
  return Grid->RotateItem(GetOwner(), NewRotation);
}

bool UGridComponent::RotateCW() {
  if (Grid == nullptr) {
    return false;
  }
  // add 90 degrees to the current rotation mod 360
  float NewRotation = FMath::Fmod(Rotation + 90.0f, 360.0f);
  return Grid->RotateItem(GetOwner(), NewRotation);
}

bool UGridComponent::RotateCCW() {
  if (Grid == nullptr) {
    return false;
  }
  // subtract 90 degrees from the current rotation mod 360
  float NewRotation = FMath::Fmod(Rotation - 90.0f, 360.0f);
  return Grid->RotateItem(GetOwner(), NewRotation);
}

bool UGridComponent::PlaceInGrid(AGrid* NewGrid, FVector2D& GridPosition, float Rotation) {
  if (NewGrid == nullptr) {
    return false;
  }
  SetGrid(NewGrid);
  Update(GridPosition, Rotation);
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
  // the rotation will be the same as the grid's, but we need to add the item's
  // rotation to it

  // NOTE: we basically copy the code from Kismet math library:
  //  - /Users/Shared/Epic Games/UE_5.5/Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h
  //    - RotatorFromAxisAndAngle
  //    - ComposeRotators

  // Grid rotation
  FRotator Rotator = Grid->GetActorRotation();
  // Item is rotated along its z-axis
  FVector WorldZ = FVector(0.0f, 0.0f, 1.0f);
  // make a rotator from the up vector and the rotation angle
  FRotator RotationRotator = FQuat(WorldZ.GetSafeNormal(), FMath::DegreesToRadians(Rotation)).Rotator();
  // combine the two rotators (order is important here!)
  Rotator = FRotator(FQuat(Rotator)*FQuat(RotationRotator));

  // we don't want to change any scaling that may have been applied, so we get
  // our owner's scale and apply it
  FVector Scale = GetOwner()->GetActorScale3D();

  return FTransform(Rotator, Location, Scale);
}
