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

	radius = GetActorScale().X * 50;

	if (ImmobileSphere)
	{
		ImmobileSphereLocation = ImmobileSphere->GetActorLocation();
		ImmobileSphereRadius = ImmobileSphere->GetActorScale().X * 50;
	}
	constantForce.Z = accelerationDueToGravity;
}

// Called every frame
void AEulerProjectileSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	previousPos = GetActorLocation();
	FVector FinalLocation;
	
	// Collisions ImmobileSphere
	FVector V = (velocity * DeltaTime) - previousPos;

	FVector A = ImmobileSphereLocation - previousPos;


	velocity = velocity + (constantForce * DeltaTime);
	FinalLocation = previousPos + (velocity * DeltaTime);

	if (ImmobileSphere)
	{
		//Add loc to velocity
		float dot = A.X * V.X + A.Y * V.Y + A.Z * V.Z;
		float q = acosf(dot / (A.Size() * V.Size()));
	
		float d = sinf(q) * A.Size();
		// should work with d instead of A.Size() but it doesnt work as well. Also The distanceses are diameters I think which shouldnt work but it does
		if (d < radius + ImmobileSphereRadius)
		{
			//Collision Currently works the bellow code has some error in it
			float e = sqrtf(((radius + ImmobileSphereRadius) * (radius + ImmobileSphereRadius)) - (d * d));
			float sizeVC = (cosf(q) * A.Size()) - e;
			FVector Vc = (V / V.Size()) * sizeVC;
	
			velocity = FVector(0, 0, 0);
			FinalLocation = previousPos + Vc;
		}
	}


	SetActorLocation(FinalLocation);




	//pos is * 123
	// gravity * 15.384

}

