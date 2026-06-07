// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChessHUD.generated.h"

class UPieceStatusWidget;
class AChessPieceBase;
class UPlayerBattleCardWidget;
class UTextBlock;

/**
 * 
 */
UCLASS()
class CHESS_API UChessHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerBattleCardWidget> MyBattleCardWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerBattleCardWidget> EnemyBattleCardWidget;

	// Stutus 창 업데이트 함수 - 델리게이트 연결
	UFUNCTION()
	void UpdatePieceStatus(AChessPieceBase* CurPiece, bool isTeam);


//	bool bIsOpenedMyTeamWidget = false;

	// 플레이어의 Piece 상태 위젯 애니메이션을 동작시키는 함수 - 블루프린트에서 연결
	UFUNCTION(BlueprintImplementableEvent)
	void PlayMyTeamWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ReverseMyTeamWidget();

	//bool bIsOpenedEnemyTeamWidget = false;

	// 플레이어의 Piece 상태 위젯 애니메이션을 동작시키는 함수 - 블루프린트에서 연결
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEnemyWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ReverseEnemyWidget();

	UPROPERTY()
	AChessPieceBase* SelectedPiece = nullptr;

	bool bIsOpenedMyTeamWidget = false;
	bool bIsOpenedEnemyWidget = false;

	void SetSelectedPiece(AChessPieceBase* Piece);

	void ClearSelectedPiece();


	// 결과 창
	void ViewBattleResult(int32 Winteam, float PlusTime);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WinTeamText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlusTimeText;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayWinTeam();
	/*
	// 플레이어의 Piece 상태 위젯 애니메이션을 동작시키는 함수 - 블루프린트에서 연결
	UFUNCTION(BlueprintImplementableEvent)
	void ViewLeftWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideLeftWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ViewRightWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void HideRightWidget();
	*/
};
