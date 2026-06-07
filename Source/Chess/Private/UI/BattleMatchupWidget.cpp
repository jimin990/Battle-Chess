// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleMatchupWidget.h"
#include "Components/Button.h"
#include "Core/ChessGameState.h"
#include "Character/Players/MyChessPlayerController.h"
#include "UI/PlayerBattleCardWidget.h"
#include "UI/BettingTimerWidget.h"
#include "Components/TextBlock.h"
#include "Character/ChessPiece/ChessPieceBase.h"

// 나중에 수정 필수!
void UBattleMatchupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SendButton->OnClicked.AddDynamic(this, &UBattleMatchupWidget::BettingTime);

	AChessGameState* GS = Cast<AChessGameState>(GetWorld()->GetGameState());

	if (!GS) return;

	AMyChessPlayerController* PC = Cast< AMyChessPlayerController>(GetOwningPlayer());

	if (!PC)
	{
		return;
	}

	if (PC->PlayerTeam == EChessTeam::White)
	{
		if (GS->Attacker->Team == PC->PlayerTeam)
		{
			MyBattleCardWidget->InitCard(GS->Attacker);
			EnemyBattleCardWidget->InitCard(GS->Defender);
		}
		else
		{
			MyBattleCardWidget->InitCard(GS->Defender);
			EnemyBattleCardWidget->InitCard(GS->Attacker);
		}
		
	}
	else
	{
		if (GS->Attacker->Team == PC->PlayerTeam)
		{
			MyBattleCardWidget->InitCard(GS->Attacker);
			EnemyBattleCardWidget->InitCard(GS->Defender);
		}
		else
		{
			MyBattleCardWidget->InitCard(GS->Defender);
			EnemyBattleCardWidget->InitCard(GS->Attacker);
		}
	}

	if (BettingTimeText)
	{
		BettingTimeText->SetText(
			FText::AsNumber(10)
		);
	}

	//게임 시작 함수 제작 - 턴 시작
	GetWorld()->GetTimerManager().SetTimer(
		BettingTimerHandle,
		this,
		&UBattleMatchupWidget::BettingTimeCounting,
		1.0f,
		true
	);
}

void UBattleMatchupWidget::BettingTime()
{
	SendButton->SetIsEnabled(false);

	AMyChessPlayerController* PC = Cast< AMyChessPlayerController>(GetOwningPlayer());

	if (!PC)
	{
		return;
	}

	float Bettingtime = BettingTimerWidget->GetCurSliderBarValue();
	
	PC->Server_SetBattleTime(Bettingtime, PC->PlayerTeam);
	
}

void UBattleMatchupWidget::BettingTimeCounting()
{
	BettingTimeCount--;

	BettingTimeText->SetText(
		FText::AsNumber(FMath::FloorToInt(BettingTimeCount)));


	if (BettingTimeCount == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BettingTimerHandle);

		UE_LOG(LogTemp, Warning, TEXT("Timer End"));
	}
}
