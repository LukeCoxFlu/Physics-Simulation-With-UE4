// Fill out your copyright notice in the Description page of Project Settings.


#include "EulerProjectileSolver.h"

// Sets default values
AEulerProjectileSolver::AEulerProjectileSolver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEulerProjectileSolver::BeginPlay()
{
	Super::BeginPlay();
	constantForce.Z = accelerationDueToGravity;
}

// Called every frame
void AEulerProjectileSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	previousPos = GetActorLocation();
	FVector temp = previousPos + velocity;
	SetActorLocation(temp);
	velocity = velocity + constantForce;

	//pos is * 123
	// gravity * 15.384

}

