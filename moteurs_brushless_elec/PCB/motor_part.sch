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
$Descr A3 16535 11693
encoding utf-8
Sheet 2 5
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
L L6235D U1
U 1 1 59613948
P 4300 2350
F 0 "U1" H 4300 2350 60  0000 C CNN
F 1 "L6235D" H 4300 2550 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-24W_7.5x15.4mm_Pitch1.27mm" H 4000 2600 60  0001 C CNN
F 3 "" H 4000 2600 60  0001 C CNN
	1    4300 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 5961463B
P 5200 2300
F 0 "#PWR01" H 5200 2050 50  0001 C CNN
F 1 "GND" H 5200 2150 50  0000 C CNN
F 2 "" H 5200 2300 50  0001 C CNN
F 3 "" H 5200 2300 50  0001 C CNN
	1    5200 2300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR02
U 1 1 59614675
P 3250 2300
F 0 "#PWR02" H 3250 2050 50  0001 C CNN
F 1 "GND" H 3250 2150 50  0000 C CNN
F 2 "" H 3250 2300 50  0001 C CNN
F 3 "" H 3250 2300 50  0001 C CNN
	1    3250 2300
	0    1    1    0   
$EndComp
$Comp
L Maxon-200142 K1
U 1 1 59614717
P 6900 1750
F 0 "K1" H 6800 2100 60  0000 C CNN
F 1 "Maxon-200142" H 6850 500 60  0000 C CNN
F 2 "maxon:Molex-52207-0433" H 6400 1850 60  0001 C CNN
F 3 "" H 6400 1850 60  0000 C CNN
	1    6900 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 596149B6
P 6050 1950
F 0 "#PWR03" H 6050 1700 50  0001 C CNN
F 1 "GND" H 6050 1800 50  0000 C CNN
F 2 "" H 6050 1950 50  0001 C CNN
F 3 "" H 6050 1950 50  0001 C CNN
	1    6050 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	6300 1950 6050 1950
Wire Wire Line
	6300 1650 5500 1650
Wire Wire Line
	5500 1650 5500 1550
Wire Wire Line
	5500 1550 5000 1550
Wire Wire Line
	6300 1750 5650 1750
Wire Wire Line
	5650 1750 5650 1350
Wire Wire Line
	5650 1350 3400 1350
Wire Wire Line
	3400 1350 3400 1550
Wire Wire Line
	3400 1550 3550 1550
Wire Wire Line
	6300 1850 5450 1850
Wire Wire Line
	5450 1850 5450 1700
Wire Wire Line
	5450 1700 5000 1700
Wire Wire Line
	6150 2750 5000 2750
Wire Wire Line
	6150 2150 6150 2750
Wire Wire Line
	6150 2250 6300 2250
Wire Wire Line
	6300 2150 6150 2150
Connection ~ 6150 2250
Wire Wire Line
	5000 2000 5750 2000
Wire Wire Line
	5750 2000 5750 2450
Wire Wire Line
	5750 2450 6300 2450
Wire Wire Line
	6300 2550 6200 2550
Wire Wire Line
	6200 2550 6200 2450
Connection ~ 6200 2450
Wire Wire Line
	3550 2300 3250 2300
Wire Wire Line
	5000 2300 5200 2300
Wire Wire Line
	3550 2450 3450 2450
Wire Wire Line
	3450 2450 3450 2300
Connection ~ 3450 2300
Wire Wire Line
	5000 2450 5100 2450
Wire Wire Line
	5100 2450 5100 2300
Connection ~ 5100 2300
Wire Wire Line
	6300 2750 6200 2750
Wire Wire Line
	6200 2750 6200 3450
Wire Wire Line
	6200 2850 6300 2850
Wire Wire Line
	3550 2150 2650 2150
Wire Wire Line
	2650 2150 2650 3450
Wire Wire Line
	2650 3450 6200 3450
Connection ~ 6200 2850
Wire Wire Line
	3550 2900 2800 2900
Wire Wire Line
	2800 2900 2800 1850
Wire Wire Line
	2500 1850 3550 1850
$Comp
L R Rsense1
U 1 1 59614D6A
P 2350 1850
F 0 "Rsense1" V 2430 1850 50  0000 C CNN
F 1 "0.03R" V 2350 1850 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P20.32mm_Horizontal" V 2280 1850 50  0001 C CNN
F 3 "" H 2350 1850 50  0001 C CNN
	1    2350 1850
	0    1    1    0   
