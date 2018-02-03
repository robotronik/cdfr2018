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
LIBS:Nucleo_64
LIBS:2
LIBS:dsn2596
LIBS:central-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L TDA8541 U4
U 1 1 5A74B05B
P 6050 3500
F 0 "U4" H 6050 3600 60  0000 C CNN
F 1 "TDA8541" H 6050 3700 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6050 3500 60  0001 C CNN
F 3 "" H 6050 3500 60  0000 C CNN
	1    6050 3500
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P2
U 1 1 5A74B105
P 7800 3350
F 0 "P2" H 7800 3500 50  0000 C CNN
F 1 "CONN_Speaker" V 7900 3350 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 7800 3350 50  0001 C CNN
F 3 "" H 7800 3350 50  0000 C CNN
	1    7800 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR23
U 1 1 5A74B1CD
P 7100 3300
F 0 "#PWR23" H 7100 3050 50  0001 C CNN
F 1 "GND" H 7100 3150 50  0000 C CNN
F 2 "" H 7100 3300 50  0000 C CNN
F 3 "" H 7100 3300 50  0000 C CNN
	1    7100 3300
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR22
U 1 1 5A74B2FA
P 6950 3400
F 0 "#PWR22" H 6950 3250 50  0001 C CNN
F 1 "+5V" H 6950 3540 50  0000 C CNN
F 2 "" H 6950 3400 50  0000 C CNN
F 3 "" H 6950 3400 50  0000 C CNN
	1    6950 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	6700 3300 7100 3300
Wire Wire Line
	6700 3400 6950 3400
Wire Wire Line
	6700 3200 7600 3200
Wire Wire Line
	7600 3200 7600 3300
Wire Wire Line
	7600 3400 7600 3500
Wire Wire Line
	7600 3500 6700 3500
$Comp
L GND #PWR20
U 1 1 5A74B53B
P 5250 3050
F 0 "#PWR20" H 5250 2800 50  0001 C CNN
F 1 "GND" H 5250 2900 50  0000 C CNN
F 2 "" H 5250 3050 50  0000 C CNN
F 3 "" H 5250 3050 50  0000 C CNN
	1    5250 3050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 3050 5250 3200
Wire Wire Line
	5250 3200 5400 3200
$Comp
L CP C3
U 1 1 5A74B66F
P 4850 3500
F 0 "C3" H 4875 3600 50  0000 L CNN
F 1 "47uCP" H 4875 3400 50  0000 L CNN
F 2 "" H 4888 3350 50  0001 C CNN
F 3 "" H 4850 3500 50  0000 C CNN
	1    4850 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 3350 4850 3300
Wire Wire Line
	4850 3300 5400 3300
Wire Wire Line
	5400 3400 5200 3400
Wire Wire Line
	5200 3400 5200 3300
Connection ~ 5200 3300
$Comp
L GND #PWR19
U 1 1 5A74B7C7
P 4850 3800
F 0 "#PWR19" H 4850 3550 50  0001 C CNN
F 1 "GND" H 4850 3650 50  0000 C CNN
F 2 "" H 4850 3800 50  0000 C CNN
F 3 "" H 4850 3800 50  0000 C CNN
	1    4850 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 3650 4850 3800
$Comp
L R R6
U 1 1 5A74B8C3
P 6350 3900
F 0 "R6" V 6430 3900 50  0000 C CNN
F 1 "56kR" V 6350 3900 50  0000 C CNN
F 2 "" V 6280 3900 50  0001 C CNN
F 3 "" H 6350 3900 50  0000 C CNN
	1    6350 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	6500 3900 7050 3900
Wire Wire Line
	7050 3900 7050 3500
Connection ~ 7050 3500
Wire Wire Line
	6200 3900 5300 3900
Wire Wire Line
	5300 3500 5300 4050
Wire Wire Line
	5300 3500 5400 3500
$Comp
L R R5
U 1 1 5A74B941
P 5300 4200
F 0 "R5" V 5380 4200 50  0000 C CNN
F 1 "11kR" V 5300 4200 50  0000 C CNN
F 2 "" V 5230 4200 50  0001 C CNN
F 3 "" H 5300 4200 50  0000 C CNN
	1    5300 4200
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5A74B97E
P 5300 4550
F 0 "C4" H 5325 4650 50  0000 L CNN
F 1 "1uC" H 5325 4450 50  0000 L CNN
F 2 "" H 5338 4400 50  0001 C CNN
F 3 "" H 5300 4550 50  0000 C CNN
	1    5300 4550
	1    0    0    -1  
$EndComp
Connection ~ 5300 3900
Wire Wire Line
	5300 4350 5300 4400
Wire Wire Line
	5300 4700 5300 5050
Text Notes 6000 4550 0    60   ~ 0
gain=2*R6/R5
$Comp
L CONN_01X02 P3
U 1 1 5A74BF57
P 5350 5250
F 0 "P3" H 5350 5400 50  0000 C CNN
F 1 "CONN_jack" V 5450 5250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5350 5250 50  0001 C CNN
F 3 "" H 5350 5250 50  0000 C CNN
	1    5350 5250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR21
U 1 1 5A74BFFC
P 5400 4950
F 0 "#PWR21" H 5400 4700 50  0001 C CNN
F 1 "GND" H 5400 4800 50  0000 C CNN
F 2 "" H 5400 4950 50  0000 C CNN
F 3 "" H 5400 4950 50  0000 C CNN
	1    5400 4950
	-1   0    0    1   
$EndComp
Wire Wire Line
	5400 4950 5400 5050
$Comp
L C C5
U 1 1 5A751FB3
P 8800 3550
F 0 "C5" H 8825 3650 50  0000 L CNN
F 1 "C" H 8825 3450 50  0000 L CNN
F 2 "" H 8838 3400 50  0000 C CNN
F 3 "" H 8800 3550 50  0000 C CNN
	1    8800 3550
	1    0    0    -1  
$EndComp
$Comp
L CP C6
U 1 1 5A751FE4
P 9100 3550
F 0 "C6" H 9125 3650 50  0000 L CNN
F 1 "CP" H 9125 3450 50  0000 L CNN
F 2 "" H 9138 3400 50  0000 C CNN
F 3 "" H 9100 3550 50  0000 C CNN
	1    9100 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR25
U 1 1 5A752015
P 8800 3800
F 0 "#PWR25" H 8800 3550 50  0001 C CNN
F 1 "GND" H 8800 3650 50  0000 C CNN
F 2 "" H 8800 3800 50  0000 C CNN
F 3 "" H 8800 3800 50  0000 C CNN
	1    8800 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR26
U 1 1 5A75203B
P 9100 3800
F 0 "#PWR26" H 9100 3550 50  0001 C CNN
F 1 "GND" H 9100 3650 50  0000 C CNN
F 2 "" H 9100 3800 50  0000 C CNN
F 3 "" H 9100 3800 50  0000 C CNN
	1    9100 3800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR24
U 1 1 5A752061
P 8800 3300
F 0 "#PWR24" H 8800 3150 50  0001 C CNN
F 1 "+5V" H 8800 3440 50  0000 C CNN
F 2 "" H 8800 3300 50  0000 C CNN
F 3 "" H 8800 3300 50  0000 C CNN
	1    8800 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 3300 8800 3400
Wire Wire Line
	8800 3350 9100 3350
Wire Wire Line
	9100 3350 9100 3400
Connection ~ 8800 3350
Wire Wire Line
	8800 3700 8800 3800
Wire Wire Line
	9100 3700 9100 3800
$EndSCHEMATC
