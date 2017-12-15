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
Sheet 3 5
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
L stm32f303k8t6 U3
U 1 1 59613C78
P 5800 3050
F 0 "U3" H 5800 3050 60  0000 C CNN
F 1 "stm32f303k8t6" H 5800 2950 60  0000 C CNN
F 2 "Housings_QFP:LQFP-32_7x7mm_Pitch0.8mm" H 5450 3000 60  0001 C CNN
F 3 "" H 5450 3000 60  0000 C CNN
	1    5800 3050
	1    0    0    -1  
$EndComp
Text Label 6850 2700 0    60   ~ 0
swclk
Text Label 6850 2800 0    60   ~ 0
swdio
Wire Wire Line
	6650 2700 6850 2700
Wire Wire Line
	6650 2800 6850 2800
Text Label 4650 3000 0    60   ~ 0
nrst
$Comp
L C C5
U 1 1 59615D27
P 4250 3000
F 0 "C5" H 4275 3100 50  0000 L CNN
F 1 "100nC" H 4275 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 4288 2850 50  0001 C CNN
F 3 "" H 4250 3000 50  0001 C CNN
	1    4250 3000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR028
U 1 1 59615D7C
P 3950 3000
F 0 "#PWR028" H 3950 2750 50  0001 C CNN
F 1 "GND" H 3950 2850 50  0000 C CNN
F 2 "" H 3950 3000 50  0001 C CNN
F 3 "" H 3950 3000 50  0001 C CNN
	1    3950 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 3000 4100 3000
Wire Wire Line
	4400 3000 4950 3000
$Comp
L CONN_01X04 J2
U 1 1 59615ECD
P 3450 1500
F 0 "J2" H 3450 1750 50  0000 C CNN
F 1 "swdio" V 3550 1500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 3450 1500 50  0001 C CNN
F 3 "" H 3450 1500 50  0001 C CNN
	1    3450 1500
	-1   0    0    1   
$EndComp
Text Label 4050 1450 0    60   ~ 0
swclk
Text Label 4050 1350 0    60   ~ 0
swdio
Text Label 4050 1550 0    60   ~ 0
nrst
$Comp
L GND #PWR029
U 1 1 59615FF5
P 3850 1650
F 0 "#PWR029" H 3850 1400 50  0001 C CNN
F 1 "GND" H 3850 1500 50  0000 C CNN
F 2 "" H 3850 1650 50  0001 C CNN
F 3 "" H 3850 1650 50  0001 C CNN
	1    3850 1650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3650 1350 4050 1350
Wire Wire Line
	3650 1450 4050 1450
Wire Wire Line
	3650 1550 4050 1550
Wire Wire Line
	3650 1650 3850 1650
$Comp
L GND #PWR030
U 1 1 596164DB
P 5450 2050
F 0 "#PWR030" H 5450 1800 50  0001 C CNN
F 1 "GND" H 5450 1900 50  0000 C CNN
F 2 "" H 5450 2050 50  0001 C CNN
F 3 "" H 5450 2050 50  0001 C CNN
	1    5450 2050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR031
U 1 1 596164F6
P 6150 4050
F 0 "#PWR031" H 6150 3800 50  0001 C CNN
F 1 "GND" H 6150 3900 50  0000 C CNN
F 2 "" H 6150 4050 50  0001 C CNN
F 3 "" H 6150 4050 50  0001 C CNN
	1    6150 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2050 5450 2200
Wire Wire Line
	6150 3900 6150 4050
$Comp
L +3.3V #PWR032
U 1 1 59629D6A
P 6800 3400
F 0 "#PWR032" H 6800 3250 50  0001 C CNN
F 1 "+3.3V" H 6800 3540 50  0000 C CNN
F 2 "" H 6800 3400 50  0001 C CNN
F 3 "" H 6800 3400 50  0001 C CNN
	1    6800 3400
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR033
U 1 1 59629D85
P 4750 3100
F 0 "#PWR033" H 4750 2950 50  0001 C CNN
F 1 "+3.3V" H 4750 3240 50  0000 C CNN
F 2 "" H 4750 3100 50  0001 C CNN
F 3 "" H 4750 3100 50  0001 C CNN
	1    4750 3100
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR034
U 1 1 59629D99
P 4750 2700
F 0 "#PWR034" H 4750 2550 50  0001 C CNN
F 1 "+3.3V" H 4750 2840 50  0000 C CNN
F 2 "" H 4750 2700 50  0001 C CNN
F 3 "" H 4750 2700 50  0001 C CNN
	1    4750 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 2700 4950 2700
