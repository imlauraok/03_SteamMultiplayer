// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		//if (!ensure(IPAddressField != nullptr)) return;
		//const FString& Address = IPAddressField->GetText().ToString();
		//MenuInterface->Join(Address);
		UWorld* World = this->GetWorld();
		if (!ensure(World != nullptr)) return;

		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}