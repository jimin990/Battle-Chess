// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Players/MyChessPlayerController.h"
#include "Character/Players/ChessCameraPawn.h"
#include "Character/ChessPiece/ChessPieceBase.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/NewChessGameMode.h"
#include "System/ChessBoard.h"
#include "Net/UnrealNetwork.h"
#include "System/ChessMoveFunc.h"
#include "EnhancedInputSubsystems.h"
#include "UI/TimeWidget.h"
#include "Core/MainGameMode.h"
#include "UI/ChessHUD.h"
#include "UI/BattleMatchupWidget.h"
#include "UI/FadeInOutWidget.h"
#include "UI/BattleHUD.h"
#include "UI/OpeningWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMyChessPlayerController::AMyChessPlayerController()
{
    isClickAvailable = false;
}

void AMyChessPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("____________________________________"));
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay() PC=%s Local=%d NetMode=%d"),
        *GetNameSafe(this),
        IsLocalController(),
        (int32)GetNetMode()
    );

    // 게임으로 들어갈때는 ON
    //UAsyncLoadingScreenLibrary::SetEnableLoadingScreen(true);

    //TryBindGameState();

    /*if (UWorld* World = GetWorld())
    {
        GameStateSetHandle = World->GameStateSetEvent.AddUObject(
            this,
            &AMyChessPlayerController::HandleGameStateSet
        );

        if (AChessGameState* CurrentGameState = Cast<AChessGameState>(World->GetGameState()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Cur_GS"));
            HandleGameStateSet(CurrentGameState);
        }
    }*/
}

void AMyChessPlayerController::PostSeamlessTravel()
{
    Super::PostSeamlessTravel();


    UE_LOG(LogTemp, Warning, TEXT("PostSeamlessTravel PC=%s Local=%d NetMode=%d"),
        *GetNameSafe(this),
        IsLocalController(),
        (int32)GetNetMode()
    );

    Client_InitAfterSeamlessTravel();
}

void AMyChessPlayerController::Client_InitAfterSeamlessTravel_Implementation()
{
    if (UWorld* World = GetWorld())
    {
        GameStateSetHandle = World->GameStateSetEvent.AddUObject(
            this,
            &AMyChessPlayerController::HandleGameStateSet
        );

        if (AChessGameState* CurrentGameState = Cast<AChessGameState>(World->GetGameState()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Cur_GS"));
            HandleGameStateSet(CurrentGameState);
        }
    }
}

void AMyChessPlayerController::TryBindGameState()
{
    AChessGameState* GS =
        GetWorld()->GetGameState<AChessGameState>();

    if (!GS)
    {
        UE_LOG(LogTemp, Warning, TEXT("GS is Not Ready"));
        /*GetWorldTimerManager().SetTimerForNextTick(
            this,
            &AMyChessPlayerController::TryBindGameState);*/

        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("GS Ready"));

    // 게임 페이즈 변경 델리게이트 연결
    GS->OnGamePhaseChanged.RemoveDynamic(
        this,
        &AMyChessPlayerController::GamePhaseChanged);

    GS->OnGamePhaseChanged.AddDynamic(
        this,
        &AMyChessPlayerController::GamePhaseChanged);

    // 위젯 연결
    SetWidget();
}

void AMyChessPlayerController::SetWidget()
{   
    //현재 조종하는 컨트롤러 인 경우만
    if (!IsLocalController())
    {
        return;
    }

    // 게임 시간 위젯
    if (!TimeWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeWidgetClass is null"));
        return;
    }

    TimerWidget = CreateWidget<UTimeWidget>(
        this,
        TimeWidgetClass);

    if (TimerWidget)
    {
        TimerWidget->AddToViewport();
        TimerWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    // 체스HUD 위젯 연결
    if (!ChessHUDClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChessHUDClass is null"));
        return;
    }

    ChessHUDWidget = CreateWidget<UChessHUD>(
        this,
        ChessHUDClass);

    if (ChessHUDWidget)
    {
        ChessHUDWidget->AddToViewport();
        ChessHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    // 페이드 인 아웃 위젯
    if (!FadeInOutWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("FadeInOutWidgetClass is null"));
        return;
    }

    FadeInOutWidget = CreateWidget<UFadeInOutWidget>(
        this,
        FadeInOutWidgetClass);

    if (FadeInOutWidget)
    {
        FadeInOutWidget->AddToViewport();
    }

    BattleHUD = CreateWidget<UBattleHUD>(
        this,
        BattleHUDClass);

    if (BattleHUD)
    {
        BattleHUD->AddToViewport();
        BattleHUD->SetVisibility(ESlateVisibility::Collapsed);
    }

    // OpeningWidget 위젯 연결
    if (!OpeningWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("OpeningWidgetClass is null"));
        return;
    }

    OpeningWidget = CreateWidget<UOpeningWidget>(
        this,
        OpeningWidgetClass);

    if (OpeningWidget)
    {
        OpeningWidget->AddToViewport();
    }

}

// 나중에 템플릿 함수로 코드 최적화
void AMyChessPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();
}

void AMyChessPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 마우스 아래 ISM에 있는 Piece 감지 UI


    Hover_ChessPiece();
    
}

void AMyChessPlayerController::Server_Ready_Implementation()
{
    ANewChessGameMode* GM =
        GetWorld()->GetAuthGameMode<ANewChessGameMode>();

    if (GM)
    {
        GM->ReadyPlayer();
    }
}

void AMyChessPlayerController::HandleGameStateSet(AGameStateBase* NewGameState)
{
    UE_LOG(LogTemp, Warning, TEXT("GS@!#!@#"));
    AChessGameState* ChessGS = Cast<AChessGameState>(NewGameState);
    if (!ChessGS) return;

    BindToChessGameState(ChessGS);
}

void AMyChessPlayerController::BindToChessGameState(AChessGameState* NewGameState)
{
    if (!NewGameState)
    {
        UE_LOG(LogTemp, Warning, TEXT("GS is Not Ready"));
        /*GetWorldTimerManager().SetTimerForNextTick(
            this,
            &AMyChessPlayerController::TryBindGameState);*/

        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("GS Ready PC=%s Local=%d NetMode=%d"),
        *GetNameSafe(this),
        IsLocalController(),
        (int32)GetNetMode()
    );

    //UE_LOG(LogTemp, Warning, TEXT("GS Ready"));

    // 게임 페이즈 변경 델리게이트 연결
    NewGameState->OnGamePhaseChanged.RemoveDynamic(
        this,
        &AMyChessPlayerController::GamePhaseChanged);

    NewGameState->OnGamePhaseChanged.AddUniqueDynamic(
        this,
        &AMyChessPlayerController::GamePhaseChanged);


    if (IsLocalController())
    {
        SetWidget();
        Server_Ready();
    }
}

void AMyChessPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void AMyChessPlayerController::ClickPiece()
{
    if (!isClickAvailable) return;

    FHitResult HitResult;

    GetHitResultUnderCursor(
        ECC_Visibility,
        false,
        HitResult
    );

    // 아무것도 안 맞음
    if (!HitResult.bBlockingHit)
    {
        return;
    }

    // 체스판 ISM만 허용
    UInstancedStaticMeshComponent* ISM =
        Cast<UInstancedStaticMeshComponent>(
            HitResult.Component
        );

    if (!ISM)
    {
        return;
    }

    // 클릭한 타일 번호
    int32 TileIndex = HitResult.Item;

    AChessGameState* GS =
        GetWorld()->GetGameState<AChessGameState>();

    if (!GS)
    {
        return;
    }

    // 해당 칸의 말
    AChessPieceBase* Piece =
        GS->ChessPieces[TileIndex];


    //현재 선택된 말이 없음
    if (!CurPiece)
    {
        // 빈칸 클릭
        if (!Piece)
        {
            return;
        }

        // 상대 말 클릭
        if (Piece->GetOwner() != this)
        {
            return;
        }

        CurPiece = Piece;

        if (ChessHUDWidget)
        {
            ChessHUDWidget->SetSelectedPiece(
                CurPiece
            );
        }

        SelectPiece(CurPiece);

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Select Piece : %d"),
            CurPiece->OwnIndex
        );

        return;
    }


    //현재 선택된 말이 있음
    // 같은 팀 말 클릭
    // -> 선택 변경
    if (Piece && Piece->GetOwner() == this)
    {
        CurPiece = Piece;

        if (ChessHUDWidget)
        {
            ChessHUDWidget->SetSelectedPiece(
                CurPiece
            );
        }

        SelectPiece(CurPiece);

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Change Selected Piece : %d"),
            CurPiece->OwnIndex
        );

        return;
    }

    //만약 현재 선택된 말이 있고 적 혹은 빈칸을 선택했다면,
    Server_MovePiece(
        CurPiece,
        TileIndex
    );

    CurPiece = nullptr;

    ReturnBoardColor();
}
void AMyChessPlayerController::ClickCancel()
{
    UE_LOG(LogTemp, Warning, TEXT("Cancel"));

    CurPiece = nullptr;

    if (ChessHUDWidget)
    {
        ChessHUDWidget->ClearSelectedPiece();
    }

    ReturnBoardColor();
}

