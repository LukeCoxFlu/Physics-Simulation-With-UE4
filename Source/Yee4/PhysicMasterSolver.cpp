// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicMasterSolver.h"
#include <math.h>
#include <ThirdParty/openexr/Deploy/OpenEXR-2.3.0/OpenEXR/include/ImathMath.h>

#include "DrawDebugHelpers.h"
#include "physicsWorldSettings.h"
#include "Kismet/GameplayStatics.h"

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhysicMasterSolver::StaticClass(), ListOfPhysObjects);
	ListOfPhysObjects.Remove(this);

	AphysicsWorldSettings* controller =  Cast<AphysicsWorldSettings>(UGameplayStatics::GetActorOfClass(GetWorld(), AphysicsWorldSettings::StaticClass()));
	if(affectedByGravity)
	{
		if(controller != nullptr)
		{
			accelerationDueToGravity = controller->accelerationDueToGravity;
			AccelerationForce.Z = accelerationDueToGravity;
		}
	}
		
	StPCollisionEnergyLoss = *controller->mapOfCollisionTypesToEnergyLoss.Find(ENUM_COLLISION_TYPES::CT_SPHERETOPLANE);
	StSCollisionEnergyLoss = *controller->mapOfCollisionTypesToEnergyLoss.Find(ENUM_COLLISION_TYPES::CT_SPHERETOSPHERE);
	
	previousPos = GetActorLocation();

	velocity = spawnedVelocity;

	if(PhysicsObject->GetStaticMesh() == nullptr)
	{
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No set Mesh on!")); 
	}
	
}

