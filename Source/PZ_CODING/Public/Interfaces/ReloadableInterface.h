#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReloadableInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UReloadableInterface : public UInterface
{
	GENERATED_BODY()
};


class PZ_CODING_API IReloadableInterface
{
	GENERATED_BODY()
	
public:

	virtual bool CanReload() const = 0;  // обязывает реализовать проверку на возможность перезарядки оружия

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Reload(); // обязывает реализовать перезарядку
	
};