void AMyChessPlayerController::SelectPiece(AChessPieceBase* Piece)
{
    ReturnBoardColor();
    //CurPiece = Piece;

    if (IsLocalController())
    {
        //UE_LOG(LogTemp, Warning, TEXT("SelectPiece %s"), *Piece->GetName());
        
        ShowPawnMoves(Piece);
    }
}

void AMyChessPlayerController::ShowPawnMoves(AChessPieceBase* Piece)
{
    AChessGameState* GS = Cast<AChessGameState>(GetWorld()->GetGameState());

    TArray<int32> Moves =
        UChessMoveFunc::GetPieceMoves(Piece, GS->ChessPieces);

    Board->VisibleHighlight(Moves);
}

void AMyChessPlayerController::ReturnBoardColor()
{
    Board->InvisibleHighlight();
}

void AMyChessPlayerController::AcknowledgePossession(APawn* P)
{
    Super::AcknowledgePossession(P);

    UE_LOG(LogTemp, Warning, TEXT("ACK PC=%s Pawn=%s Local=%d NetMode=%d"),
        *GetNameSafe(this),
        *GetNameSafe(P),
        IsLocalController(),
        (int32)GetNetMode()
    );

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = nullptr;
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    }

    if (Cast<AChessCameraPawn>(P))
    {
        /*UE_LOG(
            LogTemp,
            Warning,
            TEXT("AChessCameraPawn"));*/

        SetShowMouseCursor(true);

        SetInputMode(FInputModeGameAndUI());
        ResetIgnoreInputFlags();

        if (InputSubsystem)
        {
            InputSubsystem->ClearAllMappings();

            if (CameraIMC)
            {
                InputSubsystem->AddMappingContext(CameraIMC, 0);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("CameraIMC is null"));
            }
        }

        AChessCameraPawn* CP = Cast<AChessCameraPawn>(P);

        if (CP)
        {
            CP->PossessCon(this);

            //UE_LOG(LogTemp, Warning, TEXT("PossessCon %d"), CP->IsLocallyControlled());
        }
    }
    else
    {
        /*UE_LOG(
            LogTemp,
            Warning,
            TEXT("AChessPiecePawn"));*/

        SetShowMouseCursor(false);

        SetInputMode(FInputModeGameOnly());
        ResetIgnoreInputFlags();

        if (InputSubsystem)
        {
            InputSubsystem->ClearAllMappings();

            if (PieceIMC)
            {
                InputSubsystem->AddMappingContext(PieceIMC, 0);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("PieceIMC is null"));
            }
        }

        AChessPieceBase* CP = Cast<AChessPieceBase>(P);
        if (CP)
        {
            CP->PossessCon(this);
        }
    }
}

