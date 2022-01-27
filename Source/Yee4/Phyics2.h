// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "Phyics2.generated.h"

/**
 * 
 */
UCLASS()
class YEE4_API APhyics2 : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Physics Solver")
	float accelerationDueToGravity = -400;
	
};
