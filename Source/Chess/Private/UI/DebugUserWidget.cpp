// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugUserWidget.h"
#include "Character/Players/MyChessPlayerController.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/CheckBox.h"
#include "Core/ChessGameState.h"

void UDebugUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BettingPhaseButton)
    {
        BettingPhaseButton->OnClicked.AddDynamic(this, &UDebugUserWidget::SetBettingPhase);
    }

    if (BattlePhaseButton)
    {
        BattlePhaseButton->OnClicked.AddDynamic(this, &UDebugUserWidget::SetBattlePhase);
    }
    
    if (EndBattlePhaseButton)
    {
        EndBattlePhaseButton->OnClicked.AddDynamic(this, &UDebugUserWidget::SetBattleEndPhase);
    }
    
    if (GameEndPhaseButton)
    {
        GameEndPhaseButton->OnClicked.AddDynamic(this, &UDebugUserWidget::SetGameEndPhase);
    }
}

void UDebugUserWidget::SetBettingPhase()
{
#if !UE_BUILD_SHIPPING
    if (AMyChessPlayerController* PC = GetOwningPlayer<AMyChessPlayerController>())
    {
        int32 AttackerIndex = FMath::RoundToInt(AttackerIndexSpinBox_Betting->GetValue());
        int32 DefenderIndex = FMath::RoundToInt(DefenderIndexSpinBox_Betting->GetValue());

        PC->Server_DebugChangeToBettingPhase(
            AttackerIndex,
            DefenderIndex);
    }
#endif
}

void UDebugUserWidget::SetBattlePhase()
{
#if !UE_BUILD_SHIPPING
    if (AMyChessPlayerController* PC = GetOwningPlayer<AMyChessPlayerController>())
    {
        int32 AttackerIndex = FMath::RoundToInt(AttackerIndexSpinBox_Battle->GetValue());
        int32 DefenderIndex = FMath::RoundToInt(DefenderIndexSpinBox_Battle->GetValue());
        float AttackerBattleTime = AttackerBattleTimeSpinBox->GetValue();
        float DefenderBattleTime = DefenderBattleTimeSpinBox->GetValue();

        PC->Server_DebugChangeToBattlePhase(
            AttackerIndex,
            DefenderIndex,
            AttackerBattleTime,
            DefenderBattleTime
        );
    }
#endif
}

void UDebugUserWidget::SetBattleEndPhase()
{
#if !UE_BUILD_SHIPPING
    if (AMyChessPlayerController* PC = GetOwningPlayer<AMyChessPlayerController>())
    {
        const bool bAttackerWin = AttackWinCheckBox->IsChecked();

        PC->Server_DebugEndBattle(bAttackerWin);
    }
#endif
}

void UDebugUserWidget::SetGameEndPhase()
{
#if !UE_BUILD_SHIPPING
    if (AMyChessPlayerController* PC = GetOwningPlayer<AMyChessPlayerController>())
    {
        const bool bWhiterWin = WhiteWinCheckBox->IsChecked();
        if (bWhiterWin)
        {
            PC->Server_DebugEndGame(EChessTeam::White);
        }
        else
        {
            PC->Server_DebugEndGame(EChessTeam::Black);
        }
    }
#endif
}