void AMyChessPlayerController::Hover_ChessPiece()
{
    FHitResult HitResult;

    bool bHit = GetHitResultUnderCursor(
        ECC_Visibility,
        false,
        HitResult
    );

    AChessPieceBase* NewHoverPiece = nullptr;

    if (bHit && HitResult.bBlockingHit)
    {
        UInstancedStaticMeshComponent* ISM =
            Cast<UInstancedStaticMeshComponent>(
                HitResult.Component
            );

        if (ISM)
        {
            AChessGameState* GS =
                GetWorld()->GetGameState<AChessGameState>();

            if (GS)
            {
                int32 TileIndex = HitResult.Item;

                if (GS->ChessPieces.IsValidIndex(TileIndex))
                {
                    NewHoverPiece =
                        GS->ChessPieces[TileIndex];
                }
            }
        }
    }

    // 같은 말을 계속 Hover 중이면 무시
    if (Cur_HoverChessPiece == NewHoverPiece)
    {
        return;
    }

    // Hover 대상 변경
    Cur_HoverChessPiece = NewHoverPiece;

    // Hover 해제
    if (!Cur_HoverChessPiece)
    {
        OnCurHoverPieceChanged.Broadcast(
            nullptr,
            false
        );

        return;
    }

    // Hover 시작 / 변경
    OnCurHoverPieceChanged.Broadcast(
        Cur_HoverChessPiece,
        Cur_HoverChessPiece->Team == PlayerTeam
    );
}
void AMyChessPlayerController::GamePhaseChanged(EGamePhase NewPhase)
{
    UEnum* EnumPtr = StaticEnum<EGamePhase>();

    FString Name = EnumPtr->GetNameStringByValue((int64)NewPhase);

    UE_LOG(LogTemp, Warning, TEXT("Game Changed to %s PC=%s Local=%d NetMode=%d"),
        *Name,
        *GetNameSafe(this),
        IsLocalController(),
        (int32)GetNetMode()
    );

    switch (NewPhase)
    {
    case EGamePhase::InitGamePhase:
        
        break;

    case EGamePhase::PreGamePhase:
        SetPreGamePhase();
        break;

    case EGamePhase::ChessPhase:
        UE_LOG(LogTemp, Warning, TEXT("ChessPhase!"));
        SetChessPhase();
        break;

    case EGamePhase::BettingPhase:
        SetBettingPhase();
        break;

    case EGamePhase::PreBattlePhase:
        SetPreBattlePhase();
        break;

    case EGamePhase::BattlePhase:
        SetBattlePhase();
        break;

    case EGamePhase::EndBattlePhase:
        SetEndBattlePhase();
        break;

    case EGamePhase::FinishBattlePhase:
        SetFinishBattlePhase();
        break;

    case EGamePhase::GameEndPhase:
        SetGameEndPhase();
        break;
    }
}

void AMyChessPlayerController::SetPreGamePhase()
{
    ShowChessGameUI();

    FadeIn();

    // 체스 말 소환 2초 = 예정

    //
    if (OpeningWidget)
    {
        OpeningWidget->PlayOpening();
    }
}

void AMyChessPlayerController::ShowChessGameUI()
{
    if (!IsLocalController())
    {
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("CNoT!"));

    if (TimerWidget && ChessHUDWidget)
    {
        TimerWidget->SetVisibility(ESlateVisibility::Visible);
        ChessHUDWidget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NoT!"));
    }
}

