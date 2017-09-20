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
Sheet 4 5
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
L VCC #PWR042
U 1 1 5962E5D8
P 4650 2300
F 0 "#PWR042" H 4650 2150 50  0001 C CNN
F 1 "VCC" H 4650 2450 50  0000 C CNN
F 2 "" H 4650 2300 50  0001 C CNN
F 3 "" H 4650 2300 50  0001 C CNN
	1    4650 2300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR043
U 1 1 5962E5EC
P 10450 2450
F 0 "#PWR043" H 10450 2300 50  0001 C CNN
F 1 "+3.3V" H 10450 2590 50  0000 C CNN
F 2 "" H 10450 2450 50  0001 C CNN
F 3 "" H 10450 2450 50  0001 C CNN
	1    10450 2450
	1    0    0    -1  
$EndComp
$Comp
L B_Plug_5mm U4
U 1 1 5962E6CD
P 3400 2600
F 0 "U4" H 3400 3000 60  0000 C CNN
F 1 "B_Plug_5mm" H 3400 3100 60  0000 C CNN
F 2 "B plug:B plug 5mm" H 3400 2600 60  0001 C CNN
F 3 "" H 3400 2600 60  0000 C CNN
	1    3400 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 5962E732
P 3650 2900
F 0 "#PWR044" H 3650 2650 50  0001 C CNN
F 1 "GND" H 3650 2750 50  0000 C CNN
F 2 "" H 3650 2900 50  0001 C CNN
F 3 "" H 3650 2900 50  0001 C CNN
	1    3650 2900
	1    0    0    -1  
$EndComp
$Comp
L Fuse F1
U 1 1 5962E74B
P 3900 2550
F 0 "F1" V 3980 2550 50  0000 C CNN
F 1 "Fuse" V 3825 2550 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" V 3830 2550 50  0001 C CNN
F 3 "" H 3900 2550 50  0001 C CNN
	1    3900 2550
	0    1    1    0   
$EndComp
$Comp
L D_ALT D6
U 1 1 5962E7D0
P 4250 2750
F 0 "D6" H 4250 2850 50  0000 C CNN
F 1 "D_10A" H 4250 2650 50  0000 C CNN
F 2 "Diodes_SMD:D_SMC_Handsoldering" H 4250 2750 50  0001 C CNN
F 3 "" H 4250 2750 50  0001 C CNN
	1    4250 2750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR045
U 1 1 5962E829
P 4250 3000
F 0 "#PWR045" H 4250 2750 50  0001 C CNN
F 1 "GND" H 4250 2850 50  0000 C CNN
F 2 "" H 4250 3000 50  0001 C CNN
F 3 "" H 4250 3000 50  0001 C CNN
	1    4250 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2550 3750 2550
Wire Wire Line
	4250 2550 4250 2600
Wire Wire Line
	4250 2900 4250 3000
Connection ~ 4250 2550
Wire Wire Line
	3500 2650 3650 2650
Wire Wire Line
	3650 2650 3650 2900
$Comp
L PWR_FLAG #FLG046
U 1 1 5962EB73
P 4500 2400
F 0 "#FLG046" H 4500 2475 50  0001 C CNN
F 1 "PWR_FLAG" H 4500 2550 50  0000 C CNN
F 2 "" H 4500 2400 50  0001 C CNN
F 3 "" H 4500 2400 50  0001 C CNN
	1    4500 2400
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG047
U 1 1 5962EBA1
P 3800 2850
F 0 "#FLG047" H 3800 2925 50  0001 C CNN
F 1 "PWR_FLAG" H 3800 3000 50  0000 C CNN
F 2 "" H 3800 2850 50  0001 C CNN
F 3 "" H 3800 2850 50  0001 C CNN
	1    3800 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 2850 3650 2850
Connection ~ 3650 2850
$Comp
L +5V #PWR048
U 1 1 5962ECD4
P 7900 2450
F 0 "#PWR048" H 7900 2300 50  0001 C CNN
F 1 "+5V" H 7900 2590 50  0000 C CNN
F 2 "" H 7900 2450 50  0001 C CNN
F 3 "" H 7900 2450 50  0001 C CNN
	1    7900 2450
	1    0    0    -1  