$EndComp
Connection ~ 2800 1850
$Comp
L GND #PWR04
U 1 1 59614DFE
P 2100 1850
F 0 "#PWR04" H 2100 1600 50  0001 C CNN
F 1 "GND" H 2100 1700 50  0000 C CNN
F 2 "" H 2100 1850 50  0001 C CNN
F 3 "" H 2100 1850 50  0001 C CNN
	1    2100 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 1850 2200 1850
$Comp
L +5V #PWR05
U 1 1 59615067
P 6150 1450
F 0 "#PWR05" H 6150 1300 50  0001 C CNN
F 1 "+5V" H 6150 1590 50  0000 C CNN
F 2 "" H 6150 1450 50  0001 C CNN
F 3 "" H 6150 1450 50  0001 C CNN
	1    6150 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 1450 6150 1550
Wire Wire Line
	6150 1550 6300 1550
Wire Wire Line
	5000 2150 5550 2150
Wire Wire Line
	5550 2150 5550 2600
Wire Wire Line
	5000 2600 5650 2600
$Comp
L VCC #PWR06
U 1 1 5961531A
P 5650 2600
F 0 "#PWR06" H 5650 2450 50  0001 C CNN
F 1 "VCC" H 5650 2750 50  0000 C CNN
F 2 "" H 5650 2600 50  0001 C CNN
F 3 "" H 5650 2600 50  0001 C CNN
	1    5650 2600
	0    1    1    0   
$EndComp
Connection ~ 5550 2600
$Comp
L VCC #PWR07
U 1 1 59615522
P 2150 4000
F 0 "#PWR07" H 2150 3850 50  0001 C CNN
F 1 "VCC" H 2150 4150 50  0000 C CNN
F 2 "" H 2150 4000 50  0001 C CNN
F 3 "" H 2150 4000 50  0001 C CNN
	1    2150 4000
	1    0    0    -1  
$EndComp
$Comp
L D_ALT D1
U 1 1 5961553C
P 2150 4300
F 0 "D1" H 2150 4400 50  0000 C CNN
F 1 "1N4148" H 2150 4200 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 2150 4300 50  0001 C CNN
F 3 "" H 2150 4300 50  0001 C CNN
	1    2150 4300
	0    -1   -1   0   
$EndComp
$Comp
L D_ALT D2
U 1 1 596155C9
P 2150 4750
F 0 "D2" H 2150 4850 50  0000 C CNN
F 1 "1N4148" H 2150 4650 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 2150 4750 50  0001 C CNN
F 3 "" H 2150 4750 50  0001 C CNN
	1    2150 4750
	0    -1   -1   0   
$EndComp
$Comp
L R Rp1
U 1 1 596155F3
P 2450 4550
F 0 "Rp1" V 2530 4550 50  0000 C CNN
F 1 "100R" V 2450 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2380 4550 50  0001 C CNN
F 3 "" H 2450 4550 50  0001 C CNN
	1    2450 4550
	0    1    1    0   
$EndComp
$Comp
L C Cp1
U 1 1 5961562C
P 2850 4550
F 0 "Cp1" H 2875 4650 50  0000 L CNN
F 1 "10nC" H 2875 4450 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 2888 4400 50  0001 C CNN
F 3 "" H 2850 4550 50  0001 C CNN
	1    2850 4550
	0    1    1    0   
$EndComp
$Comp
L C Cboot1
U 1 1 5961567C
P 1900 5100
F 0 "Cboot1" H 1925 5200 50  0000 L CNN
F 1 "220nC" H 1925 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 1938 4950 50  0001 C CNN
F 3 "" H 1900 5100 50  0001 C CNN
	1    1900 5100
	0    1    1    0   
$EndComp
Wire Wire Line
	1750 5100 1750 4100
Wire Wire Line
	2150 4450 2150 4600
Wire Wire Line
	2150 4900 2150 5100
Wire Wire Line
	2050 5100 2550 5100
Wire Wire Line
	2300 4550 2150 4550
Connection ~ 2150 4550
Wire Wire Line
	2600 4550 2700 4550
Wire Wire Line
	3000 4550 3300 4550
