// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/WOFFSignificanceActor.h"
#include "Engine/Classes/Components/TextRenderComponent.h"

// Sets default values
AWOFFSignificanceActor::AWOFFSignificanceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("WOFFSignificanceText"));
	TextComponent->SetText(FText::AsNumber(0.f));
	TextComponent->SetWorldScale3D(FVector(5.f));
}

// Called when the game starts or when spawned
void AWOFFSignificanceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWOFFSignificanceActor::SetTextColor(FColor color)
{
	TextComponent->SetTextRenderColor(color);
}

void AWOFFSignificanceActor::SetText(FText Str)
{
	TextComponent->SetText(Str);
}