// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AACharacter::AACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp -> SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp -> SetupAttachment(SpringArmComp);
}

void AACharacter::PrimaryAttack()
{
	FTransform SpawnTransform(GetControlRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
	Super::BeginPlay();
	InputsAtBeginPlay();
}

// Called every frame
void AACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
    	Input->BindAction(IA_Player, ETriggerEvent::Triggered, this, &ThisClass::InputMoves);
    	Input->BindAction(IA_DirectionRef, ETriggerEvent::Triggered, this, &ThisClass::InputDirection);
    	Input->BindAction(IA_PrimaryAttackRef, ETriggerEvent::Triggered, this, &ThisClass::PrimaryAttack);
}

void AACharacter::InputsAtBeginPlay()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	// Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, Priority);
}

void AACharacter::InputMoves(const FInputActionValue& Value)
{
		if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
 
		// Forward/Backward direction
		if (MoveValue.X != 0.f)
		{
			const FVector Direction_RotateVector = MovementRotation.RotateVector(FVector::ForwardVector);
			const FVector Direction_FRotationMatrix(FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::X));
			const FVector Direction_Vector = MovementRotation.Vector();

			UE_LOG(LogTemp, Warning, TEXT("Forward ====================================================================================== "));
			UE_LOG(LogTemp, Warning, TEXT("%s Function in %s (%s)"), ANSI_TO_TCHAR(__FUNCTION__), *GetName(), *GetClass()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Direction_RotateVector : %s"), *Direction_RotateVector.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Direction_FRotationMatrix : %s"), *Direction_FRotationMatrix.ToString());
			UE_LOG(LogTemp, Warning, TEXT("MovementRotation.Vector : %s"), *Direction_Vector.ToString());
			UE_LOG(LogTemp, Warning, TEXT("MovementRotation : %s"), *MovementRotation.ToString());
			
			UE_LOG(LogTemp, Warning, TEXT("============================================================================================== "));
			
			
			AddMovementInput(Direction_Vector, MoveValue.X);
		}
 
		// Right/Left direction
		if (MoveValue.Y != 0.f)
		{
			// Get right vector
			const FVector Direction_RotateVector = MovementRotation.RotateVector(FVector::RightVector);
			const FVector Direction_FRotationMatrix(FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y));
			const FVector Direction_RightVector = FRotationMatrix(MovementRotation).GetScaledAxis(EAxis::Y);
			
			UE_LOG(LogTemp, Warning, TEXT("RightVector ====================================================================================== "));
			UE_LOG(LogTemp, Warning, TEXT("%s Function in %s (%s)"), ANSI_TO_TCHAR(__FUNCTION__), *GetName(), *GetClass()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Direction_RotateVector : %s"), *Direction_RotateVector.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Direction_FRotationMatrix : %s"), *Direction_FRotationMatrix.ToString());
			UE_LOG(LogTemp, Warning, TEXT("RightVector : %s"), *Direction_RightVector.ToString());
			UE_LOG(LogTemp, Warning, TEXT("MovementRotation : %s"), *MovementRotation.ToString());
			
			UE_LOG(LogTemp, Warning, TEXT("============================================================================================== "));
 
			AddMovementInput(Direction_RotateVector, MoveValue.Y);
		}
	}
}

void AACharacter::InputDirection(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
 
		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}
 
		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}





