// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AACharacter::AACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp -> bUsePawnControlRotation = false;
	SpringArmComp -> SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp -> SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement() -> bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
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
		Input->BindAction(IA_PrimaryInteract, ETriggerEvent::Started, this, &ThisClass::PrimaryInteract);
		Input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::InputJump);
		Input->BindAction(IA_BlackHole, ETriggerEvent::Started, this, &ThisClass::InputBlackHole);
		Input->BindAction(IA_Teleport, ETriggerEvent::Started, this, &ThisClass::PrimaryTeleport);
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

void AACharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AACharacter::PrimaryAttack_TimeElapsed, 0.2f);

	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	
}

void AACharacter::PrimaryAttack_TimeElapsed()
{
	FHitResult OutHit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * 3000.f;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);	
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams, Params);
	FVector Vector_NetQuantize = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	FVector Vector = Vector_NetQuantize - GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTransform(Vector.Rotation(), GetMesh()->GetSocketLocation("Muzzle_01"));
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}

void AACharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AACharacter::InputJump()
{
	Jump();
	
	/*
	StopJumping(); 점프가 끝나는 순간을 찾아줌
	JumpMaxHoldTime; 점프가 최대치에 도달하는 시간을 정리
	JumpMaxCount;	점프 숫자를 관리
	GetCharacterMovement()->JumpZVelocity; 점프 가속도에 대한것을 정리
*/
}

void AACharacter::InputBlackHole()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AACharacter::BlackHole_TimeElapes, 0.2f);

	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	
}


void AACharacter::BlackHole_TimeElapes()
{
	FHitResult OutHit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * 3000.f;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams,Params);
	FVector Vector_NetQuantize = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	FVector Vector = Vector_NetQuantize - GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTransform(Vector.Rotation(), GetMesh()->GetSocketLocation("Muzzle_01"));
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(BlackHoleProjectileClass, SpawnTransform, SpawnParams);
}

void AACharacter::PrimaryTeleport()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AACharacter::Teleprot_TimeElapes, 0.2f);

	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	
}


void AACharacter::Teleprot_TimeElapes()
{
	FHitResult OutHit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * 700.f;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectQueryParams,Params);
	FVector Vector_NetQuantize = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	FVector Vector = Vector_NetQuantize - GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTransform(Vector.Rotation(), GetMesh()->GetSocketLocation("Muzzle_01"));
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTransform, SpawnParams);
}



