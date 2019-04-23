// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "GenericJoystickComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOYZTICK_API UGenericJoystickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGenericJoystickComponent();

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool WasInitialized() const { return bInitialized; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Joystick Config")
		bool bTriggerAxisMoveWhenAxisEqualsZero = false;

//Events
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoystickKeyChange, int, Button);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoystickMove, FVector2D, Axis);

	UPROPERTY(BlueprintAssignable)
		FJoystickKeyChange OnKeyPress;

	UPROPERTY(BlueprintAssignable)
		FJoystickKeyChange OnKeyDown;

	UPROPERTY(BlueprintAssignable)
		FJoystickKeyChange OnKeyRelease;

	UPROPERTY(BlueprintAssignable)
	FJoystickMove OnJoystickMove;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CreateWindowClass();
	void InitButtonsMap();
	static LRESULT CALLBACK WndProcGlobal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//Windows Callbacks & stuff
private:
	static bool InitInstance();
	static bool IsNotZero(FVector2D inVector);

	ATOM RegisterWindowClass();
	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintPure)
		static FORCEINLINE  TArray<UGenericJoystickComponent*> GetRegisteredComponents() { return RegisteredComponents; };

	UFUNCTION(BlueprintPure)
		static void GetButtonsByMask(int Button, TMap<int,int> inHashTable, TArray<int>& ButtonsOut);

	UFUNCTION(BlueprintPure)
		static FVector2D NormalizeJoyInput(FVector2D Input);


	void CheckReleasedButtons();

protected:
	bool bStopThread;
	MSG msg;

protected:
	static HWND hWnd;
	static HINSTANCE hInstance;
	static bool bInitialized;
	static TArray<UGenericJoystickComponent*> RegisteredComponents;
	static TMap<int, int> ButtonsMap;
	static TMap<int, int> ButtonsReleaseMap;

	TArray<int> ButtonsPressedLastFrame;
	TArray<int> ButtonsPressedThisFrame;

};