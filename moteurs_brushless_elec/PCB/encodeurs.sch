EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:2
LIBS:mcp2562
LIBS:stm32f042k6tx
LIBS:Propulsion_control
LIBS:carte_moteurs-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Encodeur U7
U 1 1 596AA862
P 5250 2400
F 0 "U7" H 5300 2500 60  0000 C CNN
F 1 "Encodeur" H 5300 2600 60  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 5250 2400 60  0001 C CNN
F 3 "" H 5250 2400 60  0001 C CNN
	1    5250 2400
	1    0    0    -1  
$EndComp
$Comp
L Encodeur U8
U 1 1 596AA88D
P 5250 3500
F 0 "U8" H 5300 3600 60  0000 C CNN
F 1 "Encodeur" H 5300 3700 60  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-04_04x2.54mm_Straight" H 5250 3500 60  0001 C CNN
F 3 "" H 5250 3500 60  0001 C CNN
	1    5250 3500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR060
U 1 1 596AAF98
P 4650 2050
F 0 "#PWR060" H 4650 1900 50  0001 C CNN
F 1 "+5V" H 4650 2190 50  0000 C CNN
F 2 "" H 4650 2050 50  0001 C CNN
F 3 "" H 4650 2050 50  0001 C CNN
	1    4650 2050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR061
U 1 1 596AAFB0
P 4650 3150
F 0 "#PWR061" H 4650 3000 50  0001 C CNN
F 1 "+5V" H 4650 3290 50  0000 C CNN
F 2 "" H 4650 3150 50  0001 C CNN
F 3 "" H 4650 3150 50  0001 C CNN
	1    4650 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR062
U 1 1 596AB08B
P 4500 2400
F 0 "#PWR062" H 4500 2150 50  0001 C CNN
F 1 "GND" H 4500 2250 50  0000 C CNN
F 2 "" H 4500 2400 50  0001 C CNN
F 3 "" H 4500 2400 50  0001 C CNN
	1    4500 2400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR063
U 1 1 596AB0FD
P 4500 3500
F 0 "#PWR063" H 4500 3250 50  0001 C CNN
F 1 "GND" H 4500 3350 50  0000 C CNN
F 2 "" H 4500 3500 50  0001 C CNN
F 3 "" H 4500 3500 50  0001 C CNN
	1    4500 3500
	0    1    1    0   
$EndComp
Text HLabel 4500 2250 0    60   Input ~ 0
Ch_A1
Text HLabel 4500 2550 0    60   Input ~ 0
Ch_B1
Text HLabel 4500 3350 0    60   Input ~ 0
Ch_A2
Text HLabel 4500 3650 0    60   Input ~ 0
Ch_B2
Wire Wire Line
	4650 2050 4650 2100
Wire Wire Line
	4650 2100 4750 2100
Wire Wire Line
	4500 2250 4750 2250
Wire Wire Line
	4500 2400 4750 2400
Wire Wire Line
	4500 2550 4750 2550
Wire Wire Line
	4650 3150 4650 3200
Wire Wire Line
	4650 3200 4750 3200
Wire Wire Line
	4500 3350 4750 3350
Wire Wire Line
	4500 3500 4750 3500
Wire Wire Line
	4500 3650 4750 3650
$EndSCHEMATC
