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
LIBS:stm32f042k6tx
LIBS:2
LIBS:mcp2562
LIBS:Propulsion_control
LIBS:mc33926
LIBS:driver-moteurs-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L B_Plug_5mm U3
U 1 1 59F75CCE
P 2900 2450
F 0 "U3" H 2900 2850 60  0000 C CNN
F 1 "B_Plug_5mm" H 2900 2950 60  0000 C CNN
F 2 "" H 2900 2450 60  0001 C CNN
F 3 "" H 2900 2450 60  0000 C CNN
	1    2900 2450
	1    0    0    -1  
$EndComp
$Comp
L CP C4
U 1 1 59F75D85
P 4150 2850
F 0 "C4" H 4175 2950 50  0000 L CNN
F 1 "CP" H 4175 2750 50  0000 L CNN
F 2 "" H 4188 2700 50  0001 C CNN
F 3 "" H 4150 2850 50  0000 C CNN
	1    4150 2850
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 59F75DB2
P 4550 2850
F 0 "C5" H 4575 2950 50  0000 L CNN
F 1 "C" H 4575 2750 50  0000 L CNN
F 2 "" H 4588 2700 50  0001 C CNN
F 3 "" H 4550 2850 50  0000 C CNN
	1    4550 2850
	1    0    0    -1  
$EndComp
$Comp
L D D2
U 1 1 59F75DD1
P 3850 2850
F 0 "D2" H 3850 2950 50  0000 C CNN
F 1 "D" H 3850 2750 50  0000 C CNN
F 2 "" H 3850 2850 50  0001 C CNN
F 3 "" H 3850 2850 50  0000 C CNN
	1    3850 2850
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 59F75E2C
P 5500 2850
F 0 "D3" H 5500 2950 50  0000 C CNN
F 1 "LED" H 5500 2750 50  0000 C CNN
F 2 "" H 5500 2850 50  0001 C CNN
F 3 "" H 5500 2850 50  0000 C CNN
	1    5500 2850
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 59F75E9D
P 5050 2400
F 0 "R6" V 5130 2400 50  0000 C CNN
F 1 "2kR" V 5050 2400 50  0000 C CNN
F 2 "" V 4980 2400 50  0001 C CNN
F 3 "" H 5050 2400 50  0000 C CNN
	1    5050 2400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR34
U 1 1 59F7601E
P 3850 3250
F 0 "#PWR34" H 3850 3000 50  0001 C CNN
F 1 "GND" H 3850 3100 50  0000 C CNN
F 2 "" H 3850 3250 50  0000 C CNN
F 3 "" H 3850 3250 50  0000 C CNN
	1    3850 3250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR35
U 1 1 59F761AB
P 4750 2200
F 0 "#PWR35" H 4750 2050 50  0001 C CNN
F 1 "VCC" H 4750 2350 50  0000 C CNN
F 2 "" H 4750 2200 50  0000 C CNN
F 3 "" H 4750 2200 50  0000 C CNN
	1    4750 2200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG2
U 1 1 59F761CD
P 4450 2200
F 0 "#FLG2" H 4450 2295 50  0001 C CNN
F 1 "PWR_FLAG" H 4450 2380 50  0000 C CNN
F 2 "" H 4450 2200 50  0000 C CNN
F 3 "" H 4450 2200 50  0000 C CNN
	1    4450 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR32
U 1 1 59F763B8
P 3000 3200
F 0 "#PWR32" H 3000 2950 50  0001 C CNN
F 1 "GND" H 3000 3050 50  0000 C CNN
F 2 "" H 3000 3200 50  0000 C CNN
F 3 "" H 3000 3200 50  0000 C CNN
	1    3000 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR38
U 1 1 59F763D8
P 5500 3200
F 0 "#PWR38" H 5500 2950 50  0001 C CNN
F 1 "GND" H 5500 3050 50  0000 C CNN
F 2 "" H 5500 3200 50  0000 C CNN
F 3 "" H 5500 3200 50  0000 C CNN
	1    5500 3200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG1
U 1 1 59F76416
P 3250 3200
F 0 "#FLG1" H 3250 3295 50  0001 C CNN
F 1 "PWR_FLAG" H 3250 3380 50  0000 C CNN
F 2 "" H 3250 3200 50  0000 C CNN
F 3 "" H 3250 3200 50  0000 C CNN
	1    3250 3200
	-1   0    0    1   
$EndComp
$Comp
L LM317_SOT223 U5
U 1 1 59F76469
P 7850 2500
F 0 "U5" H 7850 2800 50  0000 C CNN
F 1 "LM317_SOT223" H 7900 2250 50  0000 L CNN
F 2 "" H 7850 2500 50  0001 C CNN
F 3 "" H 7850 2500 50  0000 C CNN
	1    7850 2500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR40
