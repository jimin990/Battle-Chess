// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "System/ChessTypes.h"
#include "System/BattleInfo.h"
#include "ChessGameState.generated.h"

class AMyChessPlayerController;
class AChessBoard;
class AChessPieceBase;

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	None,
	InitGamePhase			UMETA(DisplayName = "게임 초기회"),
	PreGamePhase			UMETA(DisplayName = "게임 시작 전"),
	ChessPhase				UMETA(DisplayName = "체스 게임"),
	BettingPhase			UMETA(DisplayName = "배팅"),
	PreBattlePhase			UMETA(DisplayName = "배틀 전"),
	BattlePhase				UMETA(DisplayName = "배틀"),
	EndBattlePhase			UMETA(DisplayName = "배틀 종료"),
	FinishBattlePhase		UMETA(DisplayName = "배틀 마무리"),
	GameEndPhase			UMETA(DisplayName = "게임 종료")
};

// 현재 시간과 현재 턴이 변경되었을 때 델리게이트 선언 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnTimeChanged,
	float, WhiteRemainTime,
	float, BlackRemainTime,
	EChessTeam, CurTeam);

// 현재 게임 페이즈가 변경되었을 경우 동작하는 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnGamePhaseChanged,
	EGamePhase, GamePhase);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnBattleTimeChanged,
	float, WhiteBattleTime,
	float, BlackBattleTime);

UCLASS()
class CHESS_API AChessGameState : public AGameState
{
	GENERATED_BODY()

	AChessGameState();

public:
	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<AChessBoard> ChessBoard;

	//체스보드에 어떤 말들이 들어있는지 리스트
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AChessPieceBase>> ChessPieces;
	
	UPROPERTY()
	TObjectPtr<AMyChessPlayerController> WhitePlayer;

	UPROPERTY()
	TObjectPtr<AMyChessPlayerController> BlackPlayer;

public:
	// 현재 턴 팀
	UPROPERTY(Replicated)
	EChessTeam CurrentTurnTeam;

	//총 게임 시간
	UPROPERTY(EditAnywhere, Category = "Time")
	float GameTime;

	// White 남은 시간
	UPROPERTY(ReplicatedUsing = OnRep_RemainTime)
	float WhiteRemainTime;

	// Black 남은 시간
	UPROPERTY(ReplicatedUsing = OnRep_RemainTime)
	float BlackRemainTime;

	UFUNCTION()
	void OnRep_RemainTime();

	UPROPERTY(BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

	void SetTime(float DeductedTime);

	// Battle
public:
	UPROPERTY(ReplicatedUsing = OnRep_SetBattleTime)
	float BlackBattleTime;

	UPROPERTY(ReplicatedUsing = OnRep_SetBattleTime)
	float WhiteBattleTime;

	UPROPERTY(BlueprintAssignable)
	FOnBattleTimeChanged OnBattleTimeChanged;

	void BettingBattleTime(float InBattleTime, EChessTeam BattleTeam);

	// 배틀 시간 타이머
	UFUNCTION()
	void OnRep_SetBattleTime();

	UFUNCTION()
	void SetBattleTime(float DeductedTime);

	// 배틀이 시작됬는지
	bool isBattleStart = false;

public: /*--------------------------- 게임 페이즈 ---------------------------*/
	// 현재 게임 페이즈
	UPROPERTY(ReplicatedUsing = OnRep_GamePhaseChanged)
	EGamePhase CurGamePhase;

	// 배틀 페이즈가 변경되면 클라이언트 쪽 게임스테이트에서 실행하는 함수
	UFUNCTION()
	void OnRep_GamePhaseChanged();

	// 게임 페이즈 알림 변경 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnGamePhaseChanged OnGamePhaseChanged;

public: /*--------------------------- 배틀 페이즈 ---------------------------*/

	// 현재 배틀 중 인지
	bool isBattle;

	// 공격자
	UPROPERTY(Replicated)
	TObjectPtr<AChessPieceBase> Attacker;

	// 수비자
	UPROPERTY(Replicated)
	TObjectPtr<AChessPieceBase> Defender;

	// 전투가 벌어진 Index
	int32 BattleIndex;

	void SetGamePhase(EGamePhase NewGamePhase);

	// 전투 결과 함수

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BattleResult(int32 WinTeam, float PlusTime);

public:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps
	) const override;
};
