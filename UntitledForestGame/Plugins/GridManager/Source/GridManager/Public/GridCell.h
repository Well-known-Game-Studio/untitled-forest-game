#pragma once

#include "CoreMinimal.h"
#include "GridCell.generated.h"

// Enum for grid cell types
UENUM(BlueprintType)
enum class EGridCellType : uint8
{
    Empty,
    Ground,
    Unusable,
};

// Class for grid cell atributes (can't be struct since we want pointers to it
// and subclassing in blueprint)
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GRIDMANAGER_API UGridCellAttributes : public UObject
{
    GENERATED_BODY()

public:

    // Allows spawning of objects and things in BP as long as we can get
    // reference to world
    virtual UWorld* GetWorld() const override;

    UPROPERTY(Transient)
    UWorld* World;

    // Function to allow blueprint to mark this object for deletion
    UFUNCTION(BlueprintCallable)
    void MarkForDeletion();

    // override in blueprint or subclass
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnDebugDraw(const FVector& CellWorldPosition);

    // Fill out in blueprint or subclass
};

// Struct for grid cell
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GRIDMANAGER_API UGridCell : public UObject
{
    GENERATED_BODY()

public:

    // Constructor
    UGridCell();

    // Allows spawning of objects and things in BP as long as we can get
    // reference to world
    virtual UWorld* GetWorld() const override;

    UPROPERTY(Transient)
    UWorld* World;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D GridPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGridCellType CellType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UGridCellAttributes* Attributes = nullptr;

    UFUNCTION(BlueprintCallable)
    bool IsEmpty() const;

    UFUNCTION(BlueprintCallable)
    bool IsOccupied() const;

    UFUNCTION(BlueprintCallable)
    void SetOccupyingItem(AActor* Item);

    // Function to allow blueprint to mark this object for deletion
    UFUNCTION(BlueprintCallable)
    void MarkForDeletion();

    // override in blueprint or subclass
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnDebugDraw();

protected:

    friend class AGrid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* OccupyingItem;
};