Connection ~ 2150 5100
Text Label 3300 4550 0    60   ~ 0
VCP1
Text Label 2550 5100 0    60   ~ 0
VBOOT1
Text Label 5200 1850 0    60   ~ 0
VCP1
Text Label 5300 2900 0    60   ~ 0
VBOOT1
Wire Wire Line
	5200 1850 5000 1850
Wire Wire Line
	5300 2900 5000 2900
Text HLabel 5250 3050 2    60   Input ~ 0
BRAKE_1
Wire Wire Line
	5000 3050 5250 3050
Text Notes 600  750  0    60   ~ 0
26
Text Label 3100 2000 0    60   ~ 0
RCOFF1
Wire Wire Line
	3100 2000 3550 2000
Text Label 4900 3950 0    60   ~ 0
RCOFF1
$Comp
L C Coff1
U 1 1 59618797
P 4550 3950
F 0 "Coff1" H 4575 4050 50  0000 L CNN
F 1 "1nC" H 4575 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 4588 3800 50  0001 C CNN
F 3 "" H 4550 3950 50  0001 C CNN
	1    4550 3950
	0    1    1    0   
$EndComp
$Comp
L R Roff1
U 1 1 59618982
P 4550 4200
F 0 "Roff1" V 4630 4200 50  0000 C CNN
F 1 "33kR" V 4550 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4480 4200 50  0001 C CNN
F 3 "" H 4550 4200 50  0001 C CNN
	1    4550 4200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR08
U 1 1 596189C5
P 4200 4300
F 0 "#PWR08" H 4200 4050 50  0001 C CNN
F 1 "GND" H 4200 4150 50  0000 C CNN
F 2 "" H 4200 4300 50  0001 C CNN
F 3 "" H 4200 4300 50  0001 C CNN
	1    4200 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3950 4900 3950
Wire Wire Line
	4700 4200 4800 4200
Wire Wire Line
	4800 4200 4800 3950
Connection ~ 4800 3950
Wire Wire Line
	4400 3950 4200 3950
Wire Wire Line
	4200 3950 4200 4300
Wire Wire Line
	4400 4200 4200 4200
Connection ~ 4200 4200
Text Label 3050 2750 0    60   ~ 0
RCPULSE1
Text Label 6400 3950 0    60   ~ 0
RCPULSE1
Wire Wire Line
	3050 2750 3550 2750
$Comp
L C Cpul1
U 1 1 59619FC3
P 5950 3950
F 0 "Cpul1" H 5975 4050 50  0000 L CNN
F 1 "10nC" H 5975 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5988 3800 50  0001 C CNN
F 3 "" H 5950 3950 50  0001 C CNN
	1    5950 3950
	0    1    1    0   
$EndComp
$Comp
L R Rpul1
U 1 1 5961A03C
P 5950 4200
F 0 "Rpul1" V 6030 4200 50  0000 C CNN
F 1 "47kR" V 5950 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5880 4200 50  0001 C CNN
F 3 "" H 5950 4200 50  0001 C CNN
	1    5950 4200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 5961A08D
P 5600 4300
F 0 "#PWR09" H 5600 4050 50  0001 C CNN
F 1 "GND" H 5600 4150 50  0000 C CNN
F 2 "" H 5600 4300 50  0001 C CNN
F 3 "" H 5600 4300 50  0001 C CNN
	1    5600 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4300 5600 3950
Wire Wire Line
	5600 3950 5800 3950
Wire Wire Line
	6100 3950 6400 3950
Wire Wire Line
	5800 4200 5600 4200
Connection ~ 5600 4200
Wire Wire Line
	6100 4200 6250 4200
Wire Wire Line
	6250 4200 6250 3950
Connection ~ 6250 3950
Text HLabel 3200 2600 0    60   Input ~ 0
TACHO_1
Wire Wire Line
	3200 2600 3550 2600
Text HLabel 3250 1700 0    60   Input ~ 0
DIAG_1
Wire Wire Line
	3250 1700 3550 1700
Text HLabel 3150 3050 0    60   Input ~ 0
DIR_1
Wire Wire Line
	3150 3050 3550 3050
Text Label 5300 3200 0    60   ~ 0
Vref
Wire Wire Line
	5000 3200 5300 3200
