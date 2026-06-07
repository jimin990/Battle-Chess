// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "ChessGameInstance.generated.h"

UCLASS()
class CHESS_API UChessGameInstance : public UGameInstance
{
    GENERATED_BODY()

    
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    UFUNCTION(BlueprintCallable, Category = "Session")
    void HostSession();

    UFUNCTION(BlueprintCallable, Category = "Session")
    void FindSessions();

    UFUNCTION(BlueprintCallable, Category = "Session")
    void JoinSessionByIndex(int32 SessionIndex);
    
protected:
    IOnlineSessionPtr SessionInterface;

    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    FDelegateHandle CreateSessionDelegateHandle;
    FDelegateHandle FindSessionsDelegateHandle;
    FDelegateHandle JoinSessionDelegateHandle;
    FDelegateHandle DestroySessionDelegateHandle;

    void BindSessionDelegates();
   void UnbindSessionDelegates();
    void CreateSessionInternal();

    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    // 초대를 수락했을 때를 위한 델리게이트 핸들
    FDelegateHandle OnSessionUserInviteAcceptedDelegateHandle;

    // 초대를 수락했을 때 실행될 콜백 함수
    void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);

    void CreateDedicatedSession();

    void CreateDedicatedSessionInternal();

    //bool bFindDedicated = true;
};