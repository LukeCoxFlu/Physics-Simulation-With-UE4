// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsMasterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YEE4_API UPhysicsMasterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicsMasterComponent();

	// Item Properties
	FVector velocity = FVector(0.0f, 0.0f, 0.0f);


	// Solver Properties (Always changing as I make it more officiant 

	FVector constantForce = FVector(0.0f, 0.0f, 0.0f);
	FVector previousPos = FVector(0.0f, 0.0f, 0.0f);

	// Changable
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	FVector spawnedVelocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	float accelerationDueToGravity = -9.81f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