Text Label 5000 4800 0    60   ~ 0
Vref
$Comp
L R R1
U 1 1 5961BC60
P 4550 4800
F 0 "R1" V 4630 4800 50  0000 C CNN
F 1 "R" V 4550 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4480 4800 50  0001 C CNN
F 3 "" H 4550 4800 50  0001 C CNN
	1    4550 4800
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5961BC99
P 5000 5100
F 0 "R2" V 5080 5100 50  0000 C CNN
F 1 "R" V 5000 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4930 5100 50  0001 C CNN
F 3 "" H 5000 5100 50  0001 C CNN
	1    5000 5100
	-1   0    0    1   
$EndComp
$Comp
L C C1
U 1 1 5961BF92
P 5250 5100
F 0 "C1" H 5275 5200 50  0000 L CNN
F 1 "C" H 5275 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5288 4950 50  0001 C CNN
F 3 "" H 5250 5100 50  0001 C CNN
	1    5250 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5961BFD9
P 5000 5350
F 0 "#PWR010" H 5000 5100 50  0001 C CNN
F 1 "GND" H 5000 5200 50  0000 C CNN
F 2 "" H 5000 5350 50  0001 C CNN
F 3 "" H 5000 5350 50  0001 C CNN
	1    5000 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 4800 5000 4800
Wire Wire Line
	5000 4800 5000 4950
Wire Wire Line
	5000 5350 5000 5250
Wire Wire Line
	5250 4950 5250 4850
Wire Wire Line
	5250 4850 5000 4850
Connection ~ 5000 4850
Wire Wire Line
	5250 5250 5250 5300
Wire Wire Line
	5250 5300 5000 5300
Connection ~ 5000 5300
$Comp
L R Ren1
U 1 1 59624BB3
P 2650 3200
F 0 "Ren1" V 2730 3200 50  0000 C CNN
F 1 "100kR" V 2650 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 2580 3200 50  0001 C CNN
F 3 "" H 2650 3200 50  0001 C CNN
	1    2650 3200
	0    1    1    0   
$EndComp
$Comp
L C Cen1
U 1 1 59624D3C
P 3050 3400
F 0 "Cen1" H 3075 3500 50  0000 L CNN
F 1 "5,6nC" H 3075 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3088 3250 50  0001 C CNN
F 3 "" H 3050 3400 50  0001 C CNN
	1    3050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3200 3550 3200
Text Label 3350 1700 0    60   ~ 0
DIAG1
Text Label 3150 3200 0    60   ~ 0
DIAG1
Text HLabel 2200 3200 0    60   Input ~ 0
EN_1
Wire Wire Line
	2200 3200 2500 3200
Wire Wire Line
	1750 4100 2150 4100
Wire Wire Line
	2150 4000 2150 4150
Connection ~ 2150 4100
$Comp
L GND #PWR011
U 1 1 596275DF
P 3050 3700
F 0 "#PWR011" H 3050 3450 50  0001 C CNN
F 1 "GND" H 3050 3550 50  0000 C CNN
F 2 "" H 3050 3700 50  0001 C CNN
F 3 "" H 3050 3700 50  0001 C CNN
	1    3050 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3200 3050 3250
Connection ~ 3050 3200
Wire Wire Line
	3050 3550 3050 3700
Text Label 3300 2600 0    60   ~ 0
TACHO1
Text Label 3850 5200 0    60   ~ 0
TACHO1
$Comp
L R Rdd1
U 1 1 5962811E
P 3850 5450
F 0 "Rdd1" V 3930 5450 50  0000 C CNN
F 1 "1kR" V 3850 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 3780 5450 50  0001 C CNN
F 3 "" H 3850 5450 50  0001 C CNN
	1    3850 5450
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR012
U 1 1 59629D17
P 3850 5750
F 0 "#PWR012" H 3850 5600 50  0001 C CNN
F 1 "+3.3V" H 3850 5890 50  0000 C CNN
F 2 "" H 3850 5750 50  0001 C CNN
F 3 "" H 3850 5750 50  0001 C CNN
	1    3850 5750
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 5200 3850 5300
Wire Wire Line
	3850 5600 3850 5750
Text Notes 1600 1150 0    60   ~ 0
régler vref et rsense
$Comp
L +3.3V #PWR013
U 1 1 5962BA47
P 4200 4800
F 0 "#PWR013" H 4200 4650 50  0001 C CNN
F 1 "+3.3V" H 4200 4940 50  0000 C CNN
F 2 "" H 4200 4800 50  0001 C CNN
F 3 "" H 4200 4800 50  0001 C CNN
	1    4200 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4200 4800 4400 4800
