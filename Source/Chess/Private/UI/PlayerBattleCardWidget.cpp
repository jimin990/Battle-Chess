// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerBattleCardWidget.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UPlayerBattleCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerBattleCardWidget::InitCard(AChessPieceBase* Piece)
{
	if (UEnum* EnumPtr = StaticEnum<EChessPieceType>())
	{
		PieceTypeText->SetText(
			EnumPtr->GetDisplayNameTextByValue(
				(int64)Piece->PieceType
			)
		);
	}

	CurrentHpText->SetText(
		FText::FromString(
			FString::Printf(
				TEXT("%d"),
				static_cast<int32>(Piece->CurrentHP)
			)
		)
	);

	if (HpBar)
	{
		HpBar->SetPercent(Piece->CurrentHP / Piece->Stat.MaxHP);
	}

	if (PieceTypeImage)
	{
		if (Piece->PieceType == EChessPieceType::Pawn)
		{
			PieceTypeImage->SetBrushFromTexture(PawnImage);
		}
		else if (Piece->PieceType == EChessPieceType::Bishop)
		{
			PieceTypeImage->SetBrushFromTexture(BishopImage);
		}
		else if (Piece->PieceType == EChessPieceType::Rook)
		{
			PieceTypeImage->SetBrushFromTexture(RookImage);
		}
		else if (Piece->PieceType == EChessPieceType::Knight)
		{
			PieceTypeImage->SetBrushFromTexture(KnightImage);
		}
		else if (Piece->PieceType == EChessPieceType::Queen)
		{
			PieceTypeImage->SetBrushFromTexture(QueenImage);
		}
		else if (Piece->PieceType == EChessPieceType::King)
		{
			PieceTypeImage->SetBrushFromTexture(KingImage);
		}
	}
		
}