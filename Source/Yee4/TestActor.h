// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Vector.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"


UCLASS()
class YEE4_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	FVector initialPosition = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	FVector initialVelocity = FVector(-1000.0f, 1.0f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	float T = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	float accerlerationDueToGravity = 9.81f;


	UFUNCTION(BlueprintCallable)
	void MoveActor(FVector newPosition);


	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
