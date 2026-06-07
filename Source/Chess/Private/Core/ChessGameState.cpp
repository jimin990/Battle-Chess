// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ChessGameState.h"
#include "Net/UnrealNetwork.h"
#include "Character/Players/MyChessPlayerController.h"
#include "Kismet/GameplayStatics.h"

AChessGameState::AChessGameState()
{
	CurrentTurnTeam = EChessTeam::None;

	//게임 총 시간 지정
	GameTime = 1000.0f;

	//각 남은 시간 초기화
	WhiteRemainTime = GameTime;
	BlackRemainTime = GameTime;

	// 처음 전투 상태
	CurGamePhase = EGamePhase::None;
}

void AChessGameState::SetTime(float DeductedTime)
{
	// 서버 아니면 변경 불가
	if (!HasAuthority()) return;

	// 시간 적용
	if (CurrentTurnTeam == EChessTeam::White)
	{
		WhiteRemainTime -= DeductedTime;

	}
	else if (CurrentTurnTeam == EChessTeam::Black)
	{
		BlackRemainTime -= DeductedTime;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.f,
			FColor::Green,
			FString::Printf(TEXT("Battlepage"))
		);
	}

	OnTimeChanged.Broadcast(WhiteRemainTime, BlackRemainTime, CurrentTurnTeam);
}

void AChessGameState::OnRep_RemainTime()
{
	OnTimeChanged.Broadcast(WhiteRemainTime, BlackRemainTime, CurrentTurnTeam);
}

void AChessGameState::SetGamePhase(EGamePhase NewGamePhase)
{
	// 서버 아니면 변경 불가
	if (!HasAuthority()) return;

	// 호스트의 경우 알림
	UE_LOG(LogTemp, Warning, TEXT("Host Changed GamePhase"));
	CurGamePhase = NewGamePhase;
	OnGamePhaseChanged.Broadcast(CurGamePhase);
}

void AChessGameState::OnRep_GamePhaseChanged()
{
	// 클라이언트의 경우 알림
	UE_LOG(LogTemp, Warning, TEXT("Client Changed GamePhase"));
	OnGamePhaseChanged.Broadcast(CurGamePhase);
}

void AChessGameState::BettingBattleTime(float InBattleTime, EChessTeam BattleTeam)
{
	// 어떤 팀이 내렸는지
	if (BattleTeam == EChessTeam::White)
	{
		if (InBattleTime <= WhiteRemainTime)
		{
			WhiteBattleTime = InBattleTime;

			WhiteRemainTime-= InBattleTime;
		}
		else
		{
			// 현재 남은 시간보다 많은 시간 투자했을 경우 - 버그 방지
			UE_LOG(LogTemp, Warning, TEXT("Too much time required!"));
		}
	}
	else if (BattleTeam == EChessTeam::Black)
	{
		if (InBattleTime <= BlackRemainTime)
		{
			BlackBattleTime = InBattleTime;

			BlackRemainTime -= InBattleTime;
		}
		else
		{
			// 현재 남은 시간보다 많은 시간 투자했을 경우 - 버그 방지
			UE_LOG(LogTemp, Warning, TEXT("Too much time required!"));
		}
	}
}

void AChessGameState::OnRep_SetBattleTime()
{
	OnBattleTimeChanged.Broadcast(WhiteBattleTime, BlackBattleTime);
}

void AChessGameState::SetBattleTime(float DeductedTime)
{
	BlackBattleTime -= DeductedTime;
	WhiteBattleTime -= DeductedTime;

	OnBattleTimeChanged.Broadcast(WhiteBattleTime, BlackBattleTime);
}

void AChessGameState::Multicast_BattleResult_Implementation(int32 WinTeam, float PlusTime)
{

	AMyChessPlayerController* PC =
		Cast<AMyChessPlayerController>(
			UGameplayStatics::GetPlayerController(this, 0));

	if (!PC) return;

	PC->ShowBattleResult(WinTeam, PlusTime);
}


void AChessGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessGameState, ChessBoard);
	DOREPLIFETIME(AChessGameState, ChessPieces);
	DOREPLIFETIME(AChessGameState, CurrentTurnTeam);
	DOREPLIFETIME(AChessGameState, WhiteRemainTime);
	DOREPLIFETIME(AChessGameState, BlackRemainTime);
	DOREPLIFETIME(AChessGameState, CurGamePhase);
	DOREPLIFETIME(AChessGameState, BlackBattleTime);
	DOREPLIFETIME(AChessGameState, WhiteBattleTime);
}