$EndComp
$Comp
L LM317_SOT223 U5
U 1 1 5962EDF3
P 7000 2700
F 0 "U5" H 7000 3000 50  0000 C CNN
F 1 "LM317_SOT223" H 7050 2450 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3Lead_TabPin2" H 7000 2700 50  0001 C CNN
F 3 "" H 7000 2700 50  0001 C CNN
	1    7000 2700
	1    0    0    -1  
$EndComp
$Comp
L LM317_SOT223 U6
U 1 1 5962EF34
P 9650 2650
F 0 "U6" H 9650 2950 50  0000 C CNN
F 1 "LM317_SOT223" H 9700 2400 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3Lead_TabPin2" H 9650 2650 50  0001 C CNN
F 3 "" H 9650 2650 50  0001 C CNN
	1    9650 2650
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR049
U 1 1 5962F11A
P 6450 2450
F 0 "#PWR049" H 6450 2300 50  0001 C CNN
F 1 "VCC" H 6450 2600 50  0000 C CNN
F 2 "" H 6450 2450 50  0001 C CNN
F 3 "" H 6450 2450 50  0001 C CNN
	1    6450 2450
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR050
U 1 1 5962F138
P 9050 2450
F 0 "#PWR050" H 9050 2300 50  0001 C CNN
F 1 "VCC" H 9050 2600 50  0000 C CNN
F 2 "" H 9050 2450 50  0001 C CNN
F 3 "" H 9050 2450 50  0001 C CNN
	1    9050 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2450 6450 2550
Wire Wire Line
	6450 2550 6600 2550
Wire Wire Line
	7400 2550 8000 2550
Wire Wire Line
	7900 2450 7900 2800
Wire Wire Line
	9050 2450 9050 2500
Wire Wire Line
	9050 2500 9250 2500
Wire Wire Line
	10050 2500 10500 2500
Wire Wire Line
	10450 2450 10450 2700
$Comp
L R R8
U 1 1 5962F28F
P 7000 3350
F 0 "R8" V 7080 3350 50  0000 C CNN
F 1 "3.3kR" V 7000 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6930 3350 50  0001 C CNN
F 3 "" H 7000 3350 50  0001 C CNN
	1    7000 3350
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5962F2E4
P 7350 3150
F 0 "R9" V 7430 3150 50  0000 C CNN
F 1 "10kR" V 7350 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7280 3150 50  0001 C CNN
F 3 "" H 7350 3150 50  0001 C CNN
	1    7350 3150
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 5962F32D
P 9650 3350
F 0 "R11" V 9730 3350 50  0000 C CNN
F 1 "33kR" V 9650 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 9580 3350 50  0001 C CNN
F 3 "" H 9650 3350 50  0001 C CNN
	1    9650 3350
	-1   0    0    1   
$EndComp
$Comp
L R R12
U 1 1 5962F372
P 10000 3150
F 0 "R12" V 10080 3150 50  0000 C CNN
F 1 "56kR" V 10000 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 9930 3150 50  0001 C CNN
F 3 "" H 10000 3150 50  0001 C CNN
	1    10000 3150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR051
U 1 1 5962F3CB
P 7000 3600
F 0 "#PWR051" H 7000 3350 50  0001 C CNN
F 1 "GND" H 7000 3450 50  0000 C CNN
F 2 "" H 7000 3600 50  0001 C CNN
F 3 "" H 7000 3600 50  0001 C CNN
	1    7000 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR052
U 1 1 5962F3F1
P 9650 3600
F 0 "#PWR052" H 9650 3350 50  0001 C CNN
F 1 "GND" H 9650 3450 50  0000 C CNN
F 2 "" H 9650 3600 50  0001 C CNN
F 3 "" H 9650 3600 50  0001 C CNN
	1    9650 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3600 7000 3500
Wire Wire Line
	7000 3200 7000 3050
Wire Wire Line
	7200 3150 7000 3150
Connection ~ 7000 3150
Wire Wire Line
	7500 3150 7650 3150
Wire Wire Line
	7650 3150 7650 2550
