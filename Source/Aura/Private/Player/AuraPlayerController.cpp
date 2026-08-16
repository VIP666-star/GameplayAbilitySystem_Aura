// GameplayAbilitySystem Project Aura


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	
	Subsystem->AddMappingContext(AuraContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/**
	 *  鼠标悬停在不同Actor上时，拥有的所有情况
	 *  A、LastActor == nullptr && ThisActor == nullptr
	 *      -不做处理
	 *  B、LastActor == nullptr && ThisActor != nullptr
	 *      -ThisActor进行高亮
	 *  C、LastActor != nullptr && ThisActor == nullptr
	 *      -LastActor取消高亮
	 *  D、LastActor != nullptr && ThisActor != nullptr && LastActor == ThisActor
	 *      -不做处理
	 *  E、LastActor != nullptr && ThisActor != nullptr && LastActor != ThisActor
	*       -LastActor取消高亮
	 *      -ThisActor进行高亮
	 */
	
	if (LastActor == nullptr)
	{
		if (ThisActor == nullptr)
		{
			// Case A : 不做处理
		}
		else // ThisActor != nullptr
		{
			// Case B
			ThisActor->HighLightActor();
		}
	}
	else // LastActor != nullptr
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else // ThisActor != nullptr
		{
			if (LastActor == ThisActor)
			{
				// Case D : 不做处理
			}
			else // LastActor != ThisActor
			{
				// Case E
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
		}
	}
}
