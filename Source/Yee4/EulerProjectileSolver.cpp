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

	{
		FVector boxExtent = FVector(0.0f, 0.0f, 0.0f);
		FVector Origin = FVector(0.0f, 0.0f, 0.0f);
		GetActorBounds(false, Origin, boxExtent);
		radius = boxExtent.X ;
	}

	if (ImmobileSphere)
	{
		FVector boxExtent = FVector(0.0f, 0.0f, 0.0f);
		FVector Origin = FVector(0.0f, 0.0f, 0.0f);
		ImmobileSphereLocation = ImmobileSphere->GetActorLocation();
		ImmobileSphere->GetActorBounds(false, Origin, boxExtent);
		ImmobileSphereRadius = boxExtent.X;
	}
	constantForce.Z = accelerationDueToGravity;
}

// Called every frame
void AEulerProjectileSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	previousPos = GetActorLocation();
	float collisionMultiplier = 1;
	
	// Collisions ImmobileSphere
	if (ImmobileSphere)
	{
		//Add loc to velocity 
		FVector V = (velocity * DeltaTime) ;
		FVector A = previousPos - ImmobileSphereLocation;
		float q = acosf(FVector().DotProduct(A, V)/ (A.Size() * V.Size()));
	
		float d = sinf(q) * A.Size();

		// should work with d instead of A.Size() but it doesnt work as well. Also The distanceses are diameters I think which shouldnt work but it does
		if (A.Size() < radius + ImmobileSphereRadius)
		{
			float e = sqrtf((radius + ImmobileSphereRadius) * (radius + ImmobileSphereRadius) - d * d);
			float sizeVC = cosf(q) * A.Size() - e;
			//collisionMultiplier = (sizeVC / V.Size())
			SetActorLocation(previousPos + (V * (sizeVC / V.Size())));
			velocity = FVector(0.0f, 0.0f, 0.0f);
		}
	}

	velocity = velocity + (constantForce * DeltaTime);
	SetActorLocation(previousPos + (velocity * DeltaTime));




	//pos is * 123
	// gravity * 15.384

}

