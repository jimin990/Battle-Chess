// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ChessGameState.h"
#include "DebugUserWidget.generated.h"

class UButton;
class USpinBox;
class UCheckBox;
/**
 * 
 */
UCLASS()
class CHESS_API UDebugUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// 배팅 페이즈로 변경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BettingPhaseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> AttackerIndexSpinBox_Betting;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> DefenderIndexSpinBox_Betting;

	UFUNCTION()
	void SetBettingPhase();

	// 배틀 페이즈로 변경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BattlePhaseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> AttackerIndexSpinBox_Battle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> DefenderIndexSpinBox_Battle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> AttackerBattleTimeSpinBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> DefenderBattleTimeSpinBox;

	UFUNCTION()
	void SetBattlePhase();

	// 배틀 종료 페이즈로 변경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EndBattlePhaseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> AttackWinCheckBox;

	UFUNCTION()
	void SetBattleEndPhase();

	// 게임 종료 페이즈로 변경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameEndPhaseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> WhiteWinCheckBox;

	UFUNCTION()
	void SetGameEndPhase();
};
