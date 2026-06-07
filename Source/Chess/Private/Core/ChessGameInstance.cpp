// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ChessGameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Online/OnlineSessionNames.h"

#define SESSION_NAME NAME_GameSession

void UChessGameInstance::Init()
{
    Super::Init();

    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get(TEXT("STEAM")))
    {
        SessionInterface = Subsystem->GetSessionInterface();
    }

    if (SessionInterface.IsValid())
    {
        BindSessionDelegates();

        // 데디케이트 용 코드
        if (IsDedicatedServerInstance())
        {
            CreateDedicatedSession();
        }
    }
}

void UChessGameInstance::Shutdown()
{
   // UnbindSessionDelegates();
    Super::Shutdown();
}

void UChessGameInstance::BindSessionDelegates()
{
    if (!SessionInterface.IsValid()) return;

    if (!CreateSessionDelegateHandle.IsValid())
        CreateSessionDelegateHandle = SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UChessGameInstance::OnCreateSessionComplete);

    if (!FindSessionsDelegateHandle.IsValid())
        FindSessionsDelegateHandle = SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UChessGameInstance::OnFindSessionsComplete);

    if (!JoinSessionDelegateHandle.IsValid())
        JoinSessionDelegateHandle = SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UChessGameInstance::OnJoinSessionComplete);

    if (!DestroySessionDelegateHandle.IsValid())
        DestroySessionDelegateHandle = SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UChessGameInstance::OnDestroySessionComplete);

    // [추가] 친구 초대 수락을 감지하는 델리게이트 바인딩
    if (!OnSessionUserInviteAcceptedDelegateHandle.IsValid())
        OnSessionUserInviteAcceptedDelegateHandle = SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UChessGameInstance::OnSessionUserInviteAccepted);
}

void UChessGameInstance::UnbindSessionDelegates()
{
    if (!SessionInterface.IsValid()) return;

    SessionInterface->OnCreateSessionCompleteDelegates.Remove(CreateSessionDelegateHandle);
    SessionInterface->OnFindSessionsCompleteDelegates.Remove(FindSessionsDelegateHandle);
    SessionInterface->OnJoinSessionCompleteDelegates.Remove(JoinSessionDelegateHandle);
    SessionInterface->OnDestroySessionCompleteDelegates.Remove(DestroySessionDelegateHandle);

    // [추가] 종료 시 초대 델리게이트 바인딩 해제
    SessionInterface->OnSessionUserInviteAcceptedDelegates.Remove(OnSessionUserInviteAcceptedDelegateHandle);
}

void UChessGameInstance::HostSession()
{
   if (!SessionInterface.IsValid()) return;

    if (SessionInterface->GetNamedSession(SESSION_NAME))
    {
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }
    CreateSessionInternal();
}

void UChessGameInstance::CreateSessionInternal()
{
    FOnlineSessionSettings Settings;
    Settings.NumPublicConnections = 2;
    Settings.bShouldAdvertise = true;
    Settings.bUsesPresence = true;
    Settings.bAllowJoinInProgress = true;
    Settings.bAllowInvites = true;
    Settings.bUseLobbiesIfAvailable = true;
    Settings.bIsLANMatch = false;

    Settings.bAllowJoinViaPresence = true;

    Settings.BuildUniqueId = 20260602;

    Settings.Set(FName("GAME_ID"), FString("ChessGame_2026"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    SessionInterface->CreateSession(0, SESSION_NAME, Settings);
}

void UChessGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (!bWasSuccessful) return;

    if (IsDedicatedServerInstance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Dedicated Session Created"));
        return;
    }

    if (UWorld* World = GetWorld())
    {
        World->ServerTravel(TEXT("/Game/Chess/Lobby/Lobby?listen"));
    }

    UE_LOG(LogTemp, Warning,
        TEXT("CreateSession Success = %s"),
        bWasSuccessful ? TEXT("TRUE") : TEXT("FALSE"));
}

void UChessGameInstance::FindSessions()
{
   /* if (!bFindDedicated)
    {
        SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
    }*/

    if (!SessionInterface.IsValid()) return;

    SessionSearch = MakeShared<FOnlineSessionSearch>();
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->bIsLanQuery = false;

    // 데디케이트 인경우 제거
    /*SessionSearch->QuerySettings.Set(
        SEARCH_LOBBIES,
        true,
        EOnlineComparisonOp::Equals);*/

    SessionSearch->QuerySettings.Set(
        FName("GAME_ID"),
        FString("ChessGame_2026"),
        EOnlineComparisonOp::Equals);

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UChessGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Find Complete: %s"),
        bWasSuccessful ? TEXT("TRUE") : TEXT("FALSE"));

    if (!SessionSearch.IsValid()) return;

    UE_LOG(LogTemp, Warning, TEXT("Result Count: %d"),
        SessionSearch->SearchResults.Num());

    for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
    {
        FString GameId;
        SessionSearch->SearchResults[i].Session.SessionSettings.Get(FName("GAME_ID"), GameId);

        UE_LOG(LogTemp, Warning, TEXT("[%d] GAME_ID=%s"), i, *GameId);
    }

    if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
    {
        JoinSessionByIndex(0);
    }
}

void UChessGameInstance::JoinSessionByIndex(int32 SessionIndex)
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;
    if (!SessionSearch->SearchResults.IsValidIndex(SessionIndex)) return;

    const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[SessionIndex];
    SessionInterface->JoinSession(0, SESSION_NAME, Result);
}

void UChessGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result != EOnJoinSessionCompleteResult::Success) return;

    FString ConnectString;
    if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            PC->ClientTravel(ConnectString, TRAVEL_Absolute);
        }
    }
}

void UChessGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (!bWasSuccessful)
    {
        return;
    }

    if (IsDedicatedServerInstance())
    {
        CreateDedicatedSessionInternal();
    }
    else
    {
        CreateSessionInternal();
    }
}

// [추가] 친구가 Steam에서 초대를 수락했을 때 처리되는 실제 로직
void UChessGameInstance::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
    if (!bWasSuccessful || !SessionInterface.IsValid()) return;

    // 전달받은 호스트의 세션 정보(InviteResult)로 참가를 요청합니다.
    SessionInterface->JoinSession(ControllerId, SESSION_NAME, InviteResult);
}

void UChessGameInstance::CreateDedicatedSession()
{
    if (!SessionInterface.IsValid()) return;

    if (SessionInterface->GetNamedSession(SESSION_NAME))
    {
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }

    CreateDedicatedSessionInternal();
}

void UChessGameInstance::CreateDedicatedSessionInternal()
{
    FOnlineSessionSettings Settings;
    Settings.NumPublicConnections = 2;
    Settings.bShouldAdvertise = true;
    //Settings.bUsesPresence = true;
    Settings.bAllowJoinInProgress = true;
    Settings.bAllowInvites = true;
    //Settings.bUseLobbiesIfAvailable = true;
    Settings.bIsLANMatch = false;
    //Settings.bAllowJoinViaPresence = true;
    
    //데디케이트용
    Settings.bIsDedicated = true;
    Settings.bUsesPresence = false;
    Settings.bUseLobbiesIfAvailable = false;
    Settings.bAllowJoinViaPresence = false;


    Settings.BuildUniqueId = 20260602;

    Settings.Set(FName("GAME_ID"), FString("ChessGame_2026"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    SessionInterface->CreateSession(0, SESSION_NAME, Settings);
}

