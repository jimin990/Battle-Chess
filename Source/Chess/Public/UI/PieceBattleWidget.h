// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PieceBattleWidget.generated.h"

class UTextBlock;
class AChessPieceBase;
class UProgressBar;
/**
 * 
 */
UCLASS()
class CHESS_API UPieceBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	TObjectPtr<AChessPieceBase> OwnPiece;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	void InitWidget(AChessPieceBase* OwnPiece);

	UFUNCTION()
	void UpdateHp(float NewHp);

	float CurHp;
};