Wire Wire Line
	4750 3100 4950 3100
Wire Wire Line
	6650 3400 6800 3400
$Comp
L LED_ALT D5
U 1 1 596A8942
P 7900 3850
F 0 "D5" H 7900 3950 50  0000 C CNN
F 1 "LED_ALT" H 7900 3750 50  0000 C CNN
F 2 "Diodes_SMD:D_SMA-SMB_Universal_Handsoldering" H 7900 3850 50  0001 C CNN
F 3 "" H 7900 3850 50  0001 C CNN
	1    7900 3850
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 596A89D5
P 7900 3350
F 0 "R6" V 7980 3350 50  0000 C CNN
F 1 "R" V 7900 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7830 3350 50  0001 C CNN
F 3 "" H 7900 3350 50  0001 C CNN
	1    7900 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR035
U 1 1 596A8A12
P 7900 4300
F 0 "#PWR035" H 7900 4050 50  0001 C CNN
F 1 "GND" H 7900 4150 50  0000 C CNN
F 2 "" H 7900 4300 50  0001 C CNN
F 3 "" H 7900 4300 50  0001 C CNN
	1    7900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3500 7900 3700
Wire Wire Line
	7900 4000 7900 4300
Text HLabel 4700 3200 0    60   Input ~ 0
Ch_A1
Text HLabel 4700 3300 0    60   Input ~ 0
Ch_B1
Text HLabel 6950 3300 2    60   Input ~ 0
Ch_A2
Text HLabel 6950 3200 2    60   Input ~ 0
Ch_B2
Wire Wire Line
	4700 3200 4950 3200
Wire Wire Line
	4700 3300 4950 3300
Wire Wire Line
	6950 3200 6650 3200
Wire Wire Line
	6950 3300 6650 3300
$Comp
L CONN_01X02 J1
U 1 1 5974F8CE
P 3250 4000
F 0 "J1" H 3250 4150 50  0000 C CNN
F 1 "CONN_SERIAL" V 3350 4000 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 3250 4000 50  0001 C CNN
F 3 "" H 3250 4000 50  0001 C CNN
	1    3250 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4350 3400 4950 3400
Wire Wire Line
	5450 4200 5450 3900
Text Label 4350 3400 0    60   ~ 0
TX
Text Label 5450 4200 3    60   ~ 0
RX
Wire Wire Line
	3750 3950 3450 3950
Wire Wire Line
	3750 4050 3450 4050
Text Label 3750 3950 0    60   ~ 0
TX
Text Label 3750 4050 0    60   ~ 0
RX
$Comp
L R R5
U 1 1 5974DF8C
P 5200 1700
F 0 "R5" V 5280 1700 50  0000 C CNN
F 1 "10kR" V 5200 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5130 1700 50  0001 C CNN
F 3 "" H 5200 1700 50  0001 C CNN
	1    5200 1700
	0    1    1    0   
$EndComp
$Comp
L Jumper_NC_Dual JP1
U 1 1 5974E017
P 4900 1700
F 0 "JP1" H 4950 1600 50  0000 L CNN
F 1 "Jumper_NC_Dual" H 4900 1800 50  0000 C BNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 4900 1700 50  0001 C CNN
F 3 "" H 4900 1700 50  0001 C CNN
	1    4900 1700
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR036
U 1 1 5974E084
P 4900 1300
F 0 "#PWR036" H 4900 1150 50  0001 C CNN
F 1 "+3.3V" H 4900 1440 50  0000 C CNN
F 2 "" H 4900 1300 50  0001 C CNN
F 3 "" H 4900 1300 50  0001 C CNN
	1    4900 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR037
U 1 1 5974E0A8
P 4900 2100
F 0 "#PWR037" H 4900 1850 50  0001 C CNN
F 1 "GND" H 4900 1950 50  0000 C CNN
F 2 "" H 4900 2100 50  0001 C CNN
F 3 "" H 4900 2100 50  0001 C CNN
	1    4900 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 1300 4900 1450
