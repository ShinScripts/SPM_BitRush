// Copyright Epic Games, Inc. All Rights Reserved.


#include "Bit_RushGameModeBase.h"


void ABit_RushGameModeBase::StartPlay()
{
	Super::StartPlay();

	// const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	// Post request
	// const TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	// RequestObj->SetStringField("content", "cpp test");
	//
	// FString RequestBody;
	// const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	//
	// FJsonSerializer::Serialize(RequestObj, Writer);
	//
	// Request->SetURL(DiscordWebhookUrl);
	// Request->SetVerb("POST");
	// Request->SetHeader("Content-Type", "application/json");
	// Request->SetContentAsString(RequestBody);

	// Get request
	// Request->SetURL("https://reqres.in/api/users/2");
	// Request->SetVerb("GET");
	//
	// Request->OnProcessRequestComplete().BindUObject(this, &ABit_RushGameModeBase::OnResponseReceived);
	// Request->ProcessRequest();
}

void ABit_RushGameModeBase::SendWebhook(const FString& Content)
{
	const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	
	const TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("content", Content);
	
	FString RequestBody;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	
	FJsonSerializer::Serialize(RequestObj, Writer);
	
	Request->SetURL(DiscordWebhookUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);

	// Request->OnProcessRequestComplete().BindUObject(this, &ABit_RushGameModeBase::OnResponseReceived);
	Request->ProcessRequest();
}

void ABit_RushGameModeBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool HasConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	FJsonSerializer::Deserialize(Reader, ResponseObj);
	
	UE_LOG(LogTemp, Warning, TEXT("Request status code: %d"), Response->GetResponseCode());
	UE_LOG(LogTemp, Warning, TEXT("Test request sent to 'https://reqres.in/api/users/2'. email: %s"), *ResponseObj->GetObjectField("data")->GetStringField("email"));
}
