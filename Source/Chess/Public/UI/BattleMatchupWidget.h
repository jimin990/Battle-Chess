// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMatchupWidget.generated.h"

class UBettingTimerWidget;
class UPlayerBattleCardWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CHESS_API UBattleMatchupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBettingTimerWidget> BettingTimerWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerBattleCardWidget> MyBattleCardWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerBattleCardWidget> EnemyBattleCardWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SendButton;;

	UFUNCTION()
	void BettingTime();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BettingTimeText;

	void BettingTimeCounting();

	float BettingTimeCount = 10;

	FTimerHandle BettingTimerHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayIntro();
};