$Comp
L L6235D U2
U 1 1 59B492F0
P 11450 2250
F 0 "U2" H 11450 2250 60  0000 C CNN
F 1 "L6235D" H 11450 2450 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-24W_7.5x15.4mm_Pitch1.27mm" H 11150 2500 60  0001 C CNN
F 3 "" H 11150 2500 60  0001 C CNN
	1    11450 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 59B492F6
P 12350 2200
F 0 "#PWR014" H 12350 1950 50  0001 C CNN
F 1 "GND" H 12350 2050 50  0000 C CNN
F 2 "" H 12350 2200 50  0001 C CNN
F 3 "" H 12350 2200 50  0001 C CNN
	1    12350 2200
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR015
U 1 1 59B492FC
P 10400 2200
F 0 "#PWR015" H 10400 1950 50  0001 C CNN
F 1 "GND" H 10400 2050 50  0000 C CNN
F 2 "" H 10400 2200 50  0001 C CNN
F 3 "" H 10400 2200 50  0001 C CNN
	1    10400 2200
	0    1    1    0   
$EndComp
$Comp
L Maxon-200142 K2
U 1 1 59B49302
P 14050 1650
F 0 "K2" H 13950 2000 60  0000 C CNN
F 1 "Maxon-200142" H 14000 400 60  0000 C CNN
F 2 "maxon:Molex-52207-0433" H 13550 1750 60  0001 C CNN
F 3 "" H 13550 1750 60  0000 C CNN
	1    14050 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 59B49308
P 13200 1850
F 0 "#PWR016" H 13200 1600 50  0001 C CNN
F 1 "GND" H 13200 1700 50  0000 C CNN
F 2 "" H 13200 1850 50  0001 C CNN
F 3 "" H 13200 1850 50  0001 C CNN
	1    13200 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	13450 1850 13200 1850
Wire Wire Line
	13450 1550 12650 1550
Wire Wire Line
	12650 1550 12650 1450
Wire Wire Line
	12650 1450 12150 1450
Wire Wire Line
	13450 1650 12800 1650
Wire Wire Line
	12800 1650 12800 1250
Wire Wire Line
	12800 1250 10550 1250
Wire Wire Line
	10550 1250 10550 1450
Wire Wire Line
	10550 1450 10700 1450
Wire Wire Line
	13450 1750 12600 1750
Wire Wire Line
	12600 1750 12600 1600
Wire Wire Line
	12600 1600 12150 1600
Wire Wire Line
	13300 2650 12150 2650
Wire Wire Line
	13300 2050 13300 2650
Wire Wire Line
	13300 2150 13450 2150
Wire Wire Line
	13450 2050 13300 2050
Connection ~ 13300 2150
Wire Wire Line
	12150 1900 12900 1900
Wire Wire Line
	12900 1900 12900 2350
Wire Wire Line
	12900 2350 13450 2350
Wire Wire Line
	13450 2450 13350 2450
Wire Wire Line
	13350 2450 13350 2350
Connection ~ 13350 2350
Wire Wire Line
	10700 2200 10400 2200
Wire Wire Line
	12150 2200 12350 2200
Wire Wire Line
	10700 2350 10600 2350
Wire Wire Line
	10600 2350 10600 2200
Connection ~ 10600 2200
Wire Wire Line
	12150 2350 12250 2350
Wire Wire Line
	12250 2350 12250 2200
Connection ~ 12250 2200
Wire Wire Line
	13450 2650 13350 2650
Wire Wire Line
	13350 2650 13350 3350
Wire Wire Line
	13350 2750 13450 2750
Wire Wire Line
	10700 2050 9800 2050
Wire Wire Line
	9800 2050 9800 3350
Wire Wire Line
	9800 3350 13350 3350
Connection ~ 13350 2750
Wire Wire Line
	10700 2800 9950 2800
Wire Wire Line
	9950 2800 9950 1750
Wire Wire Line
	9650 1750 10700 1750
$Comp
L R Rsense2
U 1 1 59B49337
P 9500 1750
F 0 "Rsense2" V 9580 1750 50  0000 C CNN
F 1 "0.03R" V 9500 1750 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P20.32mm_Horizontal" V 9430 1750 50  0001 C CNN
F 3 "" H 9500 1750 50  0001 C CNN
	1    9500 1750
	0    1    1    0   
