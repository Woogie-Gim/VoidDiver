#include "DiverGameMode.h"
#include "DiverCharacter.h"
#include "Kismet/GameplayStatics.h"

ADiverGameMode::ADiverGameMode()
{
	DefaultPawnClass = ADiverCharacter::StaticClass();
	PrimaryActorTick.bCanEverTick = true; // 점수 누적 위해
}

void ADiverGameMode::BeginPlay()
{
	Super::BeginPlay();
	SurvivalTime = 0.0f;
	CurrentScore = 0;
}

void ADiverGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 게임오버면 점수 누적 중단
	if (bIsGameOver) return;

	// 생존 시간 누적 → 점수로 환산
	SurvivalTime += DeltaTime;
	CurrentScore = FMath::FloorToInt(SurvivalTime * ScorePerSecond);
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

	OnGameOverBP();
}