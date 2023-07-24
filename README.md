# T4motion
Prototyp eines CAN-Controllers für die Haldex Gen.1 zur Verwendung im VW T4.

Hardware:
ATmega328
MCP2515 und MCP2551
SH1106 OLED-Display

Pinbelegung Haldex-STG:
1: Plus
2: Masse
3: Bremslichtschalter (Haldex aktiv bei Masse, inaktiv bei +12V)
4: Handbremskontrolle (Haldex aktiv bei +12V, inaktiv bei Masse)
5: K-Leitung (Diagnose: Öltemperatur)
6: nicht belegt
7: CAN-Low
8: CAN-High


OLED ist via I2C am µC angeschlossen.
An A1 befindet sich ein Poti zur Eingabe des Sollwerts.

MCP2515/MCP2551 sind über SPI angebunden (analog zum CAN-Bus Shield)
