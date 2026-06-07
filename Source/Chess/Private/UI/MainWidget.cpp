// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainWidget.h"
#include "Components/Button.h"
#include "Character/Players/MainMenuController.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HostButton->OnClicked.AddDynamic(this, &UMainWidget::HostGame);

    JoinButton->OnClicked.AddDynamic(this, &UMainWidget::JoinGame);
}

void UMainWidget::HostGame()
{
    auto PC =
        GetOwningPlayer<AMainMenuController>();

    if (PC)
    {
        PC->HostGame();
    }
}

void UMainWidget::JoinGame()
{
    auto PC =
        GetOwningPlayer<AMainMenuController>();

    if (PC)
    {
        PC->JoinGame();
    }
}