U 1 1 59F764CC
P 7000 2250
F 0 "#PWR40" H 7000 2100 50  0001 C CNN
F 1 "VCC" H 7000 2400 50  0000 C CNN
F 2 "" H 7000 2250 50  0000 C CNN
F 3 "" H 7000 2250 50  0000 C CNN
	1    7000 2250
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 59F7651D
P 7850 3400
F 0 "R8" V 7930 3400 50  0000 C CNN
F 1 "R" V 7850 3400 50  0000 C CNN
F 2 "" V 7780 3400 50  0001 C CNN
F 3 "" H 7850 3400 50  0000 C CNN
	1    7850 3400
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 59F765B0
P 8650 2950
F 0 "R9" V 8730 2950 50  0000 C CNN
F 1 "R" V 8650 2950 50  0000 C CNN
F 2 "" V 8580 2950 50  0001 C CNN
F 3 "" H 8650 2950 50  0000 C CNN
	1    8650 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2400 3400 2400
Wire Wire Line
	3600 2400 4900 2400
Wire Wire Line
	3850 2400 3850 2700
Wire Wire Line
	4550 2400 4550 2700
Connection ~ 3850 2400
Wire Wire Line
	4150 2700 4150 2400
Connection ~ 4150 2400
Wire Wire Line
	3850 3100 4550 3100
Wire Wire Line
	4550 3100 4550 3000
Connection ~ 3850 3100
Wire Wire Line
	3850 3000 3850 3250
Wire Wire Line
	4150 3000 4150 3100
Connection ~ 4150 3100
Connection ~ 4550 2400
Wire Wire Line
	4750 2400 4750 2200
Connection ~ 4750 2400
Wire Wire Line
	4450 2200 4450 2250
Wire Wire Line
	4450 2250 4750 2250
Connection ~ 4750 2250
Wire Wire Line
	5200 2400 5500 2400
Wire Wire Line
	5500 2400 5500 2650
Wire Wire Line
	5500 3200 5500 3050
Wire Wire Line
	3000 2500 3000 3200
Wire Wire Line
	3000 3000 3250 3000
Wire Wire Line
	3250 3000 3250 3200
Connection ~ 3000 3000
Wire Wire Line
	7450 2350 7000 2350
Wire Wire Line
	7000 2350 7000 2250
Wire Wire Line
	8650 3100 7850 3100
Wire Wire Line
	7850 2850 7850 3250
Connection ~ 7850 3100
$Comp
L GND #PWR42
U 1 1 59F76711
P 7850 3700
F 0 "#PWR42" H 7850 3450 50  0001 C CNN
F 1 "GND" H 7850 3550 50  0000 C CNN
F 2 "" H 7850 3700 50  0000 C CNN
F 3 "" H 7850 3700 50  0000 C CNN
	1    7850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3550 7850 3700
$Comp
L C C7
U 1 1 59F76771
P 9300 2750
F 0 "C7" H 9325 2850 50  0000 L CNN
F 1 "C" H 9325 2650 50  0000 L CNN
F 2 "" H 9338 2600 50  0001 C CNN
F 3 "" H 9300 2750 50  0000 C CNN
	1    9300 2750
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 59F7681A
P 9900 2350
F 0 "R10" V 9980 2350 50  0000 C CNN
F 1 "470R" V 9900 2350 50  0000 C CNN
F 2 "" V 9830 2350 50  0001 C CNN
F 3 "" H 9900 2350 50  0000 C CNN
	1    9900 2350
	0    1    1    0   
$EndComp
$Comp
L LED D5
U 1 1 59F7690E
P 10600 2750
F 0 "D5" H 10600 2850 50  0000 C CNN
F 1 "LED" H 10600 2650 50  0000 C CNN
F 2 "" H 10600 2750 50  0001 C CNN
F 3 "" H 10600 2750 50  0000 C CNN
	1    10600 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8250 2350 9750 2350
Wire Wire Line
	8650 2350 8650 2800
Connection ~ 9300 2350
Wire Wire Line
	10050 2350 10600 2350
Wire Wire Line
	10600 2350 10600 2550
Wire Wire Line
	10600 2950 10600 3200
Wire Wire Line
	9300 2900 9300 3150
$Comp
L +5V #PWR45
U 1 1 59F76B40
P 9300 2100
F 0 "#PWR45" H 9300 1950 50  0001 C CNN
F 1 "+5V" H 9300 2240 50  0000 C CNN
F 2 "" H 9300 2100 50  0000 C CNN
F 3 "" H 9300 2100 50  0000 C CNN
	1    9300 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR46