$EndComp
Connection ~ 9950 1750
$Comp
L GND #PWR017
U 1 1 59B4933E
P 9250 1750
F 0 "#PWR017" H 9250 1500 50  0001 C CNN
F 1 "GND" H 9250 1600 50  0000 C CNN
F 2 "" H 9250 1750 50  0001 C CNN
F 3 "" H 9250 1750 50  0001 C CNN
	1    9250 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	9250 1750 9350 1750
$Comp
L +5V #PWR018
U 1 1 59B49345
P 13300 1350
F 0 "#PWR018" H 13300 1200 50  0001 C CNN
F 1 "+5V" H 13300 1490 50  0000 C CNN
F 2 "" H 13300 1350 50  0001 C CNN
F 3 "" H 13300 1350 50  0001 C CNN
	1    13300 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	13300 1350 13300 1450
Wire Wire Line
	13300 1450 13450 1450
Wire Wire Line
	12150 2050 12700 2050
Wire Wire Line
	12700 2050 12700 2500
Wire Wire Line
	12150 2500 12800 2500
$Comp
L VCC #PWR019
U 1 1 59B49350
P 12800 2500
F 0 "#PWR019" H 12800 2350 50  0001 C CNN
F 1 "VCC" H 12800 2650 50  0000 C CNN
F 2 "" H 12800 2500 50  0001 C CNN
F 3 "" H 12800 2500 50  0001 C CNN
	1    12800 2500
	0    1    1    0   
$EndComp
Connection ~ 12700 2500
$Comp
L VCC #PWR020
U 1 1 59B49357
P 9300 3900
F 0 "#PWR020" H 9300 3750 50  0001 C CNN
F 1 "VCC" H 9300 4050 50  0000 C CNN
F 2 "" H 9300 3900 50  0001 C CNN
F 3 "" H 9300 3900 50  0001 C CNN
	1    9300 3900
	1    0    0    -1  
$EndComp
$Comp
L D_ALT D3
U 1 1 59B4935D
P 9300 4200
F 0 "D3" H 9300 4300 50  0000 C CNN
F 1 "1N4148" H 9300 4100 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 9300 4200 50  0001 C CNN
F 3 "" H 9300 4200 50  0001 C CNN
	1    9300 4200
	0    -1   -1   0   
$EndComp
$Comp
L D_ALT D4
U 1 1 59B49363
P 9300 4650
F 0 "D4" H 9300 4750 50  0000 C CNN
F 1 "1N4148" H 9300 4550 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 9300 4650 50  0001 C CNN
F 3 "" H 9300 4650 50  0001 C CNN
	1    9300 4650
	0    -1   -1   0   
$EndComp
$Comp
L R Rp2
U 1 1 59B49369
P 9600 4450
F 0 "Rp2" V 9680 4450 50  0000 C CNN
F 1 "100R" V 9600 4450 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 9530 4450 50  0001 C CNN
F 3 "" H 9600 4450 50  0001 C CNN
	1    9600 4450
	0    1    1    0   
$EndComp
$Comp
L C Cp2
U 1 1 59B4936F
P 10000 4450
F 0 "Cp2" H 10025 4550 50  0000 L CNN
F 1 "10nC" H 10025 4350 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 10038 4300 50  0001 C CNN
F 3 "" H 10000 4450 50  0001 C CNN
	1    10000 4450
	0    1    1    0   
$EndComp
$Comp
L C Cboot2
U 1 1 59B49375
P 9050 5000
F 0 "Cboot2" H 9075 5100 50  0000 L CNN
F 1 "220nC" H 9075 4900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 9088 4850 50  0001 C CNN
F 3 "" H 9050 5000 50  0001 C CNN
	1    9050 5000
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 5000 8900 4000
Wire Wire Line
	9300 4350 9300 4500
Wire Wire Line
	9300 4800 9300 5000
Wire Wire Line
	9200 5000 9700 5000
Wire Wire Line
	9450 4450 9300 4450
Connection ~ 9300 4450
Wire Wire Line
	9750 4450 9850 4450
Wire Wire Line
	10150 4450 10450 4450
Connection ~ 9300 5000
Text Label 10450 4450 0    60   ~ 0
VCP2
Text Label 9700 5000 0    60   ~ 0
VBOOT2
Text Label 12350 1750 0    60   ~ 0
VCP2
Text Label 12450 2800 0    60   ~ 0
VBOOT2
Wire Wire Line
	12350 1750 12150 1750
