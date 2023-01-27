// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputMappingContext;

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

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	// class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Inputs")
	int32   Priority;
	
	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
