#include "UfgGameplayFunctionLibrary.h"
#include "Grid.h"
#include "GridManager.h"
#include "AssetRegistry/AssetRegistryModule.h"

FGridCellAttributes UUfgGameplayFunctionLibrary::RandomizeGridCellAttributes(EGroundType GroundType) {
  FGridCellAttributes Attributes;
  Attributes.GroundType = GroundType;

  float min_soil_quality = 0.0f;
  float max_soil_quality = 1.0f;
  float min_water_level = 0.0f;
  float max_water_level = 1.0f;

  // update the min / max values based on the ground type
  switch (Attributes.GroundType) {
    case EGroundType::Empty:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.0f;
      min_water_level = 0.0f;
      max_water_level = 0.0f;
      break;
    case EGroundType::Grass:
      min_soil_quality = 0.5f;
      max_soil_quality = 1.0f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    case EGroundType::Dirt:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.0f;
      max_water_level = 0.5f;
      break;
    case EGroundType::Sand:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.0f;
      max_water_level = 0.5f;
      break;
    case EGroundType::Stone:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.0f;
      max_water_level = 0.5f;
      break;
    case EGroundType::Water:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    case EGroundType::Snow:
      min_soil_quality = 0.5f;
      max_soil_quality = 1.0f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    case EGroundType::Ice:
      min_soil_quality = 0.5f;
      max_soil_quality = 1.0f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    case EGroundType::Mud:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    case EGroundType::Swamp:
      min_soil_quality = 0.0f;
      max_soil_quality = 0.5f;
      min_water_level = 0.5f;
      max_water_level = 1.0f;
      break;
    default:
      break;
  }

  // Randomize the grid cell attributes
  Attributes.SoilQuality = FMath::RandRange(min_soil_quality, max_soil_quality);
  Attributes.WaterLevel = FMath::RandRange(min_water_level, max_water_level);

  return Attributes;
}

int32 UUfgGameplayFunctionLibrary::GetGridCellIndex(int32 X, int32 Y, const AGrid* Grid) {
  return GetGridCellIndexForGridWidth(X, Y, Grid->GridWidth);
}

int32 UUfgGameplayFunctionLibrary::GetGridCellIndexForGridWidth(int32 X, int32 Y, int32 GridWidth) {
  return Y * GridWidth + X;
}

void UUfgGameplayFunctionLibrary::DrawHitPointAndBounds(AActor* HitActor, const FHitResult& Hit)
{
  if (!HitActor) {
    UE_LOG(LogTemp, Warning, TEXT("Cannot draw hit point and bounds, invalid actor pointer!"));
    return;
  }
  FVector HitActorLocation;
  FVector HitBoxExtent;
  HitActor->GetActorBounds(false, HitActorLocation, HitBoxExtent, false);
  // draw a box around what was hit
  DrawDebugBox(HitActor->GetWorld(), HitActorLocation, HitBoxExtent,
               HitActor->GetActorRotation().Quaternion(), FColor::Green, false, 2.0f, 0, 2.0f);
  // draw a point for the hit location itself
  DrawDebugPoint(HitActor->GetWorld(), Hit.ImpactPoint, 10, FColor::Red, false, 2.0f, 100);
}

FText UUfgGameplayFunctionLibrary::GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
  FNumberFormattingOptions NumberFormat;
  NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
  NumberFormat.MaximumIntegralDigits = 10000;
  NumberFormat.MinimumFractionalDigits = Precision;
  NumberFormat.MaximumFractionalDigits = Precision;
  return FText::AsNumber(TheFloat, &NumberFormat);
}

FString UUfgGameplayFunctionLibrary::GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
  return GetFloatAsTextWithPrecision(TheFloat, Precision, IncludeLeadingZero).ToString();
}

FText UUfgGameplayFunctionLibrary::GetIntAsText(int TheNumber)
{
  FNumberFormattingOptions NumberFormat;
  NumberFormat.MinimumIntegralDigits = 1;
  NumberFormat.MaximumIntegralDigits = 10000;
  NumberFormat.MinimumFractionalDigits = 0;
  NumberFormat.MaximumFractionalDigits = 0;
  return FText::AsNumber(TheNumber, &NumberFormat);
}

FString UUfgGameplayFunctionLibrary::GetIntAsString(int TheNumber)
{
  return GetIntAsText(TheNumber).ToString();
}

void UUfgGameplayFunctionLibrary::GetAllCppSubclasses(UClass* BaseClass, TArray<UClass*>& ClassArray)
{
	FName BaseClassName = BaseClass->GetFName();
	UE_LOG(LogTemp, Log, TEXT("Getting all c++ subclasses of '%s'"), *BaseClassName.ToString());
  bool bRecursive = true;
  GetDerivedClasses(BaseClass, ClassArray, bRecursive);
}

void UUfgGameplayFunctionLibrary::GetAllBlueprintSubclasses(UClass* BaseClass, TArray<UClass*>& ClassArray)
{
	FName BaseClassName = BaseClass->GetFName();
  FTopLevelAssetPath BaseClassPath = FTopLevelAssetPath(BaseClass->GetPathName());
	UE_LOG(LogTemp, Log, TEXT("Getting all blueprint subclasses of '%s'"), *BaseClassName.ToString());

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	TArray<FAssetData> AssetData;
	// The asset registry is populated asynchronously at startup, so there's no guarantee it has finished.
	// This simple approach just runs a synchronous scan on the entire content directory.
	// Better solutions would be to specify only the path to where the relevant blueprints are,
	// or to register a callback with the asset registry to be notified of when it's finished populating.
	TArray< FString > ContentPaths;
	ContentPaths.Add(TEXT("/Game"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

  // USE FTopLevelAssetPath

	// Use the asset registry to get the set of all class names deriving from Base
	TSet< FTopLevelAssetPath > DerivedNames;
	{
		TArray< FTopLevelAssetPath > BaseNames;
		BaseNames.Add(BaseClassPath);

		TSet< FTopLevelAssetPath > Excluded;
        AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
		// AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
	}

	FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	// Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray< FAssetData > AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	// Iterate over retrieved blueprint assets
	for(auto const& Asset : AssetList) {
		// Get the the class this blueprint generates (this is stored as a full path)
        auto GeneratedClassPathPtr = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass")).AsString();
		if(!GeneratedClassPathPtr.IsEmpty()) {
			// Convert path to just the name part
			const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
			const FString ObjectClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);
      const FTopLevelAssetPath ClassPath = FTopLevelAssetPath(ClassObjectPath);

			// Check if this class is in the derived set
			if(!DerivedNames.Contains(ClassPath)) {
				continue;
			}

			UClass* Class = nullptr;
			const UBlueprint* BlueprintAsset = Cast<UBlueprint>(Asset.GetAsset());
			if (BlueprintAsset) {
				Class = BlueprintAsset->GeneratedClass;
			} else {
				UE_LOG(LogTemp, Error, TEXT("Could not cast '%s' to blueprint class"), *ObjectClassName);
			}
			if (Class) {
				UE_LOG(LogTemp, Log, TEXT("Got subclass '%s'"), *ObjectClassName);
				ClassArray.Add(Class);
			} else {
				UE_LOG(LogTemp, Error, TEXT("Invalid BP Class Data!"));
			}
		}
	}
}