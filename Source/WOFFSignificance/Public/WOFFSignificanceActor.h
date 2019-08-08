// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WOFFSignificanceActor.generated.h"

class UTextRenderComponent;

UCLASS()
class WOFFSIGNIFICANCE_API AWOFFSignificanceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWOFFSignificanceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetTextColor(FColor color);

	UPROPERTY()
	UTextRenderComponent* TextComponent;

	void SetText(FText Str);
};
