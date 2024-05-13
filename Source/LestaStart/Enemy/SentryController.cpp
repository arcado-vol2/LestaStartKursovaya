#include "SentryController.h"
#include "Sentry.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ASentryController::ASentryController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerciption"));

    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1000.0f;
    SightConfig->PeripheralVisionAngleDegrees = MaxSightAngle;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASentryController::OnPerceptionTargetUpdate);
}

void ASentryController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ASentry* SentryPawn = Cast<ASentry>(InPawn);
    if (SentryPawn)
    {
        CurrentPawn = SentryPawn;
        CurrentPawn->SetRotationAngle(MaxSightAngle);
    }
    else
    {
        UE_LOG(LogInput, Error, TEXT("Failed to possess ASentry pawn!"));
    }
    
}


void ASentryController::BeginPlay()
{
    Super::BeginPlay();
    CurrentPawn = Cast<ASentry>(GetPawn());
    if (CurrentPawn)
    {
        CurrentPawn->SetRotationAngle(MaxSightAngle);
    }
}

void ASentryController::OnPerceptionTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>() || !CurrentPawn)
    {
        return;
    }
    if (Stimulus.WasSuccessfullySensed())
    {
        WeakTargets.Add(Actor);
        CurrentPawn->SetCurrentState(ESentryStates::SentryCombat);
        CurrentPawn->SetCurrentTarget(GetClosestTarget());
    }
    else
    {
        RemoveTarget(Actor);
    }
}

AActor* ASentryController::GetClosestTarget()
{
    AActor* BestTarget = nullptr;
    float BestDistanceSquared = BIG_NUMBER;

    for (TWeakObjectPtr<AActor>& WeakTarget : WeakTargets)
    {
        if (WeakTarget.IsValid())
        {
            float DistanceSquared = FVector::DistSquared(WeakTarget->GetActorLocation(), GetPawn()->GetActorLocation());
            if (DistanceSquared < BestDistanceSquared)
            {
                BestDistanceSquared = DistanceSquared;
                BestTarget = WeakTarget.Get();
            }
        }
    }
    return BestTarget;
}

void ASentryController::RemoveTarget(AActor* Target)
{
    WeakTargets.Remove(Target);

    if (WeakTargets.Num() == 0)
    {
        CurrentPawn->SetCurrentTarget(nullptr);
        CurrentPawn->SetCurrentState(ESentryStates::SentryIdle);
    }
    else if (CurrentPawn->GetCurrentTarget() == Target)
    {
        CurrentPawn->SetCurrentTarget(GetClosestTarget());
    }
}
