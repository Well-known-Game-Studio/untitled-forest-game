#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "GridManager.generated.h"

class AGrid;

UCLASS()
class UNTITLEDFORESTGAME_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AGridManager();

    // Grids that are managed by this manager
    UPROPERTY(BlueprintReadOnly)
    TArray<AGrid*> Grids;

    UFUNCTION(BlueprintCallable)
    void AddGrid(AGrid* Grid);

    UFUNCTION(BlueprintCallable)
    void RemoveGrid(AGrid* Grid);
};
