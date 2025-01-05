// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "UfgGameplayFunctionLibrary.h"

// For Debug:
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ufg.DebugDrawInteraction"), false, TEXT("Enable DebugDrawing for Interaction Component"), ECVF_Cheat);

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
}

void UInteractionComponent::BeginPlay()
{
  Super::BeginPlay();
}

bool UInteractionComponent::IsInteractableInRange() const
{
  AActor* MyOwner = GetOwner();

  AActor *ClosestActor = nullptr;
  UActorComponent *ClosestComponent = nullptr;
  FHitResult ClosestHit;
  bool didHit = UUfgGameplayFunctionLibrary::GetClosestInteractableInRange(MyOwner,
                                                                           InteractionRange,
                                                                           InteractionBoxQueryHalfExtent,
                                                                           ClosestActor,
                                                                           ClosestComponent,
                                                                           ClosestHit);

  return didHit;
}

bool UInteractionComponent::GetInteractableInRange(AActor *&OutActor, UActorComponent *&OutComponent, FHitResult &OutHit) const
{
  AActor* MyOwner = GetOwner();
  bool didHit = UUfgGameplayFunctionLibrary::GetClosestInteractableInRange(MyOwner,
                                                                           InteractionRange,
                                                                           InteractionBoxQueryHalfExtent,
                                                                           OutActor,
                                                                           OutComponent,
                                                                           OutHit);
  return didHit;
}

void UInteractionComponent::PrimaryInteract()
{
  AActor* MyOwner = GetOwner();

  AActor *ClosestActor = nullptr;
  UActorComponent *ClosestComponent = nullptr;
  FHitResult ClosestHit;
  bool didHit = UUfgGameplayFunctionLibrary::GetClosestInteractableInRange(MyOwner,
                                                                           InteractionRange,
                                                                           InteractionBoxQueryHalfExtent,
                                                                           ClosestActor,
                                                                           ClosestComponent,
                                                                           ClosestHit);

  if (ClosestActor) {
    APawn* MyPawn = Cast<APawn>(MyOwner);
    if (ClosestComponent) {
      IInteractableInterface::Execute_Interact(ClosestComponent, MyPawn, ClosestHit);
    } else {
      IInteractableInterface::Execute_Interact(ClosestActor, MyPawn, ClosestHit);
    }

    bool bDrawDebug = CVarDebugDrawInteraction.GetValueOnGameThread();
    if (bDrawDebug) {
      UUfgGameplayFunctionLibrary::DrawHitPointAndBounds(ClosestActor, ClosestHit);
    }
  }
}
