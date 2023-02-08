// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportProjectile.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATeleportProjectile::ATeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");

	
}


// Called when the game starts or when spawned
void ATeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TestTimer, this, &ATeleportProjectile::TimeElapes, 1.f);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true); //본인 캐릭터 충돌 무시
}

// Called every frame
void ATeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATeleportProjectile::TimeElapes()
{
	SetLifeSpan(1.f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleprotParticle, GetActorLocation(), GetActorRotation());
	GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
	//GetWorldTimerManager().ClearTimer(TestTimer);
}