Connection ~ 7650 2550
Wire Wire Line
	9650 3200 9650 3000
Wire Wire Line
	9650 3600 9650 3500
Wire Wire Line
	9850 3150 9650 3150
Connection ~ 9650 3150
Wire Wire Line
	10150 3150 10300 3150
Wire Wire Line
	10300 3150 10300 2500
Connection ~ 10300 2500
$Comp
L R R7
U 1 1 59630075
P 5200 2550
F 0 "R7" V 5280 2550 50  0000 C CNN
F 1 "R" V 5200 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5130 2550 50  0001 C CNN
F 3 "" H 5200 2550 50  0001 C CNN
	1    5200 2550
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 5963017A
P 8150 2550
F 0 "R10" V 8230 2550 50  0000 C CNN
F 1 "R" V 8150 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 8080 2550 50  0001 C CNN
F 3 "" H 8150 2550 50  0001 C CNN
	1    8150 2550
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 596302DA
P 10650 2500
F 0 "R13" V 10730 2500 50  0000 C CNN
F 1 "R" V 10650 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 10580 2500 50  0001 C CNN
F 3 "" H 10650 2500 50  0001 C CNN
	1    10650 2500
	0    1    1    0   
$EndComp
$Comp
L LED_ALT D8
U 1 1 59630349
P 8400 2750
F 0 "D8" H 8400 2850 50  0000 C CNN
F 1 "LED_5V" H 8400 2650 50  0000 C CNN
F 2 "Diodes_SMD:D_SMA-SMB_Universal_Handsoldering" H 8400 2750 50  0001 C CNN
F 3 "" H 8400 2750 50  0001 C CNN
	1    8400 2750
	0    -1   -1   0   
$EndComp
$Comp
L LED_ALT D9
U 1 1 59630393
P 10850 2750
F 0 "D9" H 10850 2850 50  0000 C CNN
F 1 "LED_3.3V" H 10850 2650 50  0000 C CNN
F 2 "Diodes_SMD:D_SMA-SMB_Universal_Handsoldering" H 10850 2750 50  0001 C CNN
F 3 "" H 10850 2750 50  0001 C CNN
	1    10850 2750
	0    -1   -1   0   
$EndComp
$Comp
L LED_ALT D7
U 1 1 59630415
P 5450 2750
F 0 "D7" H 5450 2850 50  0000 C CNN
F 1 "LED_VCC" H 5450 2650 50  0000 C CNN
F 2 "Diodes_SMD:D_SMA-SMB_Universal_Handsoldering" H 5450 2750 50  0001 C CNN
F 3 "" H 5450 2750 50  0001 C CNN
	1    5450 2750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR053
U 1 1 5963063C
P 5450 3000
F 0 "#PWR053" H 5450 2750 50  0001 C CNN
F 1 "GND" H 5450 2850 50  0000 C CNN
F 2 "" H 5450 3000 50  0001 C CNN
F 3 "" H 5450 3000 50  0001 C CNN
	1    5450 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR054
U 1 1 5963066E
P 8400 3100
F 0 "#PWR054" H 8400 2850 50  0001 C CNN
F 1 "GND" H 8400 2950 50  0000 C CNN
F 2 "" H 8400 3100 50  0001 C CNN
F 3 "" H 8400 3100 50  0001 C CNN
	1    8400 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR055
U 1 1 596306A0
P 10850 3100
F 0 "#PWR055" H 10850 2850 50  0001 C CNN
F 1 "GND" H 10850 2950 50  0000 C CNN
F 2 "" H 10850 3100 50  0001 C CNN
F 3 "" H 10850 3100 50  0001 C CNN
	1    10850 3100
	1    0    0    -1  
$EndComp
Connection ~ 10450 2500
Wire Wire Line
	10800 2500 10850 2500
Wire Wire Line
	10850 2500 10850 2600
Wire Wire Line
	10850 2900 10850 3100
Connection ~ 7900 2550
Wire Wire Line
	8300 2550 8400 2550
Wire Wire Line
	8400 2550 8400 2600
Wire Wire Line
	8400 2900 8400 3100
Wire Wire Line
	5350 2550 5450 2550