void AMyChessPlayerController::HideChessGameUI()
{
    if (!IsLocalController())
    {
        return;
    }

    if (TimerWidget && ChessHUDWidget)
    {
        TimerWidget->SetVisibility(ESlateVisibility::Hidden);
        ChessHUDWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMyChessPlayerController::FadeIn() 
{
    if (FadeInOutWidget)
    {
        FadeInOutWidget->PlayFadeIn();
    }
}

void AMyChessPlayerController::FadeOut()
{
    if (FadeInOutWidget)
    {
        FadeInOutWidget->PlayFadeOut();
    }
    
}

void AMyChessPlayerController::FadeInOut()
{
    if (FadeInOutWidget)
    {
        FadeInOutWidget->PlayFadeInOut();
    }

}

void AMyChessPlayerController::SetChessPhase()
{
    isClickAvailable = true;
}

void AMyChessPlayerController::SetBettingPhase()
{
    isClickAvailable = false;

    // 체스 UI 숨기기
    HideChessGameUI();

    // 배팅 UI 보이게
    ShowBettingUI();
}

void AMyChessPlayerController::ShowBettingUI()
{
    if (!IsLocalController())
    {
        return;
    }

    if (!BattleMatchupWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("BattleMatchupWidgetClass is null"));
        return;
    }

    BattleMatchupWidget = CreateWidget<UBattleMatchupWidget>(
        this,
        BattleMatchupWidgetClass);

    if (BattleMatchupWidget)
    {
        BattleMatchupWidget->AddToViewport();
        BattleMatchupWidget->PlayIntro();
    }

}

void AMyChessPlayerController::HideBettingUI()
{
    if (BattleMatchupWidget)
    {
        BattleMatchupWidget->RemoveFromViewport();
    }
}

void AMyChessPlayerController::SetPreBattlePhase()
{
    // 배팅 UI 숨기기
    HideBettingUI();

    // 배틀 UI 표시
    ShowBattleUI();

    //FadeInOut();

    
    // 배틀 스타트 UI ON 3초 카운트 다운
    StartBattleCounter();
}

void AMyChessPlayerController::ShowBattleUI()
{
    if (BattleHUD)
    {
        BattleHUD->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMyChessPlayerController::HideBattleUI()
{
    if (BattleHUD)
    {
        BattleHUD->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AMyChessPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AChessPieceBase* CP = Cast<AChessPieceBase>(InPawn);

    if (CP && BattleHUD)
    {
        BattleHUD->InitPiece(CP);
        //BattleHUD->SetVisibility(ESlateVisibility::Visible);

        CP->GetCharacterMovement()->SetMovementMode(MOVE_None);

        // 임시로 3초로 지정 - 동기화 문제
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, [this,CP]()
            {
                // 초기화 후 다음 단계로
                CP->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }, 3, false);

    }
}

void AMyChessPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    AChessPieceBase* CP = Cast<AChessPieceBase>(GetPawn());

    if (CP && BattleHUD)
    {
        BattleHUD->InitPiece(CP);
        //BattleHUD->SetVisibility(ESlateVisibility::Visible);
        CP->GetCharacterMovement()->SetMovementMode(MOVE_None);

        // 임시로 3초로 지정 - 동기화 문제
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, [this, CP]()
            {
                // 초기화 후 다음 단계로
                CP->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }, 3, false);
    }
}

void AMyChessPlayerController::SetBattlePhase()
{

}

void AMyChessPlayerController::StartBattleCounter()
{
    if (BattleHUD)
    {
        //BattleHUD
        BattleHUD->PlayBattleCounter();
    }
}

void AMyChessPlayerController::SetEndBattlePhase()
{
    HideBattleUI();
    // 승패 뛰우기
    FadeOut();
}

void AMyChessPlayerController::SetFinishBattlePhase()
{
    FadeIn();

    // 배팅 UI 숨기기
    HideBettingUI();
    // 승패 뛰운거 UI 제거
    ShowChessGameUI();
    // 점수 들어가는 연출

    // 여기서 2초 정도 승패 나옴

}

void AMyChessPlayerController::Client_FadeInOut_Implementation()
{
    if (FadeInOutWidget)
    {
        FadeInOutWidget->PlayFadeInOut();
    }
}

void AMyChessPlayerController::ShowBattleResult(int32 WinTeam, float PlusTime)
{
    if (ChessHUDWidget)
    {
        ChessHUDWidget->ViewBattleResult(WinTeam, PlusTime);
    }
}

void AMyChessPlayerController::SetGameEndPhase()
{
    // 배팅 UI 숨기기
    HideBettingUI();

    HideChessGameUI();

    if (OpeningWidget)
    {
        OpeningWidget->PlayEnd();
    }

    // 임시로 3초로 지정 - 동기화 문제
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
        }, 3, false);
}

void AMyChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyChessPlayerController, Board);

    DOREPLIFETIME(AMyChessPlayerController, PlayerTeam);
}

void AMyChessPlayerController::Server_MovePiece_Implementation(AChessPieceBase* Piece, int32 MoveIndex)
{
    if (!Piece) return;

    ANewChessGameMode* GM = GetWorld()->GetAuthGameMode<ANewChessGameMode>();

    if (ChessHUDWidget)
    {
        ChessHUDWidget->ClearSelectedPiece();
    }


    GM->TryMovePiece(Piece, this, MoveIndex);
}

void AMyChessPlayerController::Server_SetBattleTime_Implementation(float NewTime, EChessTeam CheesTeam)
{
    AChessGameState* GS = GetWorld()->GetGameState<AChessGameState>();

    GS->BettingBattleTime(NewTime, CheesTeam);
}
