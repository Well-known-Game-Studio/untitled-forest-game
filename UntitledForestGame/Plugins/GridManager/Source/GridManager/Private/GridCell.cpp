#include "GridCell.h"

//// GridCellAttributes

UWorld* UGridCellAttributes::GetWorld() const
{
  // if our World reference is valid, return that
  if (World) {
    return World;
  }
  // Outer is set when creating action via NewObject<T>
  // try casting to actor component
  UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
  if (Comp) {
    return Comp->GetWorld();
  }
  // if that fails, try casting to actor
  AActor* Actor = Cast<AActor>(GetOuter());
  if (Actor) {
    return Actor->GetWorld();
  }
  // if those fail, return nullptr :/
  return nullptr;
}

void UGridCellAttributes::MarkForDeletion() {
  ConditionalBeginDestroy();
}

//// GridCell

UGridCell::UGridCell() {
}

bool UGridCell::IsEmpty() const {
  return OccupyingItem == nullptr;
}

bool UGridCell::IsOccupied() const {
  return OccupyingItem != nullptr;
}

void UGridCell::SetOccupyingItem(AActor* Item) {
  OccupyingItem = Item;
}

UWorld* UGridCell::GetWorld() const
{
  // if our World reference is valid, return that
  if (World) {
    return World;
  }
  // Outer is set when creating action via NewObject<T>
  // try casting to actor component
  UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
  if (Comp) {
    return Comp->GetWorld();
  }
  // if that fails, try casting to actor
  AActor* Actor = Cast<AActor>(GetOuter());
  if (Actor) {
    return Actor->GetWorld();
  }
  // if those fail, return nullptr :/
  return nullptr;
}

void UGridCell::MarkForDeletion() {
  ConditionalBeginDestroy();
}
