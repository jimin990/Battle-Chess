// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeInOutWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UFadeInOutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	// 그냥 화면에서 검은 화면으로
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeIn();

	// 검정 화면에서 그냥 화면으로
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeOut();

	// 검정 화면에서 그냥 화면으로
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInOut();
};
