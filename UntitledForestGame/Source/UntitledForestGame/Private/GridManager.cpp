#include "GridManager.h"
#include "UfgGameplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Grid.h"

// Constructor
AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGridManager::AddGrid(AGrid* Grid)
{
    if (Grid && !Grids.Contains(Grid))
    {
        Grids.Add(Grid);
    }
}

void AGridManager::RemoveGrid(AGrid* Grid)
{
    if (Grid && Grids.Contains(Grid))
    {
        Grids.Remove(Grid);
    }
}
