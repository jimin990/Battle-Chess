// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DadicateInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Online/OnlineSessionNames.h"

#define SESSION_NAME NAME_GameSession

void UDadicateInstance::Init()
{
    Super::Init();

    if (!IsDedicatedServerInstance())
    {
        return;
    }


    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get(TEXT("STEAM")))
    {
        SessionInterface = Subsystem->GetSessionInterface();
    }

    if (SessionInterface.IsValid())
    {
        BindSessionDelegates();
    }
}

void UDadicateInstance::Shutdown()
{
    Super::Shutdown();
}

void UDadicateInstance::BindSessionDelegates()
{
    if (!SessionInterface.IsValid()) return;

    if (!CreateSessionDelegateHandle.IsValid())
        CreateSessionDelegateHandle = SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDadicateInstance::OnCreateSessionComplete);

    if (!FindSessionsDelegateHandle.IsValid())
        FindSessionsDelegateHandle = SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UDadicateInstance::OnFindSessionsComplete);

    if (!JoinSessionDelegateHandle.IsValid())
        JoinSessionDelegateHandle = SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UDadicateInstance::OnJoinSessionComplete);

    if (!DestroySessionDelegateHandle.IsValid())
        DestroySessionDelegateHandle = SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UDadicateInstance::OnDestroySessionComplete);

    // [추가] 친구 초대 수락을 감지하는 델리게이트 바인딩
    if (!OnSessionUserInviteAcceptedDelegateHandle.IsValid())
        OnSessionUserInviteAcceptedDelegateHandle = SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UDadicateInstance::OnSessionUserInviteAccepted);
}

void UDadicateInstance::UnbindSessionDelegates()
{
    if (!SessionInterface.IsValid()) return;

    SessionInterface->OnCreateSessionCompleteDelegates.Remove(CreateSessionDelegateHandle);
    SessionInterface->OnFindSessionsCompleteDelegates.Remove(FindSessionsDelegateHandle);
    SessionInterface->OnJoinSessionCompleteDelegates.Remove(JoinSessionDelegateHandle);
    SessionInterface->OnDestroySessionCompleteDelegates.Remove(DestroySessionDelegateHandle);

    // [추가] 종료 시 초대 델리게이트 바인딩 해제
    SessionInterface->OnSessionUserInviteAcceptedDelegates.Remove(OnSessionUserInviteAcceptedDelegateHandle);
}

void UDadicateInstance::HostSession()
{
    if (!SessionInterface.IsValid()) return;

    if (SessionInterface->GetNamedSession(SESSION_NAME))
    {
        SessionInterface->DestroySession(SESSION_NAME);
        return;
    }
    CreateSessionInternal();
}

void UDadicateInstance::CreateSessionInternal()
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

void UDadicateInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (!bWasSuccessful) return;

    if (UWorld* World = GetWorld())
    {
        World->ServerTravel(TEXT("/Game/Chess/Lobby/Lobby?listen"));
    }

    UE_LOG(LogTemp, Warning,
        TEXT("CreateSession Success = %s"),
        bWasSuccessful ? TEXT("TRUE") : TEXT("FALSE"));
}

void UDadicateInstance::FindSessions()
{
    if (!SessionInterface.IsValid()) return;

    SessionSearch = MakeShared<FOnlineSessionSearch>();
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->bIsLanQuery = false;

    SessionSearch->QuerySettings.Set(
        SEARCH_LOBBIES,
        true,
        EOnlineComparisonOp::Equals);

    SessionSearch->QuerySettings.Set(
        FName("GAME_ID"),
        FString("ChessGame_2026"),
        EOnlineComparisonOp::Equals);

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UDadicateInstance::OnFindSessionsComplete(bool bWasSuccessful)
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

void UDadicateInstance::JoinSessionByIndex(int32 SessionIndex)
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;
    if (!SessionSearch->SearchResults.IsValidIndex(SessionIndex)) return;

    const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[SessionIndex];
    SessionInterface->JoinSession(0, SESSION_NAME, Result);
}

void UDadicateInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
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

void UDadicateInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        CreateSessionInternal();
    }
}

// [추가] 친구가 Steam에서 초대를 수락했을 때 처리되는 실제 로직
void UDadicateInstance::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
    if (!bWasSuccessful || !SessionInterface.IsValid()) return;

    // 전달받은 호스트의 세션 정보(InviteResult)로 참가를 요청합니다.
    SessionInterface->JoinSession(ControllerId, SESSION_NAME, InviteResult);
}