// Called every frame
void APhysicMasterSolver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(StaticObj) return;
	if(IsNewtonian)
	{
		NewtonianMotion(DeltaTime);
		return;
	}

	if(DebugOptionOn) debugValues(DeltaTime);

	//Collisions
	if(!(RigidBody_Types == ENUM_RIGIDBODY_TYPES::RT_Plane))
	{
		for (AActor* Object : ListOfPhysObjects)
		{
			APhysicMasterSolver* CompatableObject = Cast<APhysicMasterSolver>(Object);
			if(CompatableObject != nullptr)
			{
				// check if sphere, if so do sphere to sphere collision
				if(CompatableObject->RigidBody_Types == ENUM_RIGIDBODY_TYPES::RT_Sphere)
				{
					FVector P1 = GetActorLocation();
					FVector P2 = CompatableObject->GetActorLocation();
					FVector V1 = (velocity + AccelerationForce * DeltaTime) * DeltaTime;
					FVector V2 = (CompatableObject->velocity + CompatableObject->AccelerationForce * DeltaTime) * DeltaTime ;
					float r1 = GetRadius();
					float r2 = CompatableObject->GetRadius();
					float m1 = Mass;
					float m2 = CompatableObject->Mass;

					float deltaXp = P1.X - P2.X;
					float deltaYp = P1.Y - P2.Y;
					float deltaZp = P1.Z - P2.Z;

					float deltaXv = V1.X - V2.X;
					float deltaYv = V1.Y - V2.Y;
					float deltaZv = V1.Z - V2.Z;
					
					float A = (deltaXv * deltaXv) + (deltaYv * deltaYv) + (deltaZv * deltaZv);
					float C = ((deltaXp * deltaXp) + (deltaYp * deltaYp) + (deltaZp * deltaZp)) - ((r1 + r2) * (r1 + r2));
					float B = (2 * deltaXp * deltaXv) + (2 * deltaYp * deltaYv) + (2 * deltaZp * deltaZv);

					float discriminant = (B*B) - (4*A*C);
					
					if ( discriminant > 0)
					{
						float minusT = ( -B - sqrt( discriminant)) / (2 * A);
						float plusT = ( -B + sqrt( discriminant)) / (2 * A);
						float T = minusT < plusT ? minusT : plusT;

						//Checking if it collides within this frame
						if(T < 1 && T > 0)
						{
							FVector ResultingP1 = P1 + (T*V1);
							FVector ResultingP2 = P2 + (T*V2);
							
							FVector V1c = velocity;
							FVector V2c = CompatableObject->velocity;
							
							FVector collisionNormal = ResultingP1 - ResultingP2;
							collisionNormal = collisionNormal / collisionNormal.Size();

							// Break down vectors into transfered and remainder force for sphere 1 impact
							float v1DotScaler = FVector::DotProduct(collisionNormal, V1c);
							FVector V1Collided = collisionNormal * v1DotScaler;
							FVector V1Remainder = V1c - V1Collided;

							// Break down vectors into transfered and remainder force for sphere 2 impact
							float v2DotScaler = FVector::DotProduct(collisionNormal, V2c);
							FVector V2Collided = collisionNormal * v2DotScaler;
							FVector V2Remainder = V2c - V2Collided;
						
							//Collision calculations
							// times the length by the sign of the dot product
							float v1Length = V1Collided.Size() *  ((v1DotScaler > 0) - (v1DotScaler < 0));
							float v2Length = V2Collided.Size() *  ((v2DotScaler > 0) - (v2DotScaler < 0));
							
							float TotalMagnitude = 2 * (m1 * v1Length + m2 * v2Length) / (m1 + m2);

							//Dividing these 
							float veloctyLengthAfterCollision1 = TotalMagnitude - v1Length;
							float veloctyLengthAfterCollision2 = TotalMagnitude - v2Length;

							V1Collided = V1Collided * (veloctyLengthAfterCollision1 / v1Length);
							V2Collided = V2Collided * (veloctyLengthAfterCollision2 / v2Length);

							//Setting the position to be flush with the collision
							//SetActorLocation(ResultingP1);
							//CompatableObject->SetActorLocation(ResultingP2);
							velocity = (V1Collided + V1Remainder);
							CompatableObject->velocity = (V2Collided + V2Remainder);
						}
					}
					else
					{
						//if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Colliding")); 
					}
				}
				// check if plane, then do sphere to plane collision
				else if(CompatableObject->RigidBody_Types == ENUM_RIGIDBODY_TYPES::RT_Plane)
				{
					if(CompatableObject->StaticObj)
					{
					    FVector V = (velocity + AccelerationForce * DeltaTime) * DeltaTime;
						FVector Normal = FVector::CrossProduct(CompatableObject->PhysicsObject->GetForwardVector(),CompatableObject->PhysicsObject->GetRightVector());
						//DrawDebugLine(GetWorld(), CompatableObject->GetActorLocation(), CompatableObject->GetActorLocation() + (Normal * 100), FColor::Black,false,0.2f,0,10);

						
						if(AngleBetweenTwoVectors(Normal, - V) < 90)
						{
							FVector k = CompatableObject->GetActorLocation();
							FVector P = GetActorLocation() - k;
								
							float q1 = AngleBetweenTwoVectors(Normal,P);
							float q2 = (90 - q1) * (3.14159 / 180); //In to radians

							float d = sinf(q2) * P.Size() ;

							float s = AngleBetweenTwoVectors(V, -Normal) * (3.14159 / 180); //In to radians
							float r = GetRadius();
							
							float VC = (d - r) / (cosf(s));
							
							if(VC <= V.Size())
							{
								V.Normalize();
								SetActorLocation(GetActorLocation() + (V * VC));
								
								FVector uVelocityB =  velocity / velocity.Size();
								FVector uNormal = Normal / Normal.Size();

								FVector uVelocityA = (2 * uNormal * FVector::DotProduct(uNormal,-uVelocityB) + uVelocityB);

								FVector VelocityA = uVelocityA * velocity.Size();
								
								velocity = VelocityA;
								SetActorLocation(GetActorLocation() + (V * VC));
								
							}
						}
					}
				}
				else
				{
					if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No set type on!")); 
				}
			}
		}
	}
	
	
	
	//Clamping Velocity
    	
    //if(velocity.Size() < 0.05f) velocity = velocity * 0;
    	
	// Adding air resistance
	
	velocity = velocity * (1 - (drag * DeltaTime));	
	
	
	
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


float APhysicMasterSolver::GetRadius() const
{
	return PhysicsObject->Bounds.SphereRadius;
}

void APhysicMasterSolver::debugValues(float DeltaTime)
{
	DrawDebugCircle(GetWorld(), GetActorLocation(), GetRadius(), 50, FColor::Red, false, DeltaTime);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (velocity + AccelerationForce * DeltaTime) * DeltaTime, FColor::Cyan, false, DeltaTime);
}


float APhysicMasterSolver::AngleBetweenTwoVectors(FVector a, FVector b)
{
	
	float theta = acosf((FVector::DotProduct(a,b) / (a.Size() * b.Size())));
	return theta * (180 / 3.14159);
}
