// Copyright (c) 2009-2022 FIRST and other WPILib contributors
// All rights reserved.

#pragma once

#include <frc2/command/SubsystemBase.h>

class ReplaceMeSubsystem2 : public frc2::SubsystemBase {
 public:
  ReplaceMeSubsystem2();

  /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;

 private:
  // Components (e.g. motor controllers and sensors) should generally be
  // declared private and exposed only through public methods.
};
