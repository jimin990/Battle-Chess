// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

class UMainWidget;
/**
 * 
 */
UCLASS()
class CHESS_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuController();

	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMainWidget> MainWidget;

	UFUNCTION()
	void HostGame();

	UFUNCTION()
	void JoinGame();
};
