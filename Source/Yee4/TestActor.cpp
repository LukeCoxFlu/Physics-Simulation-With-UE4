// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Misc/FrameRate.h"

void ATestActor::MoveActor(FVector newPosition)
{
	SetActorLocation(newPosition);
}

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	initialPosition = GetActorLocation();

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor Pos: %s"), *initialPosition.ToString())); 
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{			

	Super::Tick(DeltaTime);
	T += DeltaTime;

	
	float newX = initialPosition.X + (initialVelocity.X * T);
	float newY = initialPosition.Y + (initialPosition.Y * T);
	float newZ = initialPosition.Z + (initialVelocity.Z * T) + ((accerlerationDueToGravity * T * T) / 2);

	//if(GEngine)
      //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!")); 
	
	
	
	
	MoveActor(FVector(newX, newY, newZ));


}

