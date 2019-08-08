// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SignificanceManager.h"

#include "WOFFSignificanceGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AWOFFSignificanceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWOFFSignificanceGameModeBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
};
