// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/ServerRow.h"
#include "Components/Button.h"
#include "MenuSystem/MainMenu.h"

void UServerRow::Setup(UMainMenu* Parent, uint32 Index) {
	Parentx = Parent;
	Index1 = Index;
	RowButton->OnClicked.AddDynamic(this,&UServerRow::OnClicked);
}

void UServerRow::OnClicked() {

	Parentx->SelectIndex(Index1);

}
