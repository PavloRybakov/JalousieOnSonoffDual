# Arduino Ide

# JalousieOnSonoffDual
Control (and manage) external jalousie using Sonoff Dual

Expected motor: usual tube-motor with two "live"-cabels.

Modules description:
# DualHandler:
Handle SonoffDual - read/set state, blink (for debug reasons)
# MyDualWrapper:
Tries to prevent situation where both #SonoffDual relays are on
# VirtualMotor:
Module tries to calculate current jalousie position using max time which motor need to open/close window.
# MqttWrapper:
Module contains several useful methods for publishing relays state and jalousie position
# MyWiFi:
Module contains minimal useful methods for handling wifi
