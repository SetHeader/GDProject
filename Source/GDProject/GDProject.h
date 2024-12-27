// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleLogger.h"

#define ECC_Projectile ECC_GameTraceChannel1
#define ECC_Target ECC_GameTraceChannel2
#define ECC_ExcludePlayers ECC_GameTraceChannel2

using GDLog = SimpleLogger;

#define GDLOG(Category, Format, ...) SimpleLogger::LogStatic(Category, Format, ##__VA_ARGS__);
#define GDLOG_E(Category, Format, ...) SimpleLogger::ErrorStatic(Category, Format, ##__VA_ARGS__);
#define GDLOG_W(Category, Format, ...) SimpleLogger::WarningStatic(Category, Format, ##__VA_ARGS__);