Wire Wire Line
	12450 2800 12150 2800
Text HLabel 12400 2950 2    60   Input ~ 0
BRAKE_2
Wire Wire Line
	12150 2950 12400 2950
Text Label 10250 1900 0    60   ~ 0
RCOFF2
Wire Wire Line
	10250 1900 10700 1900
Text Label 12050 3850 0    60   ~ 0
RCOFF2
$Comp
L C Coff2
U 1 1 59B4938F
P 11700 3850
F 0 "Coff2" H 11725 3950 50  0000 L CNN
F 1 "1nC" H 11725 3750 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 11738 3700 50  0001 C CNN
F 3 "" H 11700 3850 50  0001 C CNN
	1    11700 3850
	0    1    1    0   
$EndComp
$Comp
L R Roff2
U 1 1 59B49395
P 11700 4100
F 0 "Roff2" V 11780 4100 50  0000 C CNN
F 1 "33kR" V 11700 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 11630 4100 50  0001 C CNN
F 3 "" H 11700 4100 50  0001 C CNN
	1    11700 4100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR021
U 1 1 59B4939B
P 11350 4200
F 0 "#PWR021" H 11350 3950 50  0001 C CNN
F 1 "GND" H 11350 4050 50  0000 C CNN
F 2 "" H 11350 4200 50  0001 C CNN
F 3 "" H 11350 4200 50  0001 C CNN
	1    11350 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	11850 3850 12050 3850
Wire Wire Line
	11850 4100 11950 4100
Wire Wire Line
	11950 4100 11950 3850
Connection ~ 11950 3850
Wire Wire Line
	11550 3850 11350 3850
Wire Wire Line
	11350 3850 11350 4200
Wire Wire Line
	11550 4100 11350 4100
Connection ~ 11350 4100
Text Label 10200 2650 0    60   ~ 0
RCPULSE2
Text Label 13550 3850 0    60   ~ 0
RCPULSE2
Wire Wire Line
	10200 2650 10700 2650
$Comp
L C Cpul2
U 1 1 59B493AC
P 13100 3850
F 0 "Cpul2" H 13125 3950 50  0000 L CNN
F 1 "10nC" H 13125 3750 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 13138 3700 50  0001 C CNN
F 3 "" H 13100 3850 50  0001 C CNN
	1    13100 3850
	0    1    1    0   
$EndComp
$Comp
L R Rpul2
U 1 1 59B493B2
P 13100 4100
F 0 "Rpul2" V 13180 4100 50  0000 C CNN
F 1 "47kR" V 13100 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 13030 4100 50  0001 C CNN
F 3 "" H 13100 4100 50  0001 C CNN
	1    13100 4100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR022
U 1 1 59B493B8
P 12750 4200
F 0 "#PWR022" H 12750 3950 50  0001 C CNN
F 1 "GND" H 12750 4050 50  0000 C CNN
F 2 "" H 12750 4200 50  0001 C CNN
F 3 "" H 12750 4200 50  0001 C CNN
	1    12750 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	12750 4200 12750 3850
Wire Wire Line
	12750 3850 12950 3850
Wire Wire Line
	13250 3850 13550 3850
Wire Wire Line
	12950 4100 12750 4100
Connection ~ 12750 4100
Wire Wire Line
	13250 4100 13400 4100
Wire Wire Line
	13400 4100 13400 3850
Connection ~ 13400 3850
Text HLabel 10350 2500 0    60   Input ~ 0
TACHO_2
Wire Wire Line
	10350 2500 10700 2500
Text HLabel 10400 1600 0    60   Input ~ 0
DIAG_2
Wire Wire Line
	10400 1600 10700 1600
Text HLabel 10300 2950 0    60   Input ~ 0
DIR_2
Wire Wire Line
	10300 2950 10700 2950
Text Label 12450 3100 0    60   ~ 0
Vref
Wire Wire Line
	12150 3100 12450 3100
Text Label 12150 4700 0    60   ~ 0
Vref
$Comp
L R R3
U 1 1 59B493CF
P 11700 4700
F 0 "R3" V 11780 4700 50  0000 C CNN
F 1 "R" V 11700 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 11630 4700 50  0001 C CNN
F 3 "" H 11700 4700 50  0001 C CNN
	1    11700 4700
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 59B493D5
P 12150 5000
F 0 "R4" V 12230 5000 50  0000 C CNN
F 1 "R" V 12150 5000 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 12080 5000 50  0001 C CNN
F 3 "" H 12150 5000 50  0001 C CNN
	1    12150 5000
	-1   0    0    1   
