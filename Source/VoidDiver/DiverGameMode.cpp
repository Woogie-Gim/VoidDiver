#include "DiverGameMode.h"
#include "DiverCharacter.h"
#include "Kismet/GameplayStatics.h"

ADiverGameMode::ADiverGameMode()
{
	DefaultPawnClass = ADiverCharacter::StaticClass();
}

void ADiverGameMode::OnGameOver()
{
	// 이미 게임오버면 중복 처리 방지
	if (bIsGameOver) return;

	bIsGameOver = true;

	// 시간을 거의 멈춤 (0으로 하면 문제 생길 수 있어 아주 작은 값)
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0001f);

	// 플레이어 입력 차단
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		DisableInput(PC); // GameMode에서 입력 비활성화
	}

	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("GAME OVER"));
	}
}