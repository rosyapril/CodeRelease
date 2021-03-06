/**
 * @file Modules/Infrastructure/NaoProvider.h
 * The file declares a module that provides information from the Nao via DCM.
 * @author <a href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</a>
 */

#pragma once

#include "Platform/Linux/NaoBody.h"
#include "Representations/Configuration/JointCalibration.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/GameInfo.h"
#include "Representations/Infrastructure/JointAngles.h"
#include "Representations/Infrastructure/JointRequest.h"
#include "Representations/Infrastructure/LEDRequest.h"
#include "Representations/Infrastructure/RobotInfo.h"
#include "Representations/Infrastructure/TeamInfo.h"
#include "Representations/Infrastructure/USRequest.h"
#include "Representations/Infrastructure/SensorData/FsrSensorData.h"
#include "Representations/Infrastructure/SensorData/InertialSensorData.h"
#include "Representations/Infrastructure/SensorData/JointSensorData.h"
#include "Representations/Infrastructure/SensorData/KeyStates.h"
#include "Representations/Infrastructure/SensorData/SystemSensorData.h"
#include "Representations/Infrastructure/SensorData/UsSensorData.h"
#include "Tools/Module/Module.h"

MODULE(NaoProvider,
{,
  REQUIRES(JointCalibration),
  REQUIRES(LEDRequest),
  REQUIRES(USRequest),

  PROVIDES(FrameInfo),
  REQUIRES(FrameInfo),

  PROVIDES(FsrSensorData),
  PROVIDES(InertialSensorData),
  PROVIDES(JointSensorData),
  PROVIDES(KeyStates),
  PROVIDES(OpponentTeamInfo),
  PROVIDES(OwnTeamInfo),
  PROVIDES(RawGameInfo),
  PROVIDES(RobotInfo),
  PROVIDES(SystemSensorData),
  PROVIDES(UsSensorData),
  USES(JointRequest), // Will be accessed in send()
  LOADS_PARAMETERS(
  {,
    ((RobotInfo) NaoVersion) naoVersion, ///< V33, V4, V5 ...
    ((RobotInfo) NaoType) naoBodyType, ///< H21, H25 ...
    ((RobotInfo) NaoType) naoHeadType,
  }),
});

#ifdef TARGET_ROBOT

/**
 * @class NaoProvider
 * A module that provides information from the Nao.
 */
class NaoProvider : public NaoProviderBase
{
private:
  static PROCESS_LOCAL NaoProvider* theInstance; /**< The only instance of this module. */

  NaoBody naoBody;
  RoboCup::RoboCupGameControlData gameControlData; /**< The last game control data received. */
  unsigned gameControlTimeStamp; /**< The time when the last gameControlData was received (kind of). */
  float clippedLastFrame[Joints::numOfJoints]; /**< Array that indicates whether a certain joint value was clipped in the last frame (and what was the value)*/
  unsigned lastBodyTemperatureReadTime = 0;

public:
  NaoProvider();
  ~NaoProvider();

  /** The method is called by process Motion to send the requests to the Nao. */
  static void finishFrame();
  static void waitForFrameData();

private:
  void update(FrameInfo& frameInfo);
  void update(FsrSensorData& fsrSensorData);
  void update(InertialSensorData& inertialSensorData);
  void update(JointSensorData& jointSensorData);
  void update(KeyStates& keyStates);
  void update(OpponentTeamInfo& opponentTeamInfo);
  void update(OwnTeamInfo& ownTeamInfo);
  void update(RawGameInfo& rawGameInfo);
  void update(RobotInfo& robotInfo);
  void update(SystemSensorData& systemSensorData);
  void update(UsSensorData& usSensorData);

  /** The function sends a command to the Nao. */
  void send();
};

#else
//TARGET_ROBOT not defined here (Simulator).

class NaoProvider : public NaoProviderBase
{
private:
  void update(FrameInfo& frameInfo) {}
  void update(FsrSensorData& fsrSensorData) {}
  void update(InertialSensorData& inertialSensorData) {}
  void update(JointSensorData& jointSensorData) {}
  void update(KeyStates& keyStates) {}
  void update(OpponentTeamInfo& opponentTeamInfo) {}
  void update(OwnTeamInfo& ownTeamInfo) {}
  void update(RawGameInfo& rawGameInfo) {}
  void update(RobotInfo& robotInfo) {}
  void update(SystemSensorData& systemSensorData) {}
  void update(UsSensorData& usSensorData) {}
  void send();

public:
  static void finishFrame() {}
  static void waitForFrameData() {}
};

#endif