$EndComp
$Comp
L C C2
U 1 1 59B493DB
P 12400 5000
F 0 "C2" H 12425 5100 50  0000 L CNN
F 1 "C" H 12425 4900 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 12438 4850 50  0001 C CNN
F 3 "" H 12400 5000 50  0001 C CNN
	1    12400 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 59B493E1
P 12150 5250
F 0 "#PWR023" H 12150 5000 50  0001 C CNN
F 1 "GND" H 12150 5100 50  0000 C CNN
F 2 "" H 12150 5250 50  0001 C CNN
F 3 "" H 12150 5250 50  0001 C CNN
	1    12150 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	11850 4700 12150 4700
Wire Wire Line
	12150 4700 12150 4850
Wire Wire Line
	12150 5250 12150 5150
Wire Wire Line
	12400 4850 12400 4750
Wire Wire Line
	12400 4750 12150 4750
Connection ~ 12150 4750
Wire Wire Line
	12400 5150 12400 5200
Wire Wire Line
	12400 5200 12150 5200
Connection ~ 12150 5200
$Comp
L R Ren2
U 1 1 59B493F0
P 9800 3100
F 0 "Ren2" V 9880 3100 50  0000 C CNN
F 1 "100kR" V 9800 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 9730 3100 50  0001 C CNN
F 3 "" H 9800 3100 50  0001 C CNN
	1    9800 3100
	0    1    1    0   
$EndComp
$Comp
L C Cen2
U 1 1 59B493F6
P 10200 3300
F 0 "Cen2" H 10225 3400 50  0000 L CNN
F 1 "5,6nC" H 10225 3200 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 10238 3150 50  0001 C CNN
F 3 "" H 10200 3300 50  0001 C CNN
	1    10200 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 3100 10700 3100
Text Label 10500 1600 0    60   ~ 0
DIAG2
Text Label 10300 3100 0    60   ~ 0
DIAG2
Text HLabel 9350 3100 0    60   Input ~ 0
EN_2
Wire Wire Line
	9350 3100 9650 3100
Wire Wire Line
	8900 4000 9300 4000
Wire Wire Line
	9300 3900 9300 4050
Connection ~ 9300 4000
$Comp
L GND #PWR024
U 1 1 59B49404
P 10200 3600
F 0 "#PWR024" H 10200 3350 50  0001 C CNN
F 1 "GND" H 10200 3450 50  0000 C CNN
F 2 "" H 10200 3600 50  0001 C CNN
F 3 "" H 10200 3600 50  0001 C CNN
	1    10200 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 3100 10200 3150
Connection ~ 10200 3100
Wire Wire Line
	10200 3450 10200 3600
Text Label 10450 2500 0    60   ~ 0
TACHO2
Text Label 11000 5100 0    60   ~ 0
TACHO2
$Comp
L R Rdd2
U 1 1 59B4940F
P 11000 5350
F 0 "Rdd2" V 11080 5350 50  0000 C CNN
F 1 "1kR" V 11000 5350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 10930 5350 50  0001 C CNN
F 3 "" H 11000 5350 50  0001 C CNN
	1    11000 5350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR025
U 1 1 59B49415
P 11000 5650
F 0 "#PWR025" H 11000 5500 50  0001 C CNN
F 1 "+3.3V" H 11000 5790 50  0000 C CNN
F 2 "" H 11000 5650 50  0001 C CNN
F 3 "" H 11000 5650 50  0001 C CNN
	1    11000 5650
	-1   0    0    1   
$EndComp
Wire Wire Line
	11000 5100 11000 5200
Wire Wire Line
	11000 5500 11000 5650
Text Notes 8750 1050 0    60   ~ 0
régler vref et rsense
$Comp
L +3.3V #PWR026
U 1 1 59B4941E
P 11350 4700
F 0 "#PWR026" H 11350 4550 50  0001 C CNN
F 1 "+3.3V" H 11350 4840 50  0000 C CNN
F 2 "" H 11350 4700 50  0001 C CNN
F 3 "" H 11350 4700 50  0001 C CNN
	1    11350 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11350 4700 11550 4700
$EndSCHEMATC