U 1 1 59F76BEF
P 9300 3150
F 0 "#PWR46" H 9300 2900 50  0001 C CNN
F 1 "GND" H 9300 3000 50  0000 C CNN
F 2 "" H 9300 3150 50  0000 C CNN
F 3 "" H 9300 3150 50  0000 C CNN
	1    9300 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR47
U 1 1 59F76C21
P 10600 3200
F 0 "#PWR47" H 10600 2950 50  0001 C CNN
F 1 "GND" H 10600 3050 50  0000 C CNN
F 2 "" H 10600 3200 50  0000 C CNN
F 3 "" H 10600 3200 50  0000 C CNN
	1    10600 3200
	1    0    0    -1  
$EndComp
$Comp
L LM317_SOT223 U4
U 1 1 59F7704D
P 3300 4950
F 0 "U4" H 3300 5250 50  0000 C CNN
F 1 "LM317_SOT223" H 3350 4700 50  0000 L CNN
F 2 "" H 3300 4950 50  0001 C CNN
F 3 "" H 3300 4950 50  0000 C CNN
	1    3300 4950
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR31
U 1 1 59F77053
P 2450 4700
F 0 "#PWR31" H 2450 4550 50  0001 C CNN
F 1 "VCC" H 2450 4850 50  0000 C CNN
F 2 "" H 2450 4700 50  0000 C CNN
F 3 "" H 2450 4700 50  0000 C CNN
	1    2450 4700
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 59F77059
P 3300 5850
F 0 "R4" V 3380 5850 50  0000 C CNN
F 1 "R" V 3300 5850 50  0000 C CNN
F 2 "" V 3230 5850 50  0001 C CNN
F 3 "" H 3300 5850 50  0000 C CNN
	1    3300 5850
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 59F7705F
P 4100 5400
F 0 "R5" V 4180 5400 50  0000 C CNN
F 1 "R" V 4100 5400 50  0000 C CNN
F 2 "" V 4030 5400 50  0001 C CNN
F 3 "" H 4100 5400 50  0000 C CNN
	1    4100 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4800 2450 4800
Wire Wire Line
	2450 4800 2450 4700
Wire Wire Line
	4100 5550 3300 5550
Wire Wire Line
	3300 5300 3300 5700
Connection ~ 3300 5550
$Comp
L GND #PWR33
U 1 1 59F7706A
P 3300 6150
F 0 "#PWR33" H 3300 5900 50  0001 C CNN
F 1 "GND" H 3300 6000 50  0000 C CNN
F 2 "" H 3300 6150 50  0000 C CNN
F 3 "" H 3300 6150 50  0000 C CNN
	1    3300 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 6000 3300 6150
$Comp
L C C6
U 1 1 59F77071
P 4750 5200
F 0 "C6" H 4775 5300 50  0000 L CNN
F 1 "C" H 4775 5100 50  0000 L CNN
F 2 "" H 4788 5050 50  0001 C CNN
F 3 "" H 4750 5200 50  0000 C CNN
	1    4750 5200
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 59F77077
P 5350 4800
F 0 "R7" V 5430 4800 50  0000 C CNN
F 1 "470R" V 5350 4800 50  0000 C CNN
F 2 "" V 5280 4800 50  0001 C CNN
F 3 "" H 5350 4800 50  0000 C CNN
	1    5350 4800
	0    1    1    0   
$EndComp
$Comp
L LED D4
U 1 1 59F7707D
P 6050 5200
F 0 "D4" H 6050 5300 50  0000 C CNN
F 1 "LED" H 6050 5100 50  0000 C CNN
F 2 "" H 6050 5200 50  0001 C CNN
F 3 "" H 6050 5200 50  0000 C CNN
	1    6050 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 4800 4100 5250
Wire Wire Line
	4750 4550 4750 5050
Connection ~ 4750 4800
Wire Wire Line
	5500 4800 6050 4800
Wire Wire Line
	6050 4800 6050 5000
Wire Wire Line
	6050 5400 6050 5650
Wire Wire Line
	4750 5350 4750 5600
$Comp
L GND #PWR37
U 1 1 59F7709B
P 4750 5600
F 0 "#PWR37" H 4750 5350 50  0001 C CNN
F 1 "GND" H 4750 5450 50  0000 C CNN
F 2 "" H 4750 5600 50  0000 C CNN
F 3 "" H 4750 5600 50  0000 C CNN
	1    4750 5600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR39
U 1 1 59F770A1
P 6050 5650
F 0 "#PWR39" H 6050 5400 50  0001 C CNN
F 1 "GND" H 6050 5500 50  0000 C CNN
F 2 "" H 6050 5650 50  0000 C CNN
F 3 "" H 6050 5650 50  0000 C CNN
	1    6050 5650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR36
