#include "JSONBuilder.h"

// Include all view UIState classes
#include "TankController.h"
#include "UIState/SeeDeviceAddress.h"
#include "UIState/SeeDeviceUptime.h"
#include "UIState/SeeFreeMemory.h"
#include "UIState/SeeGoogleMins.h"
#include "UIState/SeeLogFile.h"
#include "UIState/SeePHSlope.h"
#include "UIState/SeePIDConstants.h"
#include "UIState/SeeTankID.h"
#include "UIState/SeeVersion.h"

int JSONBuilder::buildCurrentValues() {
  // Returns pointer to buffer. Will be null-terminated.
  TankController* tc = TankController::instance();
  buffer[bytes++] = '{';
  bytes += SeeDeviceAddress(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeeDeviceAddress(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 1);
  buffer[bytes++] = ',';
  bytes += SeeFreeMemory(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeeGoogleMins(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeeLogFile(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeePHSlope(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeePIDConstants(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeePIDConstants(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 1);
  buffer[bytes++] = ',';
  bytes += SeePIDConstants(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 2);
  buffer[bytes++] = ',';
  bytes += SeePIDConstants(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 3);
  buffer[bytes++] = ',';
  bytes += SeeTankID(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeeDeviceUptime(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes++] = ',';
  bytes += SeeVersion(tc).writeToBuffer(buffer + bytes, BUFFER_SIZE - bytes, 0);
  buffer[bytes] = '}';  // Replace null terminator from previous call
  buffer[++bytes] = '\0';
  return bytes;
}
