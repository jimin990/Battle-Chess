// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "System/ChessTypes.h"
#include "Core/ChessGameState.h"
#include "InputActionValue.h"
#include "MyChessPlayerController.generated.h"

class AChessPieceBase;
class UInputAction;
class UInputMappingContext;
class AChessBoard;
class UTimeWidget;
class UChessHUD;
class UBattleMatchupWidget;
class UFadeInOutWidget;
class UBattleHUD;
class UOpeningWidget;
class UDebugUserWidget;

// 현재 Hover 중인 Piece의 변경 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCurHoverPieceChanged,
	AChessPieceBase*,Cur_HoverChessPiece,
	bool, isTeam);

UCLASS()
class CHESS_API AMyChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyChessPlayerController();
	
	// SeamTraval 후 호스트는 다시 호출하지 않는거 같음
	virtual void BeginPlay() override;

	// 그래서 이거 추가
	virtual void PostSeamlessTravel() override;

	// SeamTraval은 서버만 동작하니 로컬에도 뿌려서 로컬 델리게이트와 연결을 해준다.
	UFUNCTION(Client, Reliable)
	void Client_InitAfterSeamlessTravel();

	// 이 함수 기능 알아보기 
	virtual void BeginPlayingState() override;

	virtual void Tick(float DeltaTime) override;

	// 플레이어가 레디를 하는 함수
	UFUNCTION(Server, Reliable)
	void Server_Ready();

	// 게임 스테이트 훅
	FDelegateHandle GameStateSetHandle;

	void HandleGameStateSet(AGameStateBase* NewGameState);
	void BindToChessGameState(AChessGameState* NewGameState);

public:
	void SetupInputComponent();

public:
	UPROPERTY(Replicated)
	EChessTeam PlayerTeam;

	UPROPERTY()
	TObjectPtr<AChessPieceBase> CurPiece;

public:
	// 마우스 클릭 함수
	void ClickPiece();

	//마우스 클릭 취소
	void ClickCancel();

	// 말 선택 함수
	void SelectPiece(AChessPieceBase* Piece);

public:
	// 선택한 말의 이동가능 범위 표시 함수
	void ShowPawnMoves(AChessPieceBase* Piece);

public:
	// 타일 색상 변경 함수
	void ReturnBoardColor();

public:
	UPROPERTY(Replicated)
	TObjectPtr<AChessBoard> Board;

public:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CameraIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PieceIMC;

public:	
	virtual void AcknowledgePossession(APawn* P);

	// UI
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UTimeWidget> TimeWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UTimeWidget> TimerWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UChessHUD> ChessHUDClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UChessHUD> ChessHUDWidget;

	// 안내 UI
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UOpeningWidget> OpeningWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UOpeningWidget> OpeningWidget;

	// HUD 위젯의 내 팀 정보 나타내기
	void ViewLeft();

	// HUD 위젯의 내 팀 정보 숨기기
	void HideLeft();

	// HUD 위젯의 적 팀 정보 나타내기
	void ViewRight();

	// HUD 위젯의 적 팀 정보 숨기기
	void HideRight();

	//Hover 함수
	void Hover_ChessPiece();

	bool isHoverAvailable;

	bool isClickAvailable;

	//현재 마우스 아래 선택된 Piece
	UPROPERTY()
	TObjectPtr<AChessPieceBase> Cur_HoverChessPiece;

	UPROPERTY(BlueprintAssignable)
	FOnCurHoverPieceChanged OnCurHoverPieceChanged;

// Battle
public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UBattleMatchupWidget> BattleMatchupWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBattleMatchupWidget> BattleMatchupWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UBattleHUD> BattleHUDClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBattleHUD> BattleHUD;

public:
	// 배틀 UI
	void ShowBattleUI();
	void HideBattleUI();

	UFUNCTION(Server, Reliable)
	void Server_SetBattleTime(float NewTime, EChessTeam CheesTeam);

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UFadeInOutWidget> FadeInOutWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UFadeInOutWidget> FadeInOutWidget;

public:
	// 컨트롤러 초기화 함수 - 될때 까지 시도
	void TryBindGameState();

	// 위젯 설정 함수
	void SetWidget();

// 게임 페이즈
public:
	// 게임 페이즈 변경 델리게이트 연결 함수
	UFUNCTION()
	void GamePhaseChanged(EGamePhase NewPhase);

	// 게임 시작전 - 게임 화면으로 넘어온 직후
	void SetPreGamePhase();

	// 체스 UI ON/OFF
	// 나중에 하나의 UI로 만들기
	void ShowChessGameUI();
	void HideChessGameUI();

	// 페이드 인 아웃함수
	void FadeIn();
	void FadeOut();
	void FadeInOut();
	/*------------------------------*/

	// 체스 페이즈 설정으로 세팅
	void SetChessPhase();

	// 배팅 페이즈 설정으로 세팅
	void SetBettingPhase();

	// 배팅 UI 생성/제거
	void ShowBettingUI();
	void HideBettingUI();
	/*------------------------------*/

	// 배틀 전 
	void SetPreBattlePhase();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	/*------------------------------*/

	void SetBattlePhase();

	void StartBattleCounter();

	// 배틀 결과
	void SetEndBattlePhase();

	// 배틀 마무리
	void SetFinishBattlePhase();


	// 모든 플레이어 게임모드에 FadeInOut
	UFUNCTION(Client, Reliable)
	void Client_FadeInOut();

	void ShowBattleResult(int32 WinTeam, float PlusTime);

	void SetGameEndPhase();

public:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;

	UFUNCTION(Server, Reliable)
	void Server_MovePiece(AChessPieceBase* Piece, int32 MoveIndex);

//Debug
public:
	UPROPERTY(EditAnywhere, Category = "Debug|UI")
	TSubclassOf<UDebugUserWidget> DebugWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Debug|UI")
	TObjectPtr<UDebugUserWidget> DebugWidget;

	// 배팅 페이즈로 변경  - 전투 중이면 변경 불가.
	UFUNCTION(Server, Reliable)
	void Server_DebugChangeToBettingPhase(int32 AttackerIndex, int32 DefenderIndex);

	UFUNCTION(Server, Reliable)
	void Server_DebugChangeToBattlePhase(int32 AttackerIndex, int32 DefenderIndex, float AttackerBattleTime, float DefenderBattleTime);

	UFUNCTION(Server, Reliable)
	void Server_DebugEndBattle(bool bAttackerWin);

	UFUNCTION(Server, Reliable)
	void Server_DebugEndGame(EChessTeam WinnerTeam);

	UFUNCTION()
	void ShowDebugWidget();

	bool bShowDebugWidget = false;
};
