#ifndef POINCARE_SERIALIZATION_HELPER_H
#define POINCARE_SERIALIZATION_HELPER_H

#include <poincare/serialization_helper_interface.h>

namespace Poincare {

namespace SerializationHelper {
  /* SerializableReference to Text */
  int Infix(
      const SerializationHelperInterface * interface,
      char * buffer,
      int bufferSize,
      Preferences::PrintFloatMode floatDisplayMode,
      int numberOfDigits,
      const char * operatorName,
      int firstChildIndex = 0,
      int lastChildIndex = -1);

  int Prefix(
      const SerializationHelperInterface * interface,
      char * buffer,
      int bufferSize,
      Preferences::PrintFloatMode floatDisplayMode,
      int numberOfDigits,
      const char * operatorName,
      bool writeFirstChild = true);

  /* Write one char in buffer */
  int Char(char * buffer, int bufferSize, char charToWrite);
};

}

#endif
