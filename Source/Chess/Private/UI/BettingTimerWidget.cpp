// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BettingTimerWidget.h"
#include "Core/ChessGameState.h"
#include "Components/TextBlock.h"
#include "Character/Players/MyChessPlayerController.h"
#include "System/ChessTypes.h"
#include "Components/Slider.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "Components/ProgressBar.h"

void UBettingTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AChessGameState* GS = Cast<AChessGameState>(GetWorld()->GetGameState());

	if (!GS) return;

	AMyChessPlayerController* PC = Cast<AMyChessPlayerController>(GetOwningPlayer());

	if (!PC)
	{
		return;
	}

	if (!SliderBar) return;

	if (PC->PlayerTeam == GS->Attacker->Team)
	{
		TimeText->SetText(
			FText::FromString(
				FString::Printf(TEXT("Attacker"))
			)
		);

		AttackerBonusText-> SetText(
			FText::FromString(
				FString::Printf(TEXT("+%d (Attacker bonus)"), 20)
			)
		);
	}
	else 
	{
		TimeText->SetText(
			FText::FromString(
				FString::Printf(TEXT("Defender"))
			)
		);
	}

	// 슬라이더 바
	if (PC->PlayerTeam == EChessTeam::White)
	{
		MaxRemainTime = GS->WhiteRemainTime;
	}
	else
	{
		MaxRemainTime = GS->BlackRemainTime;
	}

	SliderBar->SetMinValue(0.0f);
	SliderBar->SetMaxValue(MaxRemainTime);

	MaxTime->SetText(
		FText::FromString(
			FString::Printf(TEXT("/%d"), FMath::FloorToInt(MaxRemainTime))
		)
	);


	if (TimeBar)
	{
		TimeBar->SetPercent(0 / MaxRemainTime);
	}

	if (SliderBar)
	{
		SliderBar->OnValueChanged.AddDynamic(
			this,
			&UBettingTimerWidget::ChangedSliderBar
		);

		// 초기화
		SliderBar->SetValue(0.0f);
	}	

}

void UBettingTimerWidget::ChangedSliderBar(float Value)
{
	//SliderBar->SetValue(Value);

	BettingText->SetText(
		FText::AsNumber(FMath::FloorToInt(Value)));

	if (TimeBar)
	{
		TimeBar->SetPercent(Value / MaxRemainTime);
	}
}

float UBettingTimerWidget::GetCurSliderBarValue()
{
	return SliderBar->GetValue();
}
