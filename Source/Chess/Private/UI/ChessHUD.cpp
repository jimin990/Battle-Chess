// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChessHUD.h"
#include "Components/TextBlock.h"
#include "Character/Players/MyChessPlayerController.h"
#include "Character/Players/ChessCameraPawn.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "UI/PieceStatusWidget.h"
#include "UI/PlayerBattleCardWidget.h"
#include "Components/TextBlock.h"

void UChessHUD::NativeConstruct()
{
	Super::NativeConstruct();

	AMyChessPlayerController* PC =
		Cast<AMyChessPlayerController>(
			GetOwningPlayer()
		);

	if (!PC)
	{
		return;
	}

	PC->OnCurHoverPieceChanged.AddDynamic(
		this,
		&UChessHUD::UpdatePieceStatus
	);
}

/*
void UChessHUD::UpdatePieceStatus(AChessPieceBase* CurPiece, bool isTeam)
{
	//UE_LOG(LogTemp, Warning, TEXT("Widget Delegate Called"));

	if (isTeam)
	{
		if (MyBattleCardWidget)
		{
			MyBattleCardWidget->InitCard(CurPiece);
		}

	}
	else
	{
		if (EnemyBattleCardWidget)
		{
			EnemyBattleCardWidget->InitCard(CurPiece);
		}
	}

	/////////////
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
	
	

	if (isTeam && !bIsOpenedMyTeamWidget)
	{
		PlayMyTeamWidget();
	}

}
*/

void UChessHUD::UpdatePieceStatus(
    AChessPieceBase* CurPiece,
    bool bIsTeam)
{
    //----------------------------------
    // Hover 해제
    //----------------------------------

    if (!CurPiece)
    {
        // 선택된 말이 없을 때만 닫기
        if (!SelectedPiece &&
            bIsOpenedMyTeamWidget)
        {
            ReverseMyTeamWidget();

            bIsOpenedMyTeamWidget = false;
        }

        if (bIsOpenedEnemyWidget)
        {
            ReverseEnemyWidget();

            bIsOpenedEnemyWidget = false;
        }

        return;
    }

    //----------------------------------
    // 아군
    //----------------------------------

    if (bIsTeam)
    {
        // 이미 선택된 말이 있으면
        // Hover 무시
        if (SelectedPiece)
        {
            return;
        }

        if (MyBattleCardWidget)
        {
            MyBattleCardWidget->InitCard(
                CurPiece
            );
        }

        if (!bIsOpenedMyTeamWidget)
        {
            PlayMyTeamWidget();

            bIsOpenedMyTeamWidget = true;
        }

        return;
    }

    //----------------------------------
    // 적군
    //----------------------------------

    if (EnemyBattleCardWidget)
    {
        EnemyBattleCardWidget->InitCard(
            CurPiece
        );
    }

    if (!bIsOpenedEnemyWidget)
    {
        PlayEnemyWidget();

        bIsOpenedEnemyWidget = true;
    }
}

void UChessHUD::SetSelectedPiece(
    AChessPieceBase* Piece)
{
    SelectedPiece = Piece;

    if (!SelectedPiece)
    {
        return;
    }

    if (MyBattleCardWidget)
    {
        MyBattleCardWidget->InitCard(
            SelectedPiece
        );
    }

    if (!bIsOpenedMyTeamWidget)
    {
        PlayMyTeamWidget();

        bIsOpenedMyTeamWidget = true;
    }
}

void UChessHUD::ClearSelectedPiece()
{
    SelectedPiece = nullptr;

    if (bIsOpenedMyTeamWidget)
    {
        ReverseMyTeamWidget();

        bIsOpenedMyTeamWidget = false;
    }
}

void UChessHUD::ViewBattleResult(int32 WinTeam, float PlusTime)
{
    // WinTeam 0 무승부, 1 백팀, 2 흑팀
    AMyChessPlayerController* PC =
        Cast<AMyChessPlayerController>(
            GetOwningPlayer()
        );

    if (!PC)
    {
        return;
    }


    if (WinTeam == 0)
    {
        WinTeamText->SetText(
            FText::FromString(
                FString::Printf(TEXT("DRAW!"))
            )
        );

        PlusTimeText->SetText(
            FText::FromString(
                FString::Printf(TEXT(""))
            )
        );
    }
    else if (WinTeam == 1)
    {
        if (PC->PlayerTeam == EChessTeam::White)
        {
            WinTeamText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("WIN!"))
                )
            );

            PlusTimeText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("+ %d TIME"), FMath::FloorToInt(PlusTime))
                )
            );
        }
        else
        {
            WinTeamText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("lOSE!"))
                )
            );

            PlusTimeText->SetText(
                FText::FromString(
                    FString::Printf(TEXT(""))
                )
            );
        }
    }
    else
    {
        if (PC->PlayerTeam == EChessTeam::Black)
        {
            WinTeamText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("WIN!"))
                )
            );

            PlusTimeText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("+ %d TIME"), FMath::FloorToInt(PlusTime))
                )
            );
        }
        else
        {
            WinTeamText->SetText(
                FText::FromString(
                    FString::Printf(TEXT("lOSE!"))
                )
            );

            PlusTimeText->SetText(
                FText::FromString(
                    FString::Printf(TEXT(""))
                )
            );
        }
    }

    PlayWinTeam();
}

