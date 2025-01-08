#include "GridCell.h"

void UGridCellAttributes::MarkForDeletion() {
  ConditionalBeginDestroy();
}
