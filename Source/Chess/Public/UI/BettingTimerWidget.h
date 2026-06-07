// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BettingTimerWidget.generated.h"

class UTextBlock;
class USlider;
class UProgressBar;
/**
 * 
 */
UCLASS()
class CHESS_API UBettingTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BettingText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackerBonusText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SliderBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> TimeBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTime;

	float MaxRemainTime = 10;

	UFUNCTION()
	void ChangedSliderBar(float Value);

	float GetCurSliderBarValue();
};
