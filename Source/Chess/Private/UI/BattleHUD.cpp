// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "Core/ChessGameState.h"

void UBattleHUD::NativeConstruct()
{
	MaxHp = 100;
}

// 많이 위험하지만 나중에 변경
void UBattleHUD::InitPiece(AChessPieceBase* InPawn)
{
	OwnPawn = InPawn;

	AChessGameState* GS =
		GetWorld()->GetGameState<AChessGameState>();

	if (GS)
	{
		GS->OnBattleTimeChanged.RemoveAll(this);
		GS->OnBattleTimeChanged.AddDynamic(
			this,
			&UBattleHUD::SetBattleRemainTime);

		if (OwnPawn->Team == EChessTeam::White)
		{
			BattleRemainTime->SetText(
				FText::AsNumber(GS->WhiteBattleTime)
			);
		}
		else
		{
			BattleRemainTime->SetText(
				FText::AsNumber(GS->BlackBattleTime)
			);
		}
	}

	InPawn->OnHPChanged.RemoveAll(this);
	InPawn->OnHPChanged.AddDynamic(
		this,
		&UBattleHUD::SetHpBar
	);

	MaxHp = InPawn->Stat.MaxHP;

	if (CurHp)
	{
		CurHp->SetText(
			FText::AsNumber(InPawn->CurrentHP)
		);
	}

	if (HpBar)
	{
		HpBar->SetPercent(InPawn->CurrentHP / MaxHp);
	}

	//BattleCounter->SetVisibility(ESlateVisibility::Collapsed);
}

void UBattleHUD::SetHpBar(float NewHp)
{
	if (CurHp)
	{
		CurHp->SetText(
			FText::AsNumber(NewHp)
		);
	}

	if (HpBar)
	{
		HpBar->SetPercent(NewHp / MaxHp);
	}
}

void UBattleHUD::SetBattleRemainTime(float WhiteBattleTime, float BlackBattleTime)
{
	if (!OwnPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Own Pawn is null"));
	}

	if (OwnPawn->Team == EChessTeam::White)
	{
		BattleRemainTime->SetText(
			FText::FromString(
				FString::Printf(TEXT("%d"), FMath::FloorToInt(WhiteBattleTime))
			)
		);
	}
	else
	{

		BattleRemainTime->SetText(
			FText::FromString(
				FString::Printf(TEXT("%d"), FMath::FloorToInt(BlackBattleTime))
			)
		);
	}
}

void UBattleHUD::SetCounter()
{
	Counter--;

	BattleCounter->SetText(
		FText::AsNumber(FMath::FloorToInt(Counter)));

	if (Counter == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BettingCounterTimer);

		UE_LOG(LogTemp, Warning, TEXT("Timer End"));

		BattleCounter->SetVisibility(ESlateVisibility::Collapsed);

		BattleCounter->SetText(
			FText::AsNumber(3));
	}
}

void UBattleHUD::PlayBattleCounter()
{
	Counter = 3;
	BattleCounter->SetVisibility(ESlateVisibility::Visible);

	//게임 시작 함수 제작 - 턴 시작
	GetWorld()->GetTimerManager().SetTimer(
		BettingCounterTimer,
		this,
		&UBattleHUD::SetCounter,
		1.0f,
		true
	);
}
