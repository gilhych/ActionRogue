// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "ACharacter.generated.h"

class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUE_API AACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACharacter();

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	// class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Inputs")
	int32   Priority;
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Projectile")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> TeleportProjectileClass;
	
	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_BlackHole;

	
	
	void InputsAtBeginPlay();
	
	///
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<class UInputAction> IA_Player;
	
	void InputMoves(const FInputActionValue& Value);

	///
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_DirectionRef;
	
	void InputDirection(const FInputActionValue& Value);

	///
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_PrimaryAttackRef;
	
	void PrimaryAttack();

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_PrimaryInteract;

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_Jump;

	void InputJump();

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_BlackHole;

	void InputBlackHole();
	void BlackHole_TimeElapes();

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> IA_Teleport;

	void PrimaryTeleport();
	void Teleprot_TimeElapes();
	
	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
