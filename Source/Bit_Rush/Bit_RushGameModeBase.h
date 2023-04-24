// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "Bit_RushGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BIT_RUSH_API ABit_RushGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool HasConnectedSuccessfully);
	
	const FString DiscordWebhookUrl = "https://discord.com/api/webhooks/1099076835972763668/ASd4IyyHIyKyTQoq2DKmRfXsecirkcuBAW6j7UjmugnAiDvSgX5zSa3HD2GaonpU8EKb";
};
