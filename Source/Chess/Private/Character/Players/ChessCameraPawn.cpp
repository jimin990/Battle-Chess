// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Players/ChessCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Players/MyChessPlayerController.h"

// Sets default values
AChessCameraPawn::AChessCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent; 

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->SetupAttachment(RootComponent);

	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AChessCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChessCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChessCameraPawn::Click()
{
	AMyChessPlayerController* PC = Cast<AMyChessPlayerController>(GetController());

	if (!PC) return;

	PC->ClickPiece();
}

void AChessCameraPawn::ClickCancel()
{
	AMyChessPlayerController* PC = Cast<AMyChessPlayerController>(GetController());

	if (!PC) return;

	PC->ClickCancel();

}

void AChessCameraPawn::PossessCon(AMyChessPlayerController* CP)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("PossessCon %d"), IsLocallyControlled());

	if(IsLocallyControlled())
	{
		if (!CP->CameraIMC)
		{
			UE_LOG(LogTemp, Warning, TEXT("IMC is Null"));
			return;
		}

		if (!ClickAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("ClickAction && MoveAction is Null"));
			return;
		}

		if (!CP) return;


		if (ULocalPlayer* LocalPlayer = CP->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				//Subsystem->ClearAllMappings();
				Subsystem->RemoveMappingContext(CP->PieceIMC);
				Subsystem->AddMappingContext(CP->CameraIMC, 0);
			}
		}

		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EIC->ClearActionBindings();
			EIC->BindAction(ClickAction, ETriggerEvent::Started, this, &AChessCameraPawn::Click);
			EIC->BindAction(ClickCancelAction, ETriggerEvent::Started, this, &AChessCameraPawn::ClickCancel);
		}
	
	}
	
}

void AChessCameraPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	UE_LOG(LogTemp, Warning, TEXT("PawnClientRestart"));

	AMyChessPlayerController* PC =
		Cast<AMyChessPlayerController>(GetController());

	PossessCon(PC);
}
