// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Players/MainMenuController.h"
#include "UI/MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ChessGameInstance.h"

AMainMenuController::AMainMenuController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMainMenuController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsLocalController())
    {
        return;
    }

    // 메인 메뉴 위젯 생성 및 뷰포트에 추가
    if (MainWidgetClass)
    {
        MainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);
        if (MainWidget)
        {
            MainWidget->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[MainMenu] MainWidget "));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[MainMenu] MainWidgetClass not"));
    }
}

void AMainMenuController::HostGame()
{
    // 로비로 갈 때는 Async Loading Screen 강제로 OFF
    UGameplayStatics::OpenLevel(GetWorld(), "/Game/Chess/Lobby/Lobby", true, "listen");

    
    /*UE_LOG(LogTemp, Log, TEXT("Host"));
    UChessGameInstance* GI = GetGameInstance<UChessGameInstance>();
    if (GI)
    {
        UE_LOG(LogTemp, Log, TEXT("[MainMenu] HostGame call"));
        GI->HostSession();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[MainMenu] ChessGameInstance not."));
    }*/
    
}

void AMainMenuController::JoinGame()
{
    // 로비로 갈 때는 Async Loading Screen 강제로 OFF
    //UAsyncLoadingScreenLibrary::SetEnableLoadingScreen(false);
    ClientTravel("127.0.0.1", TRAVEL_Absolute);

    
    //UE_LOG(LogTemp, Log, TEXT("Join"));
    //UChessGameInstance* GI = GetGameInstance<UChessGameInstance>();
    //if (GI)
    //{
    //    UE_LOG(LogTemp, Log, TEXT("[MainMenu] JoinGame call - Sessions find"));
    //    GI->FindSessions();   // ← 이전에 만든 FindSessions()로 수정
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("[MainMenu] ChessGameInstance not."));
    //}
    
}