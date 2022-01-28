// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "physicsWorldSettings.generated.h"


UENUM(BlueprintType)
enum class ENUM_COLLISION_TYPES : uint8
{
	CT_SPHERETOSPHERE      UMETA(DisplayName="SPHERE TO SPHERE"),
	CT_SPHERETOPLANE        UMETA(DisplayName="SPHERE TO PLANE"),
	RT_Nothing		UMETA(DisplayName="Non"),
};

UCLASS()
class YEE4_API AphysicsWorldSettings : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AphysicsWorldSettings();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Physics Solver")
	float accelerationDueToGravity = -400;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Physics Solver")
	TMap<ENUM_COLLISION_TYPES, float> mapOfCollisionTypesToEnergyLoss;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
