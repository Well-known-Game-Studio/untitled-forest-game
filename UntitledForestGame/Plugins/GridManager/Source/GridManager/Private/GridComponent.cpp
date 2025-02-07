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

TArray<UGridCell*> UGridComponent::GetNeighborCells(bool IncludeOccupied) const {
  TArray<UGridCell*> AdjacentCells;
  if (Grid == nullptr) {
    return AdjacentCells;
  }
  // // build up a list of FVector2D's that represent the neighboring cells around
  // // the object
  TArray<FVector2D> NeighborCoords;
  // use the center + the size of the object to compute the neighboring cells
  FVector2D RotatedSize = GetRotatedSize();
  FVector2D PermiterSize = RotatedSize + FVector2D(2.0f, 2.0f);
  FVector2D TopLeft = Position - FVector2D(1.0f, 1.0f);
  FVector2D BottomRight = TopLeft + PermiterSize;

  // log the center, size, half size
  UE_LOG(LogTemp, Warning, TEXT("PerimeterSize: (%f, %f)"), PermiterSize.X, PermiterSize.Y);
  UE_LOG(LogTemp, Warning, TEXT("TopLeft: (%f, %f)"), TopLeft.X, TopLeft.Y);
  UE_LOG(LogTemp, Warning, TEXT("BottomRight: (%f, %f)"), BottomRight.X, BottomRight.Y);

  // Get coordinates for the cells around the perimeter of the object
  for (int32 x = TopLeft.X; x < BottomRight.X; x++) {
    for (int32 y = TopLeft.Y; y < BottomRight.Y; y++) {
      // top edge or bottom edge, add all the cells in this row
      if (y == TopLeft.Y || y == (BottomRight.Y-1)) {
        NeighborCoords.Add(FVector2D(x, y));
      } else {
        // left edge or right edge, add the cells on the left and right
        if (x == TopLeft.X || x == (BottomRight.X-1)) {
          NeighborCoords.Add(FVector2D(x, y));
        }
      }
    }
  }

  // print out the neighbor coords
  // UE_LOG(LogTemp, Warning, TEXT("NeighborCoords:"));
  // for (const FVector2D &Coord : NeighborCoords) {
  //   UE_LOG(LogTemp, Warning, TEXT("  (%d, %d)"), (int32)Coord.X, (int32)Coord.Y);
  // }

  for (FVector2D Coord : NeighborCoords) {
    UGridCell* Cell = Grid->GetCellAtGridPosition(Coord);
    if (Cell == nullptr) {
      continue;
    }
    // Only add the cell if it is empty or we want to include occupied cells.
    // Only add the cell if it is not already in the list.
    if (IncludeOccupied || Cell->IsEmpty()) {
      if (!AdjacentCells.Contains(Cell)) {
        AdjacentCells.Add(Cell);
      }
    }
  }

  // // Less efficient, but more general way to get the adjacent cells
  // for (UGridCell* Cell : OccupiedCells) {
  //   if (Cell == nullptr) {
  //     continue;
  //   }
  //   TArray<UGridCell*> Neighbors = Grid->GetNeighborCells(Cell, IncludeOccupied);
  //   for (UGridCell* Neighbor : Neighbors) {
  //     if (Neighbor == nullptr) {
  //       continue;
  //     }
  //     // Only add the cell if it is empty or we want to include occupied cells.
  //     // Only add the cell if it is not already in the list.
  //     if (IncludeOccupied || Neighbor->IsEmpty()) {
  //       if (!AdjacentCells.Contains(Neighbor)) {
  //         AdjacentCells.Add(Neighbor);
  //       }
  //     }
  //   }
  // }
  return AdjacentCells;
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

void UGridComponent::Update(FVector2D NewPosition, float NewRotation) {
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

bool UGridComponent::PlaceInGrid(AGrid* NewGrid, FVector2D NewPosition, float NewRotation) {
  if (NewGrid == nullptr) {
    return false;
  }
  SetGrid(NewGrid);
  Update(NewPosition, NewRotation);
  // broadcast that the item has been placed
  OnPlacedInGrid.Broadcast();
  return true;
}

TArray<UGridCell*> UGridComponent::GetTargetCells(FVector2D NewPosition, float NewRotation) const {
  if (Grid == nullptr) {
    return TArray<UGridCell*>();
  }
  // change the size that we use for occupying cells based on the rotation
  FVector2D RotatedSize = GetSizeAtRotation(NewRotation);
  // get the new cells
  return Grid->GetCells(NewPosition, RotatedSize);
}

FTransform UGridComponent::GetTargetWorldTransform(FVector2D NewPosition, float NewRotation) const {
  // get the target cells
  TArray<UGridCell*> TargetCells = GetTargetCells(NewPosition, NewRotation);
  // return the transform
  return MakeTransform(NewRotation, TargetCells);
}

FTransform UGridComponent::GetWorldTransform() const
{
  return MakeTransform(Rotation, OccupiedCells);
}

FTransform UGridComponent::MakeTransform(float AtRotation, const TArray<UGridCell*> &Cells) const {
  if (Grid == nullptr) {
    return FTransform();
  }
  if (Cells.Num() == 0) {
    return FTransform();
  }
  UGridCell* Cell = Cells[0];
  if (Cell == nullptr) {
    return FTransform();
  }
  FVector Location = Cell->GetWorldPosition();
  // if the width > 1 or the height > 1, then the position is the center of the
  // cells, which is just midway between the first and last cells
  if (Cells.Num() > 1) {
    UGridCell* LastCell = Cells[Cells.Num() - 1];
    if (LastCell != nullptr) {
      Location = (Location + LastCell->GetWorldPosition()) / 2.0f;
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
  FRotator RotationRotator = FQuat(WorldZ.GetSafeNormal(), FMath::DegreesToRadians(AtRotation)).Rotator();
  // combine the two rotators (order is important here!)
  Rotator = FRotator(FQuat(Rotator)*FQuat(RotationRotator));

  // we don't want to change any scaling that may have been applied, so we get
  // our owner's scale and apply it
  FVector Scale = GetOwner()->GetActorScale3D();

  return FTransform(Rotator, Location, Scale);
}