U 1 1 59F7712F
P 4750 4550
F 0 "#PWR36" H 4750 4400 50  0001 C CNN
F 1 "+3.3V" H 4750 4690 50  0000 C CNN
F 2 "" H 4750 4550 50  0000 C CNN
F 3 "" H 4750 4550 50  0000 C CNN
	1    4750 4550
	1    0    0    -1  
$EndComp
Connection ~ 4100 4800
Connection ~ 8650 2350
Wire Wire Line
	3700 4800 5200 4800
$Comp
L F_Small F1
U 1 1 59F7C292
P 3500 2400
F 0 "F1" H 3460 2460 50  0000 L CNN
F 1 "F_Small" H 3380 2340 50  0000 L CNN
F 2 "" H 3500 2400 50  0001 C CNN
F 3 "" H 3500 2400 50  0000 C CNN
	1    3500 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 2100 9300 2600
$Comp
L B_Plug_5mm U9
U 1 1 5A3E7965
P 7100 4850
F 0 "U9" H 7100 5250 60  0000 C CNN
F 1 "B_Plug_5mm" H 7100 5350 60  0000 C CNN
F 2 "" H 7100 4850 60  0000 C CNN
F 3 "" H 7100 4850 60  0000 C CNN
	1    7100 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR41
U 1 1 5A3E7A0F
P 7300 5350
F 0 "#PWR41" H 7300 5100 50  0001 C CNN
F 1 "GND" H 7300 5200 50  0000 C CNN
F 2 "" H 7300 5350 50  0000 C CNN
F 3 "" H 7300 5350 50  0000 C CNN
	1    7300 5350
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR44
U 1 1 5A3E7A4B
P 8550 4700
F 0 "#PWR44" H 8550 4550 50  0001 C CNN
F 1 "+9V" H 8550 4840 50  0000 C CNN
F 2 "" H 8550 4700 50  0000 C CNN
F 3 "" H 8550 4700 50  0000 C CNN
	1    8550 4700
	1    0    0    -1  
$EndComp
$Comp
L F_Small F2
U 1 1 5A3E7AEB
P 7700 4800
F 0 "F2" H 7660 4860 50  0000 L CNN
F 1 "F_Small" H 7580 4740 50  0000 L CNN
F 2 "" H 7700 4800 50  0000 C CNN
F 3 "" H 7700 4800 50  0000 C CNN
	1    7700 4800
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5A3E7B8B
P 8650 5250
F 0 "C9" H 8675 5350 50  0000 L CNN
F 1 "C" H 8675 5150 50  0000 L CNN
F 2 "" H 8688 5100 50  0000 C CNN
F 3 "" H 8650 5250 50  0000 C CNN
	1    8650 5250
	1    0    0    -1  
$EndComp
$Comp
L CP C8
U 1 1 5A3E7BD6
P 8350 5250
F 0 "C8" H 8375 5350 50  0000 L CNN
F 1 "CP" H 8375 5150 50  0000 L CNN
F 2 "" H 8388 5100 50  0000 C CNN
F 3 "" H 8350 5250 50  0000 C CNN
	1    8350 5250
	1    0    0    -1  
$EndComp
$Comp
L D D6
U 1 1 5A3E7C4B
P 8050 5250
F 0 "D6" H 8050 5350 50  0000 C CNN
F 1 "D" H 8050 5150 50  0000 C CNN
F 2 "" H 8050 5250 50  0000 C CNN
F 3 "" H 8050 5250 50  0000 C CNN
	1    8050 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 4800 7600 4800
Wire Wire Line
	7800 4800 8650 4800
Wire Wire Line
	8550 4800 8550 4700
Wire Wire Line
	8050 5100 8050 4800
Connection ~ 8050 4800
Wire Wire Line
	8350 4800 8350 5100
Connection ~ 8350 4800
Wire Wire Line
	8650 4800 8650 5100
Connection ~ 8550 4800
$Comp
L GND #PWR43
U 1 1 5A3E7EEF
P 8350 5600
F 0 "#PWR43" H 8350 5350 50  0001 C CNN
F 1 "GND" H 8350 5450 50  0000 C CNN
F 2 "" H 8350 5600 50  0000 C CNN
F 3 "" H 8350 5600 50  0000 C CNN
	1    8350 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 5400 8050 5600
Wire Wire Line
	8050 5600 8650 5600
Wire Wire Line
	8350 5600 8350 5400
Wire Wire Line
	8650 5600 8650 5400
Connection ~ 8350 5600
Wire Wire Line
	7300 5350 7300 4900
Wire Wire Line
	7300 4900 7200 4900
$EndSCHEMATC
