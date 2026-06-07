// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UOpeningWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpening();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayWhiteWin();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBlackWin();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnime();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEnd();

};
