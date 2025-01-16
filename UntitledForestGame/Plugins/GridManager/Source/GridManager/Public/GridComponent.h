// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridInterface.h"
#include "GridComponent.generated.h"

// Forward declarations
class AGrid;
class UGridCell;

// Blueprints will bind to this to update the UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlacedInGrid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemovedFromGrid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridChanged, AGrid*, NewGrid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGridPositionRotationChanged, FVector2D, NewPosition, float, NewRotation);

// This is the grid component class which an actor must have to be able to be
// placed on the grid. It is responsible for handling the interaction with the
// grid and the grid manager. The size of the grid is determined by the Grid
// itself, but this component allows the developer to specify for the parent
// object what the size of the object is in grid squares (2d).
//
// It is used to maintain the relationship of the object to the grid and
// therefore has the following members:
// - Grid: A pointer to the grid that the object is placed on. This is set by
//         the grid manager when the object is placed on the grid. If the object
//         is not on the grid, this will be nullptr.
// - Size: The size of the object in grid squares.
// - Position: The position of the object on the grid. This is the position of
//             the bottom left corner of the object in grid squares. This is set
//             by the grid manager when the object is placed on the grid. If the
//             object is not on the grid, this will be FVector2D::ZeroVector.
// - Rotation: The rotation of the object on the grid. This is the rotation of
//             the object in degrees and is relative to the grid's rotation.
// - OccupiedCells: A list of the grid cells that the object occupies. If the
//                  object is larger than one grid square, it will occupy
//                  multiple grid cells. If the object is not on the grid, this
//                  list will be empty.
// - WorldLocation: The world location of the object. This is set by the grid
//                 manager when the object is placed on the grid. If the object
//                 is not on the grid, this will be FVector::ZeroVector. This is
//                 the location that the actor should be placed in the world and
//                 may not be at the center of a grid square, if the object is
//                 is larger than one grid square and the width or height of the
//                 object is even.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDMANAGER_API UGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    // Pointer to the grid that the object is placed on.
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    AGrid* Grid{nullptr};

    // The size of the object in grid squares.
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Grid")
    FVector2D Size{1.0f, 1.0f};

    // The position of the object on the grid.
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    FVector2D Position{FVector2D::ZeroVector};

    // The rotation of the object on the grid.
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    float Rotation{0.0f};

    // Sets default values for this actor's properties
    UGridComponent();

protected:

    // Called on the actor's construction
    virtual void BeginPlay() override;

    // Called when the actor is being removed from a level
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Function to set the grid that the object is placed on.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void SetGrid(AGrid* NewGrid);

    // Function to get the occupied cells of the object.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    TArray<UGridCell*> GetOccupiedCells() const;

    // Function to get the size of the object. Note that this is the size of the
    // object in grid squares and not the size of the object in the world. Also,
    // the size of the object is not rotated.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetSize() const;

    // Function to get the size of the object after it has been rotated. Note
    // that this is the size of the object in grid squares and not the size of
    // the object in the world. The size of the object is rotated by the
    // rotation of the object on the grid.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetRotatedSize() const;

    // Function to get the size of the object after it has been rotated to the
    // specified rotation. Note that this is the size of the object in grid
    // squares and not the size of the object in the world.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetSizeAtRotation(float NewRotation) const;

    // Function to get the position of the object on the grid. Note that this is
    // the position of the bottom left corner of the object in grid squares and
    // not the position of the object in the world.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector2D GetPosition() const;

    // Function To update the position and rotation of the object on the grid.
    void Update(FVector2D &NewPosition, float NewRotation);

    // Function to place the object on the grid at the specified position and rotation.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool PlaceInGrid(AGrid* NewGrid, FVector2D& NewPosition, float NewRotation);

    // Function to get the transform of the object in the world.
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FTransform GetWorldTransform() const;

    // Broadcast events

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnPlacedInGrid OnPlacedInGrid;

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnRemovedFromGrid OnRemovedFromGrid;

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnGridChanged OnGridChanged;

    UPROPERTY(BlueprintAssignable, Category = "Grid")
    FOnGridPositionRotationChanged OnGridPositionRotationChanged;

 protected:

    // The list of the grid cells that the object occupies.
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TArray<UGridCell*> OccupiedCells;

    // declare that AGrid is a friend class
    friend class AGrid;

    // Function to update the occupied cells
    void UpdateOccupiedCells();
};
