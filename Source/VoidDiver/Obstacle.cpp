#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "DiverCharacter.h"
#include "DiverGameMode.h"
#include "Kismet/GameplayStatics.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시를 루트로 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	// 콜리전 활성화 (쿼리 = 오버랩/트레이스 감지용)
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 기본은 모두 무시, Pawn 채널만 오버랩
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 오버랩 이벤트 생성 켜기 (이게 꺼져 있으면 이벤트가 안 옴)
	MeshComp->SetGenerateOverlapEvents(true);

	// 메시의 오버랩 시작 이벤트에 함수 연결
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnObstacleOverlap);
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임 아래로 이동. DeltaTime 곱해 프레임 독립적으로
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= FallSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	// 너무 아래로 가면 제거
	if (NewLocation.Z < KillZ)
	{
		Deactivate();
	}
}

void AObstacle::Activate(const FVector& NewLocation)
{
	bIsActive = true;
	SetActorLocation(NewLocation);
	SetActorHiddenInGame(false);   // 화면에 보이게
	SetActorEnableCollision(true); // 충돌 켜기 (나중에 충돌 판정용)
	SetActorTickEnabled(true);     // Tick(하강) 켜기
}

void AObstacle::Deactivate()
{
	bIsActive = false;
	SetActorHiddenInGame(true);    // 화면에서 숨김
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);    // Tick 끄기 (성능 절약)
}

void AObstacle::OnObstacleOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// 비활성(풀에 반납된) 장애물은 무시
	if (!bIsActive) return;

	// 자기 자신이나 null과의 겹침 무시
	if (OtherActor == nullptr || OtherActor == this) return;

	if (OtherActor->IsA(ADiverCharacter::StaticClass()))
	{
		// GameMode를 가져와 게임오버 요청
		if (ADiverGameMode* GM = Cast<ADiverGameMode>(
			UGameplayStatics::GetGameMode(GetWorld())))
		{
			GM->OnGameOver();
		}
	}
}