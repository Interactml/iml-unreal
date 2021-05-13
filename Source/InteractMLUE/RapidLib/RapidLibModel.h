// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RapidLibLinkerDll.h"

class INTERACTMLUE_API FRapidLibModel
{


public:
    void Run(double inputs[], double PredictedOutputs[]);
    //string Run()
    bool StopRunning();
};

