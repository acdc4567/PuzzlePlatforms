// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/BlasterGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSessionSettings.h"



const static FName SESSION_NAME = TEXT("My Session Game");

UBlasterGameInstance::UBlasterGameInstance(const FObjectInitializer& ObjectInitializer) {

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;




}

void UBlasterGameInstance::Init() {
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBlasterGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBlasterGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBlasterGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UBlasterGameInstance::OnJoinSessionComplete);
			
			
		
		
		}


	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem "));

	}






}

void UBlasterGameInstance::LoadMenuWidget() {

	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);


}

void UBlasterGameInstance::Host() {
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession();
		}


	}
	


}

void UBlasterGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could Not Create Session "));
		return;
	}
	

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");


}

void UBlasterGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}


}

void UBlasterGameInstance::OnFindSessionsComplete(bool bSuccess) {
	if (bSuccess && SessionSearch.IsValid() && Menu != nullptr) {

		TArray<FString> ServerNames;


		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
			//UE_LOG(LogTemp, Warning, TEXT("FoundSessionNamed : %s"), *SearchResult.GetSessionIdStr());
			ServerNames.Add(SearchResult.GetSessionIdStr());
		}

		Menu->SetServerList(ServerNames);

	}

}

void UBlasterGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid())return;
	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could Not GetConnectString"));
		return;
	}


	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);


}

void UBlasterGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = 0;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bAllowJoinInProgress = 1;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bShouldAdvertise = 1;
		SessionSettings.bUsesPresence = 1;
		SessionSettings.BuildUniqueId = 1;
		SessionSettings.bUseLobbiesIfAvailable = 1;
		
		
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);

	}
	

}

void UBlasterGameInstance::Join(uint32 Index) {
	
	if (!SessionInterface.IsValid())return;
	if (!SessionSearch.IsValid())return;

	if (Menu != nullptr) {
		//Menu->SetServerList({ "Test1","Test2" });
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);









}

void UBlasterGameInstance::LoadMainMenu() {

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/ThirdPerson/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UBlasterGameInstance::RefreshServerList() {

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Starting To Find Sessions"));
		//SessionSearch->bIsLanQuery = 1;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, 1, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}

}























