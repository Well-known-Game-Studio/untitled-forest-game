#pragma once

#include "CoreMinimal.h"
#include "GridCell.generated.h"

class UGridItem;

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
UCLASS(BlueprintType, Blueprintable)
class GRIDMANAGER_API UGridCellAttributes : public UObject
{
    GENERATED_BODY()

public:

    // Function to allow blueprint to mark this object for deletion
    UFUNCTION(BlueprintCallable)
    void MarkForDeletion();

    // override in blueprint or subclass
    UFUNCTION(BlueprintImplementableEvent)
    void OnDebugDraw();

    // Fill out in blueprint or subclass
};

// Struct for grid cell
USTRUCT(BlueprintType, Blueprintable)
struct GRIDMANAGER_API FGridCell
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D GridPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGridCellType CellType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UGridCellAttributes* Attributes = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UGridItem* OccupyingItem;
};
