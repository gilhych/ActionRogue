// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "TeleportProjectile.generated.h"

class UCapsuleComponent;

UCLASS()
class ACTIONROGUE_API ATeleportProjectile : public ASMagicProjectile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATeleportProjectile();
	
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Hit")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(EditAnywhere, Category="Particles")
	class UParticleSystem* TeleprotParticle;


	void TimeElapes();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle TestTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
