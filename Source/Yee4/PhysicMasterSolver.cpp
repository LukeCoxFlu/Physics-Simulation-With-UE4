// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicMasterSolver.h"

// Sets default values
APhysicMasterSolver::APhysicMasterSolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	SetRootComponent(RootSceneComponent);

	PhysicsObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Physics Body"));
	
	
}

// Called when the game starts or when spawned
void APhysicMasterSolver::BeginPlay()
{
	Super::BeginPlay();
	
	AccelerationForce.Z = accelerationDueToGravity;
	previousPos = GetActorLocation();

	velocity = spawnedVelocity;

	
}

// Called every frame
void APhysicMasterSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsNewtonian)
	{
		NewtonianMotion(DeltaTime);
		return;
	}

	//Semi - Explicit Euler Motion
	const FVector PositionN0 = GetActorLocation();
	velocity = velocity + AccelerationForce * DeltaTime;
	SetActorLocation(PositionN0 + velocity * DeltaTime);
	
}

void APhysicMasterSolver::NewtonianMotion(float deltaTime)
{
	NM_Time += deltaTime;
	const float NewX = previousPos.X + (spawnedVelocity.X * NM_Time);
	const float NewY = previousPos.Y + (spawnedVelocity.Y * NM_Time);
	const float NewZ = previousPos.Z + (spawnedVelocity.Z * NM_Time) + ((accelerationDueToGravity * NM_Time * NM_Time) / 2);
	SetActorLocation(FVector(NewX, NewY, NewZ));
}