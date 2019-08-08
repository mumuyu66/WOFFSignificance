// Fill out your copyright notice in the Description page of Project Settings.


#include "WOFFSignificanceGameModeBase.h"
#include "Public/WOFFSignificanceActor.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Particles/Emitter.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "SignificanceManager.h"


float WOFFSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform);
void WOFFPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered);
float EmitterSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform);
void EmitterPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered);

AWOFFSignificanceGameModeBase::AWOFFSignificanceGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWOFFSignificanceGameModeBase::BeginPlay()
{
	TArray<AActor *> actorArray;
	UGameplayStatics::GetAllActorsOfClass(this,AWOFFSignificanceActor::StaticClass(), actorArray);

	TArray<AActor *> emitterActorArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEmitter::StaticClass(), emitterActorArray);

	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			FName Tag(TEXT("Actor"));
			for (auto actor : actorArray) 
			{
				SignificanceManager->RegisterObject(actor,Tag, WOFFSignificanceFunction, USignificanceManager::EPostSignificanceType::Sequential, WOFFPostSignificanceFunction);
			}

			FName EmitterTag(TEXT("Emitter"));
			for (auto actor : emitterActorArray)
			{
				SignificanceManager->RegisterObject(actor, EmitterTag, EmitterSignificanceFunction, USignificanceManager::EPostSignificanceType::Sequential, EmitterPostSignificanceFunction);
			}
		}
	}
}

void AWOFFSignificanceGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
			{
				TArray<FTransform> TransformArray;
				TransformArray.Add(PlayerPawn->GetActorTransform());

				SignificanceManager->Update(TArrayView<FTransform>(TransformArray));
			}
		}
	}
}

void AWOFFSignificanceGameModeBase::EndPlay(EEndPlayReason::Type reason)
{
	if (UWorld* World = GetWorld())
	{
		if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(World))
		{
			SignificanceManager->UnregisterAll(FName(TEXT("Actor")));
			SignificanceManager->UnregisterAll(FName(TEXT("Emitter")));
		}
	}
}

static const float GSignificanceDistance = 2000.f;
float WOFFSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform)
{
	if (AWOFFSignificanceActor* Actor = Cast<AWOFFSignificanceActor>(Obj->GetObject()))
	{
		float Distance = (InTransform.GetLocation() - Actor->GetActorLocation()).Size();
		if (Distance < GSignificanceDistance)
		{
			float Significance = 1.f - Distance / GSignificanceDistance;
			return Significance;
		}
	}
	return 0.f;
}
void WOFFPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered)
{
	if (AWOFFSignificanceActor* Actor = Cast<AWOFFSignificanceActor>(Obj->GetObject()))
	{
		if (Significance > 0.f)
		{
			Actor->SetActorTickEnabled(true);
			Actor->SetTextColor(FColor::Red);
		}
		else
		{
			Actor->SetActorTickEnabled(false);
			Actor->SetTextColor(FColor::White);
		}
		Actor->SetText(FText::AsNumber(Significance));
	}
}

float EmitterSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, const FTransform& InTransform)
{
	if (AEmitter* Actor = Cast<AEmitter>(Obj->GetObject()))
	{
		float Distance = (InTransform.GetLocation() - Actor->GetActorLocation()).Size();
		if (Distance < GSignificanceDistance)
		{
			float Significance = 1.f - Distance / GSignificanceDistance;
			return Significance;
		}
	}
	return 0.f;
}
void EmitterPostSignificanceFunction(USignificanceManager::FManagedObjectInfo* Obj, float OldSignificance, float Significance, bool bUnregistered)
{
	if (AEmitter* Actor = Cast<AEmitter>(Obj->GetObject()))
	{
		if (Significance > 0.f)
		{
			Actor->GetParticleSystemComponent()->Activate(false);
		}
		else
		{
			Actor->GetParticleSystemComponent()->Activate(true);
		}
	}
}

