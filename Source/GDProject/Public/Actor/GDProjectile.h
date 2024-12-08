// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GDAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "GDProjectile.generated.h"

class ICombatInterface;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class GDPROJECT_API AGDProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	// 持续性的音效，发射物生成后就自带
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<USoundBase> PersistentSound;

	// 碰撞时音效
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<USoundBase> ImpactSound;

	// 碰撞时粒子效果
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;
	
	// 跟踪指定位置
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float LifeSpan = 15.f;

	// 撞击时是否自动销毁
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	bool bDestroyedOnHit = true;
	// 撞击不自动销毁时，就可以用频率来控制多次伤害的间隔。单位秒。<=0表示只造成一次伤害。
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	float DamageFrequency = 0.f;
	
private:
	bool bHit = false;
	UPROPERTY()
	TMap<TObjectPtr<AActor>, FTimerHandle> OverlappingActorsToTimerHandle;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AGDProjectile();
	
	virtual void Destroyed() override;

	

protected:
	virtual void BeginPlay() override;

	void PlayHitEffect(FVector InLocation);

	void PlayHitEffect();

	void CauseDamage(AActor* TargetActor) const;
	// 如果在服务端就造成伤害，如果在客户端就播放击中效果
	void CauseDamageAndPlayHitEffect(AActor* TargetActor);

	void StartDamageTimer(AActor* TargetActor);

	void StopDamageTimer(AActor* TargetActor);
};
