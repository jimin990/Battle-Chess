// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TimeWidget.h"
#include "Components/TextBlock.h"
#include "Core/ChessGameState.h"
#include "Character/Players/MyChessPlayerController.h"

void UTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

    PC = Cast<AMyChessPlayerController>(GetOwningPlayer());

    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("PC is null"));
    }

    AChessGameState* GS =
        GetWorld()->GetGameState<AChessGameState>();

    if (GS)
    {
        GS->OnTimeChanged.AddDynamic(
            this,
            &UTimeWidget::UpdatWhiteTime);

        WhiteTime->SetText(
            FText::AsNumber(GS->GameTime));

        BlackTime->SetText(
            FText::AsNumber(GS->GameTime));

        UpdatWhiteTime(GS->WhiteRemainTime, GS->BlackRemainTime, GS->CurrentTurnTeam);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Gs not GameStat"));
    }
}

void UTimeWidget::UpdatWhiteTime(float WhiteRemainTime, float BlackRemainTime, EChessTeam CurTeam)
{
    if (CurTeam == EChessTeam::White)
    {
        WhiteTime->SetText(
            FText::AsNumber(FMath::FloorToInt(WhiteRemainTime)));

        BlackTime->SetColorAndOpacity(
            FSlateColor(FLinearColor::White));

        WhiteTime-> SetColorAndOpacity(
            FSlateColor(FLinearColor::Green));
    }
    else if (CurTeam == EChessTeam::Black)
    {
        BlackTime->SetText(
            FText::AsNumber(FMath::FloorToInt(BlackRemainTime)));

        WhiteTime->SetColorAndOpacity(
            FSlateColor(FLinearColor::White));

        BlackTime->SetColorAndOpacity(
            FSlateColor(FLinearColor::Green));
    }

    if(CurTeam == PC->PlayerTeam && !bOpened && CurTeam != EChessTeam::None)
    {
        PlayMyTrun();
        bOpened = true;
    }
    else if (CurTeam != PC->PlayerTeam && bOpened && CurTeam != EChessTeam::None)
    {
        ReversePlayMyTrun();
        bOpened = false;
    }
}
