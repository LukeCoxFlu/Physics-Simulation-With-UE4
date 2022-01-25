// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicMasterSolver.generated.h"

UENUM(BlueprintType)
enum class ENUM_RIGIDBODY_TYPES : uint8
{
    RT_Plane       UMETA(DisplayName="Plane"),
    RT_Sphere        UMETA(DisplayName="Sphere"),
	RT_Nothing		UMETA(DisplayName="Non"),
};

UCLASS()
class YEE4_API APhysicMasterSolver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicMasterSolver();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere , Category = "Genral")
	TArray<AActor*> ListOfPhysObjects;
	
	// Changables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	bool StaticObj = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	bool DebugOptionOn = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	ENUM_RIGIDBODY_TYPES RigidBody_Types;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	bool IsNewtonian = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	FVector spawnedVelocity = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Genral")
	float accelerationDueToGravity = -400;

	virtual void Tick(float DeltaTime) override;

	float GetRadius() const;

private:
	// UE4 genralised Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneComponent = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PhysicsObject = nullptr;

	// Solver Properties (Always changing as I make it more officiant 
	
	FVector previousPos = FVector(0.0f, 0.0f, 0.0f);

	void NewtonianMotion(float deltaTime);
	float NM_Time = 0;

	void debugValues(float DeltaTime);

protected:

	// Item Properties
	FVector velocity = FVector(0.0f, 0.0f, 0.0f);
	FVector AccelerationForce = FVector(0.0f, 0.0f, 0.0f);
	
	// List of other items
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
