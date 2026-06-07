// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PieceBattleWidget.h"
#include "Components/TextBlock.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "Components/ProgressBar.h"

void UPieceBattleWidget::NativeConstruct()
{
	
}

void UPieceBattleWidget::InitWidget(AChessPieceBase* NewPiece)
{
	OwnPiece = NewPiece;

	CurHp = OwnPiece->CurrentHP;

	if (HpText)
	{
		HpText->SetText(
			FText::AsNumber(CurHp)
		);
	}

	if (HpBar)
	{
		HpBar->SetPercent(CurHp / CurHp);
	}
}

void UPieceBattleWidget::UpdateHp(float NewHp)
{
	if (HpText)
	{
		HpText->SetText(
			FText::AsNumber(NewHp)
		);
	}

	if (HpBar)
	{
		HpBar->SetPercent(NewHp / CurHp);
	}
}

