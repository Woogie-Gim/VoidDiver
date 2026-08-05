#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiverGameMode.generated.h"

UCLASS()
class VOIDDIVER_API ADiverGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
	ADiverGameMode();

	// 게임오버 처리. 장애물이 충돌 시 호출
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnGameOver();

	// 현재 게임오버 상태인지
	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGameOver() const { return bIsGameOver; }

protected:
	// 게임오버 상태 플래그
	bool bIsGameOver = false;
};