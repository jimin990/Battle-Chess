// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeWidget.generated.h"

class UTextBlock;
class AMyChessPlayerController;
/**
 * 
 */
UCLASS()
class CHESS_API UTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WhiteTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BlackTime;

	UPROPERTY()
	TObjectPtr<AMyChessPlayerController> PC;

	UFUNCTION()
	void UpdatWhiteTime(float WhiteRemainTime, float BlackRemainTime, EChessTeam CurTeam);

	// 나중에 턴 변경 델리게이트 변경
	bool bOpened = false;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMyTrun();

	UFUNCTION(BlueprintImplementableEvent)
	void ReversePlayMyTrun();
};