Wire Wire Line
	4900 1950 4900 2100
Wire Wire Line
	5000 1700 5050 1700
Wire Wire Line
	5350 1700 5550 1700
Wire Wire Line
	5550 1700 5550 2200
Text HLabel 5850 4050 3    60   Input ~ 0
BRAKE_1
Text HLabel 6950 3100 2    60   Input ~ 0
TACHO_1
Text HLabel 6050 4050 3    60   Input ~ 0
DIAG_1
Text HLabel 5950 4050 3    60   Input ~ 0
DIR_1
Text HLabel 5750 4050 3    60   Input ~ 0
EN_1
Wire Wire Line
	6650 3000 7900 3000
Wire Wire Line
	7900 3000 7900 3200
Text HLabel 5950 2000 1    60   Input ~ 0
BRAKE_2
Text HLabel 6050 2000 1    60   Input ~ 0
DIR_2
Text HLabel 6150 2000 1    60   Input ~ 0
DIAG_2
Text HLabel 6950 2900 2    60   Input ~ 0
TACHO_2
Text HLabel 5850 2000 1    60   Input ~ 0
EN_2
Wire Wire Line
	5850 2000 5850 2200
Wire Wire Line
	5950 2000 5950 2200
Wire Wire Line
	6050 2000 6050 2200
Wire Wire Line
	6150 2000 6150 2200
Wire Wire Line
	6950 2900 6650 2900
Wire Wire Line
	6650 3100 6950 3100
Wire Wire Line
	6050 3900 6050 4050
Wire Wire Line
	5950 3900 5950 4050
Wire Wire Line
	5850 3900 5850 4050
Wire Wire Line
	5750 3900 5750 4050
$Comp
L CONN_01X02 P1
U 1 1 59C14D2B
P 5700 1150
F 0 "P1" H 5700 1300 50  0000 C CNN
F 1 "CONN_I2C" V 5800 1150 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 5700 1150 50  0001 C CNN
F 3 "" H 5700 1150 50  0000 C CNN
	1    5700 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5650 1350 5650 2200
Wire Wire Line
	5750 1350 5750 2200
Text Label 5650 1700 1    60   ~ 0
I2C_SDA
Text Label 5750 1700 1    60   ~ 0
I2C_SCL
$Comp
L Crystal Y1
U 1 1 59C158A8
P 3150 2750
F 0 "Y1" H 3150 2900 50  0000 C CNN
F 1 "Crystal_8MHz" H 3150 2600 50  0000 C CNN
F 2 "Crystals:Crystal_HC49-4H_Vertical" H 3150 2750 50  0001 C CNN
F 3 "" H 3150 2750 50  0000 C CNN
	1    3150 2750
	0    1    1    0   
$EndComp
$Comp
L C C3
U 1 1 59C15931
P 2850 2500
F 0 "C3" H 2875 2600 50  0000 L CNN
F 1 "10nC" H 2875 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2888 2350 50  0001 C CNN
F 3 "" H 2850 2500 50  0000 C CNN
	1    2850 2500
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 59C15C12
P 2850 3000
F 0 "C4" H 2875 3100 50  0000 L CNN
F 1 "10nC" H 2875 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2888 2850 50  0001 C CNN
F 3 "" H 2850 3000 50  0000 C CNN
	1    2850 3000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR038
U 1 1 59C15C7D
P 2400 2500
F 0 "#PWR038" H 2400 2250 50  0001 C CNN
F 1 "GND" H 2400 2350 50  0000 C CNN
F 2 "" H 2400 2500 50  0000 C CNN
F 3 "" H 2400 2500 50  0000 C CNN
	1    2400 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 2500 2700 2500
Wire Wire Line
	2700 3000 2600 3000
Wire Wire Line
	2600 3000 2600 2500
Connection ~ 2600 2500
Wire Wire Line
	3000 2500 4300 2500
Wire Wire Line
	3150 2500 3150 2600
Wire Wire Line
	3000 3000 3450 3000
Wire Wire Line
	3150 3000 3150 2900
