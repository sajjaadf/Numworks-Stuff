#include "usb_timer.h"
#include "global_preferences.h"
#include "apps_container.h"

USBTimer::USBTimer(AppsContainer * container) :
  Timer(1),
  m_container(container),
  m_previousPluggedState(false)
{
}

bool USBTimer::fire() {
  bool needRedrawing = false;
  if (Ion::USB::isPlugged()) {
    if (!m_previousPluggedState && GlobalPreferences::sharedGlobalPreferences()->examMode() == GlobalPreferences::ExamMode::Activate) {
      m_container->displayExamModePopUp(false);
      needRedrawing = true;
    }
    KDColor LEDColor = Ion::Battery::isCharging() ? KDColorYellow : KDColorGreen;
    Ion::LED::setColor(LEDColor);
    if (!m_previousPluggedState) {
      Ion::Backlight::setBrightness(Ion::Backlight::MaxBrightness);
    }
    m_previousPluggedState = true;
  } else {
    if (m_previousPluggedState) {
      Ion::LED::setColor(KDColorBlack);
      m_previousPluggedState = false;
    }
  }
  return needRedrawing;
}
