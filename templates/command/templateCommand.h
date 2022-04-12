// Copyright (c) 2009-2022 FIRST and other WPILib contributors
// All rights reserved.

#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

/**
 * An example command.
 *
 * <p>Note that this extends CommandHelper, rather extending CommandBase
 * directly; this is crucially important, or else the decorator functions in
 * Command will *not* work!
 */
class ReplaceMeCommand2
    : public frc2::CommandHelper<frc2::CommandBase, ReplaceMeCommand2> {
 public:
  ReplaceMeCommand2();

  void Initialize() override;

  void Execute() override;

  void End(bool interrupted) override;

  bool IsFinished() override;
};