Connection ~ 3150 2500
Connection ~ 3150 3000
$Comp
L CONN_01X02 P2
U 1 1 59C1439A
P 5600 4750
F 0 "P2" H 5600 4900 50  0000 C CNN
F 1 "CONN_FREE" V 5700 4750 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 5600 4750 50  0001 C CNN
F 3 "" H 5600 4750 50  0000 C CNN
	1    5600 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 3900 5550 4550
Wire Wire Line
	5650 3900 5650 4550
$Comp
L R R14
U 1 1 59C17650
P 3950 2850
F 0 "R14" V 4030 2850 50  0000 C CNN
F 1 "R" V 3950 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 3880 2850 50  0001 C CNN
F 3 "" H 3950 2850 50  0000 C CNN
	1    3950 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 2850 4950 2850
Wire Wire Line
	4950 2850 4950 2900
Wire Wire Line
	3800 2850 3450 2850
Wire Wire Line
	3450 2850 3450 3000
Wire Wire Line
	4300 2500 4300 2800
Wire Wire Line
	4300 2800 4950 2800
$Comp
L MCP2562 U9
U 1 1 59C2F137
P 3500 5300
F 0 "U9" H 3500 5400 60  0000 C CNN
F 1 "MCP2562" H 3500 5300 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3750 5300 60  0001 C CNN
F 3 "" H 3750 5300 60  0000 C CNN
	1    3500 5300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR039
U 1 1 59C2F346
P 3300 4500
F 0 "#PWR039" H 3300 4350 50  0001 C CNN
F 1 "+3.3V" H 3300 4640 50  0000 C CNN
F 2 "" H 3300 4500 50  0000 C CNN
F 3 "" H 3300 4500 50  0000 C CNN
	1    3300 4500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR040
U 1 1 59C2F5FE
P 3700 4500
F 0 "#PWR040" H 3700 4350 50  0001 C CNN
F 1 "+5V" H 3700 4640 50  0000 C CNN
F 2 "" H 3700 4500 50  0000 C CNN
F 3 "" H 3700 4500 50  0000 C CNN
	1    3700 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR041
U 1 1 59C2F720
P 3500 6100
F 0 "#PWR041" H 3500 5850 50  0001 C CNN
F 1 "GND" H 3500 5950 50  0000 C CNN
F 2 "" H 3500 6100 50  0000 C CNN
F 3 "" H 3500 6100 50  0000 C CNN
	1    3500 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR042
U 1 1 59C2F752
P 2550 5650
F 0 "#PWR042" H 2550 5400 50  0001 C CNN
F 1 "GND" H 2550 5500 50  0000 C CNN
F 2 "" H 2550 5650 50  0000 C CNN
F 3 "" H 2550 5650 50  0000 C CNN
	1    2550 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5650 2550 5500
Wire Wire Line
	2550 5500 2800 5500
Wire Wire Line
	3500 6100 3500 5900
Text Label 2450 5100 0    60   ~ 0
TX
Text Label 2450 5300 0    60   ~ 0
RX
Wire Wire Line
	2450 5100 2800 5100
Wire Wire Line
	2450 5300 2800 5300
Wire Wire Line
	3300 4500 3300 4600
Wire Wire Line
	3700 4500 3700 4600
$Comp
L CONN_01X02 P3
U 1 1 59C2FCB1
P 4850 5350
F 0 "P3" H 4850 5500 50  0000 C CNN
F 1 "CONN_CAN" V 4950 5350 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 4850 5350 50  0001 C CNN
F 3 "" H 4850 5350 50  0000 C CNN
	1    4850 5350
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 59C2FCEC
P 4450 5300
F 0 "R15" V 4530 5300 50  0000 C CNN
F 1 "R" V 4450 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4380 5300 50  0001 C CNN
F 3 "" H 4450 5300 50  0000 C CNN
	1    4450 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5500 4650 5500
Wire Wire Line
	4650 5500 4650 5400
Wire Wire Line
	4450 5450 4450 5500
Connection ~ 4450 5500
Wire Wire Line
	4200 5100 4650 5100
Wire Wire Line
	4450 5100 4450 5150
Wire Wire Line
	4650 5100 4650 5300
Connection ~ 4450 5100
$EndSCHEMATC
