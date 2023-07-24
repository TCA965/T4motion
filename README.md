# T4motion
Prototyp eines CAN-Controllers für die Haldex Gen.1 zur Verwendung im VW T4.

<br>
Hardware:
<br>
ATmega328
<br>
MCP2515 und MCP2551
<br>
SH1106 OLED-Display

<br><br><br>

Pinbelegung Haldex-STG:
<br>
1: Plus
<br>
2: Masse
<br>
3: Bremslichtschalter (Haldex aktiv bei Masse, inaktiv bei +12V)
<br>
4: Handbremskontrolle (Haldex aktiv bei +12V, inaktiv bei Masse)
<br>
5: K-Leitung (Diagnose: Öltemperatur)
<br>
6: nicht belegt
<br>
7: CAN-Low
<br>
8: CAN-High
<br>

<br><br><br>



OLED ist via I2C am µC angeschlossen.

An A1 befindet sich ein Poti zur Eingabe des Sollwerts.

MCP2515/MCP2551 sind über SPI angebunden (analog zum CAN-Bus Shield)
