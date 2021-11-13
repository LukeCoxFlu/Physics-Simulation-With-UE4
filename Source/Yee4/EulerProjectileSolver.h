// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EulerProjectileSolver.generated.h"

UCLASS()
class YEE4_API AEulerProjectileSolver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEulerProjectileSolver();

	FVector previousPos = FVector(0.0f, 0.0f, 0.0f);
	float radius = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	FVector velocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	float accelerationDueToGravity = -9.81f;

	FVector constantForce = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sphere To Sphere Collision")
	AActor* ImmobileSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sphere To Sphere Collision")
	float ImmobileSphereRadius = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sphere To Sphere Collision")
		float radiusasidhaouisdhf = 0;

	FVector ImmobileSphereLocation = FVector(0.0f, 0.0f, 0.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
