// DiverCharacter.cpp
#include "DiverCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ADiverCharacter::ADiverCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암 생성 후 루트에 부착
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = BoomLength;
	CameraBoom->bUsePawnControlRotation = false; // 낙하게임: 마우스로 카메라 안 돌림, 고정 시점
	CameraBoom->bEnableCameraLag = true;         // 부드러운 추적
	CameraBoom->CameraLagSpeed = 5.0f;
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, 0.0f, 0.0f)); // 아래로 내려다봄

	// 카메라를 스프링암 끝에 부착
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// CharacterMovementComponent 접근
	GetCharacterMovement()->GravityScale = 2.0f;        // 중력 배수. 높을수록 빨리 떨어짐
	GetCharacterMovement()->AirControl = 1.0f;          // 공중에서 100% 조작 가능
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;      // 좌우 이동 속도
	GetCharacterMovement()->BrakingDecelerationFalling = 0.0f; // 공중에서 관성 유지

	// 컨트롤러 회전이 캐릭터에 영향 안 주게 잠금. 낙하 시점 고정
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}

void ADiverCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 로컬 플레이어의 EIS 서브시스템에 기본 매핑 컨텍스트 등록
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ADiverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EIS 전용 컴포넌트로 캐스팅
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// MoveAction이 Triggered 상태일 때 Move 함수 호출
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADiverCharacter::Move);
		}
	}
}

void ADiverCharacter::Move(const FInputActionValue& Value)
{
	// Axis2D 입력을 Vector2D로 받음. X=좌우, Y=앞뒤
	const FVector2D Input = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 월드 기준 단순 이동. 낙하 게임이라 카메라 회전과 무관하게 축 고정
		AddMovementInput(FVector::RightVector, Input.X * MoveScale);   // 좌우
		AddMovementInput(FVector::ForwardVector, Input.Y * MoveScale); // 앞뒤
	}
}