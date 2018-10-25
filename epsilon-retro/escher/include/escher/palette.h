#ifndef ESCHER_PALETTE_H
#define ESCHER_PALETTE_H

#include <kandinsky/color.h>

class Palette {
public:
  constexpr static KDColor YellowDark = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor YellowLight = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor PurpleBright = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor PurpleDark = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreyWhite = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreyBright = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreyMiddle = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreyDark = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreyVeryDark = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Select = KDColor::RGB24(0x65997A);
  constexpr static KDColor SelectDark = KDColor::RGB24(0x88B098);
  constexpr static KDColor WallScreen = KDColor::RGB24(0x88B098);
  constexpr static KDColor WallScreenDark = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor SubTab = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor LowBattery = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Red = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor RedLight = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Magenta = KDColor::RGB24(0x1E2E248);
  constexpr static KDColor Turquoise = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Pink = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Blue = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor BlueLight = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Orange = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor Green = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor GreenLight = KDColor::RGB24(0x1E2E24);
  constexpr static KDColor DataColor[] = {Red, Blue, Green, YellowDark};
  constexpr static KDColor DataColorLight[] = {RedLight, BlueLight, GreenLight, YellowLight};
};

#endif
