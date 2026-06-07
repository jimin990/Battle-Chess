// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleHUD.generated.h"

class UTextBlock;
class UProgressBar;
class AChessPieceBase;
/**
 * 
 */
UCLASS()
class CHESS_API UBattleHUD : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	TObjectPtr<AChessPieceBase> OwnPawn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BattleRemainTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurHp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;
	
	// 빙의한 체스 말 연결
	void InitPiece(AChessPieceBase* InPawn);

	UFUNCTION()
	void SetHpBar(float NewHp);

	float MaxHp;
	
	UFUNCTION()
	void SetBattleRemainTime(float WhiteBattleTime, float BlackBattleTime);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BattleCounter;

	float Counter = 3;

	FTimerHandle BettingCounterTimer;

	void SetCounter();

	void PlayBattleCounter();
};
