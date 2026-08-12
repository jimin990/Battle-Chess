// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "System/ChessTypes.h"
#include "Core/ChessGameState.h"
#include "NewChessGameMode.generated.h"

class AMyChessPlayerController;
class AChessBoard;
class AChessPieceBase;
class AChessCameraPawn;
enum class EChessPieceType : uint8;

/**
 * 
 */
UCLASS()
class CHESS_API ANewChessGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ANewChessGameMode();

public:
	// 접속 플로우 공부
	// 접속시 가장 먼저 실행
	virtual void PreLogin(
		const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual APlayerController* Login(
		UPlayer* NewPlayer,
		ENetRole InRemoteRole,
		const FString& Portal,
		const FString& Options,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	// 멀티 접속 플로우 공부

	virtual void InitGame(
		const FString& MapName,
		const FString& Options,
		FString& ErrorMessage
	) override;

	virtual void InitGameState() override;

	// 현재 준비 완료된 플레이어 수 
	int32 ReadyToPlayerNum = 0;

	// 준비가 완료된 플레이어가 호출하는 함수
	void ReadyPlayer();
	
	virtual void BeginPlay() override;

	// SeamTraval이 끝나면 호출이 되는 함수 - 플레이어들의 컨트롤러가 유지된다.
	virtual void PostSeamlessTravel() override;

	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	// SeamTraval, Login 이후 플레이어 컨트롤러가 준비 완료됬을 때 호출 되는 함수
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

	// Starplay 위치를 지정하는 함수
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	bool bChessGameStarted = false;

	// 게임 시작을 요청하는 함수
	void TryStartChessGame();
public:
	// 체스 보드 클래스
	UPROPERTY(EditAnywhere, Category = "Board")
	TSubclassOf<AChessBoard> ChessBoardClass;

	// 체스 말 클래스들
	UPROPERTY(EditAnywhere, Category = "Pieces")
	TMap<EChessPieceType, TSubclassOf<AChessPieceBase>> ChessPieceClasses;

public:
	// 플레이어 목록
	TArray<TObjectPtr<AMyChessPlayerController>> Players;

	UPROPERTY()
	TObjectPtr<AMyChessPlayerController> WhitePlayer;

	UPROPERTY()
	TObjectPtr<AMyChessPlayerController> BlackPlayer;

	UPROPERTY()
	TObjectPtr<AChessCameraPawn> WhiteCamera;

	UPROPERTY()
	TObjectPtr<AChessCameraPawn> BlackCamera;

	// 서버 검사
public:
	// 이동 명령을 내림
	void TryMovePiece(AChessPieceBase* Piece, AMyChessPlayerController* TryPlayer, int32 MoveIndex);

	// 이동 명령이 유효한지 검사
	bool CheckInvalidMove(AChessPieceBase* Piece, int32 MoveIndex);

	// 전투 돌입 전 세팅 - 같은 위치에서 말이 만났을 경우
	void SetBattle(AChessPieceBase* InAttacker, AChessPieceBase* InDefender, int32 InBattleIndex);

	// 전투 끝 (한 말이 사망했을 경우) - 쓰러진 말이 호출, 혹은 시간이 다 딸어진 말이 호출
	void SetEndBattle();

public: /*--------------------------- 게임 페이즈 ---------------------------*/
	// 게임 페이즈 변경 함수
	void ChangeGamePhase(EGamePhase NewPhase);

	// InitGame 게임 초기화
	void InitGamePhase();

	// 체스판 스폰 및 게임 스테이트에 연결 함수
	void SpawnChessBoard();

	// 현재 턴의 팀 시간 계산 
	void CheckTurnTime();
	/*------------------------------*/

	// PreGamePhase 게임 시작 대기
	void PreGamePhase();

	// 체스 말들 생성 함수
	void SpawnChessPieces();

	//체스 말 실제 스폰 함수
	void SpawnPiece(int32 Index, EChessPieceType PieceType, EChessTeam Team);
	/*------------------------------*/

	// Chess 체스 (본 게임 시작)
	void ChessPhase();
	/*------------------------------*/

	// Betting 배팅 페이즈
	void BettingPhase();

	// 배팅 결과 확인
	void CheckBettingResult();
	/*------------------------------*/

	// PreBattle 페이즈
	void PreBattlePhase();

	void CheckBattleTime();
	/*------------------------------*/

	// Battle Phase
	void BattlePhase();
	/*------------------------------*/

	// EndBattle 배틀 마무리 하기
	void EndBattlePhase();

	// 동시 호출을 막기 위해
	bool isChecking = false;

	// 배틀 결과 체크, 부전승 여부도 판단(베팅 타임에서), 이긴 팀 반환, 무승부 일경우 NONE 반환
	// -1 : 오류, 0 무승부, 1: Attacker 승리, 2: Defender 승리 
	int32 CheckBattleResult();
	//EChessTeam CheckBattleResult();
	/*------------------------------*/
	
	void FinishBattlePhase();

	void GameEndPhase();

	EChessTeam GameResult;

	// 모든 플레이어에게 페이드 아웃
	void PlayGlobalFade();
	
// Debug
public:
	// 배팅 페이즈로 변경  - 전투 중이면 변경 불가.
	UFUNCTION()
	void DebugChangeToBettingPhase(int32 AttackerIndex, int32 DefenderIndex);

	UFUNCTION()
	void DebugChangeToBattlePhase(int32 AttackerIndex, int32 DefenderIndex, float AttackerBattleTime, float DefenderBattleTime);

	UFUNCTION()
	void DebugEndBattle(bool bAttackerWin);

	UFUNCTION()
	void DebugEndGame(EChessTeam WinnerTeam);
};