Wire Wire Line
	5450 2550 5450 2600
Wire Wire Line
	5450 2900 5450 3000
$Comp
L C C8
U 1 1 5962615B
P 7900 2950
F 0 "C8" H 7925 3050 50  0000 L CNN
F 1 "10uC" H 7925 2850 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D8.0mm_P3.50mm" H 7938 2800 50  0001 C CNN
F 3 "" H 7900 2950 50  0001 C CNN
	1    7900 2950
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5962624A
P 10450 2850
F 0 "C9" H 10475 2950 50  0000 L CNN
F 1 "10uC" H 10475 2750 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D8.0mm_P3.50mm" H 10488 2700 50  0001 C CNN
F 3 "" H 10450 2850 50  0001 C CNN
	1    10450 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR056
U 1 1 596262B8
P 7900 3300
F 0 "#PWR056" H 7900 3050 50  0001 C CNN
F 1 "GND" H 7900 3150 50  0000 C CNN
F 2 "" H 7900 3300 50  0001 C CNN
F 3 "" H 7900 3300 50  0001 C CNN
	1    7900 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR057
U 1 1 5962633A
P 10450 3350
F 0 "#PWR057" H 10450 3100 50  0001 C CNN
F 1 "GND" H 10450 3200 50  0000 C CNN
F 2 "" H 10450 3350 50  0001 C CNN
F 3 "" H 10450 3350 50  0001 C CNN
	1    10450 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3300 7900 3100
Wire Wire Line
	10450 3000 10450 3350
$Comp
L C C7
U 1 1 596270DD
P 4800 2750
F 0 "C7" H 4825 2850 50  0000 L CNN
F 1 "100nC" H 4825 2650 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 4838 2600 50  0001 C CNN
F 3 "" H 4800 2750 50  0001 C CNN
	1    4800 2750
	1    0    0    -1  
$EndComp
$Comp
L CP C6
U 1 1 59627130
P 4550 2750
F 0 "C6" H 4575 2850 50  0000 L CNN
F 1 "100uCP" H 4450 2650 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D8.0mm_P3.50mm" H 4588 2600 50  0001 C CNN
F 3 "" H 4550 2750 50  0001 C CNN
	1    4550 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2550 5050 2550
Wire Wire Line
	4550 2550 4550 2600
Wire Wire Line
	4800 2550 4800 2600
Connection ~ 4550 2550
Wire Wire Line
	4250 2950 4800 2950
Wire Wire Line
	4550 2950 4550 2900
Connection ~ 4250 2950
Wire Wire Line
	4800 2950 4800 2900
Connection ~ 4550 2950
Wire Wire Line
	4500 2400 4650 2400
Connection ~ 4650 2550
Connection ~ 4650 2400
Wire Wire Line
	4650 2300 4650 2550
Connection ~ 4800 2550
$Comp
L CONN_01X02 J3
U 1 1 596A9627
P 5800 1950
F 0 "J3" H 5800 2100 50  0000 C CNN
F 1 "FAN" V 5900 1950 50  0000 C CNN
F 2 "Connectors_Molex:Molex_KK-6410-02_02x2.54mm_Straight" H 5800 1950 50  0001 C CNN
F 3 "" H 5800 1950 50  0001 C CNN
	1    5800 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR058
U 1 1 596AA4A9
P 5500 2050
F 0 "#PWR058" H 5500 1800 50  0001 C CNN
F 1 "GND" H 5500 1900 50  0000 C CNN
F 2 "" H 5500 2050 50  0001 C CNN
F 3 "" H 5500 2050 50  0001 C CNN
	1    5500 2050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR059
U 1 1 596AA4E5
P 5500 1850
F 0 "#PWR059" H 5500 1700 50  0001 C CNN
F 1 "VCC" H 5500 2000 50  0000 C CNN
F 2 "" H 5500 1850 50  0001 C CNN
F 3 "" H 5500 1850 50  0001 C CNN
	1    5500 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 1850 5500 1900
Wire Wire Line
	5500 1900 5600 1900
Wire Wire Line
	5500 2050 5500 2000
Wire Wire Line
	5500 2000 5600 2000
$EndSCHEMATC
