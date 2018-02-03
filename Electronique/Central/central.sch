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
$Descr A2 16535 23386 portrait
encoding utf-8
Sheet 1 4
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
L Nucleo-64 U1
U 1 1 5A6CA3F4
P 3700 16650
F 0 "U1" H 3750 16700 60  0000 C CNN
F 1 "Nucleo-64" H 8050 25650 100 0000 C CNN
F 2 "Nucleo_64:Nucleo-64" H 7950 28100 60  0001 C CNN
F 3 "" H 7950 28100 60  0001 C CNN
	1    3700 16650
	1    0    0    -1  
$EndComp
$Sheet
S 1200 1000 5000 2000
U 5A6CA888
F0 "Power" 60
F1 "Power.sch" 60
$EndSheet
Text GLabel 13350 11650 0    60   Input ~ 0
I2C_TOF_SDA
Text GLabel 13350 11450 0    60   Input ~ 0
I2C_TOF_SCL
$Comp
L +5V #PWR1
U 1 1 5A6CAF8E
P 3250 10750
F 0 "#PWR1" H 3250 10600 50  0001 C CNN
F 1 "+5V" H 3250 10890 50  0000 C CNN
F 2 "" H 3250 10750 50  0000 C CNN
F 3 "" H 3250 10750 50  0000 C CNN
	1    3250 10750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 10750 3250 10850
Wire Wire Line
	3250 10850 4750 10850
NoConn ~ 11850 5850
NoConn ~ 11850 6850
NoConn ~ 11850 6650
NoConn ~ 11850 9450
NoConn ~ 11850 9650
NoConn ~ 11850 10050
NoConn ~ 4750 12850
NoConn ~ 4750 12050
NoConn ~ 4750 11650
NoConn ~ 4750 11250
NoConn ~ 4750 11450
NoConn ~ 4750 12650
$Comp
L +3.3V #PWR2
U 1 1 5A6CB32D
P 4250 11800
F 0 "#PWR2" H 4250 11650 50  0001 C CNN
F 1 "+3.3V" H 4250 11940 50  0000 C CNN
F 2 "" H 4250 11800 50  0000 C CNN
F 3 "" H 4250 11800 50  0000 C CNN
	1    4250 11800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 11800 4250 11850
Wire Wire Line
	4250 11850 4750 11850
$Comp
L I2C_repeater U3
U 1 1 5A6CB761
P 14500 11650
F 0 "U3" H 14500 11650 60  0000 C CNN
F 1 "I2C_repeater" H 14500 11750 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 14500 11650 60  0001 C CNN
F 3 "" H 14500 11650 60  0000 C CNN
	1    14500 11650
	1    0    0    -1  
$EndComp
Wire Wire Line
	13350 11450 13900 11450
Wire Wire Line
	13350 11650 13900 11650
Text GLabel 13300 11850 0    60   Input ~ 0
I2C_TOF_EN
Wire Wire Line
	13300 11850 13900 11850
$Comp
L GND #PWR4
U 1 1 5A6CC583
P 14500 12200
F 0 "#PWR4" H 14500 11950 50  0001 C CNN
F 1 "GND" H 14500 12050 50  0000 C CNN
F 2 "" H 14500 12200 50  0000 C CNN
F 3 "" H 14500 12200 50  0000 C CNN
	1    14500 12200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR3
U 1 1 5A6CC59B
P 14300 10800
F 0 "#PWR3" H 14300 10650 50  0001 C CNN
F 1 "+3.3V" H 14300 10940 50  0000 C CNN
F 2 "" H 14300 10800 50  0000 C CNN
F 3 "" H 14300 10800 50  0000 C CNN
	1    14300 10800
	1    0    0    -1  
$EndComp
Wire Wire Line
	14300 10800 14300 10950
Text GLabel 14700 10800 1    60   Input ~ 0
+2.8V
Wire Wire Line
	14700 10800 14700 10950
Wire Wire Line
	14500 12150 14500 12200
$Comp
L CONN_01X04 P1
U 1 1 5A6CC727
P 15850 11600
F 0 "P1" H 15850 11850 50  0000 C CNN
F 1 "CONN_TOF_04" V 15950 11600 50  0000 C CNN
F 2 "" H 15850 11600 50  0001 C CNN
F 3 "" H 15850 11600 50  0000 C CNN
	1    15850 11600
	1    0    0    -1  
$EndComp
Wire Wire Line
	15100 11450 15650 11450
Wire Wire Line
	15100 11650 15350 11650
Wire Wire Line
	15350 11650 15350 11550
Wire Wire Line
	15350 11550 15650 11550
$Comp
L GND #PWR5
U 1 1 5A6CC78A
P 15250 11800
F 0 "#PWR5" H 15250 11550 50  0001 C CNN
F 1 "GND" H 15250 11650 50  0000 C CNN
F 2 "" H 15250 11800 50  0000 C CNN
F 3 "" H 15250 11800 50  0000 C CNN
	1    15250 11800
	1    0    0    -1  
$EndComp
Text GLabel 15500 12150 0    60   Input ~ 0
+2.8V
Wire Wire Line
	15250 11800 15400 11800
Wire Wire Line
	15400 11800 15400 11650
Wire Wire Line
	15400 11650 15650 11650
Wire Wire Line
	15500 12150 15650 12150
Wire Wire Line
	15650 12150 15650 11750
$Comp
L R R2
U 1 1 5A6CC90A
P 13650 11150
F 0 "R2" V 13730 11150 50  0000 C CNN
F 1 "10kR" V 13650 11150 50  0000 C CNN
F 2 "" V 13580 11150 50  0001 C CNN
F 3 "" H 13650 11150 50  0000 C CNN
	1    13650 11150
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5A6CC927
P 13500 11400
F 0 "R1" V 13580 11400 50  0000 C CNN
F 1 "10kR" V 13500 11400 50  0000 C CNN
F 2 "" V 13430 11400 50  0001 C CNN
F 3 "" H 13500 11400 50  0000 C CNN
	1    13500 11400
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5A6CC988
P 15250 11150
F 0 "R3" V 15330 11150 50  0000 C CNN
F 1 "10kR" V 15250 11150 50  0000 C CNN
F 2 "" V 15180 11150 50  0001 C CNN
F 3 "" H 15250 11150 50  0000 C CNN
	1    15250 11150
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5A6CC9AD
P 15500 11350
F 0 "R4" V 15580 11350 50  0000 C CNN
F 1 "10kR" V 15500 11350 50  0000 C CNN
F 2 "" V 15430 11350 50  0001 C CNN
F 3 "" H 15500 11350 50  0000 C CNN
	1    15500 11350
	1    0    0    -1  
$EndComp
Wire Wire Line
	13500 11550 13500 11650
Connection ~ 13500 11650
Wire Wire Line
	13650 11300 13650 11450
Connection ~ 13650 11450
Wire Wire Line
	13500 11250 13500 11000
Wire Wire Line
	13500 11000 13650 11000
Wire Wire Line
	13600 11000 13600 10900
Wire Wire Line
	13600 10900 14300 10900
Connection ~ 14300 10900
Connection ~ 13600 11000
Wire Wire Line
	15250 11000 15500 11000
Wire Wire Line
	15500 11000 15500 11200
Wire Wire Line
	15500 11500 15500 11550
Connection ~ 15500 11550
Wire Wire Line
	15250 11300 15250 11450
Connection ~ 15250 11450
Wire Wire Line
	14700 10900 15350 10900
Wire Wire Line
	15350 10900 15350 11000
Connection ~ 15350 11000
Connection ~ 14700 10900
Text GLabel 12050 12850 2    60   Input ~ 0
I2C_TOF_SDA
Text GLabel 12050 12450 2    60   Input ~ 0
I2C_TOF_SCL
Text GLabel 12050 12650 2    60   Input ~ 0
I2C_TOF_EN
Wire Wire Line
	11850 12450 12050 12450
Wire Wire Line
	11850 12650 12050 12650
Wire Wire Line
	11850 12850 12050 12850
$Sheet
S 10600 1350 3650 1450
U 5A74AF92
F0 "Audio" 60
F1 "Audio.sch" 60
$EndSheet
NoConn ~ 4750 9250
NoConn ~ 4750 6850
NoConn ~ 4750 7050
NoConn ~ 4750 6650
NoConn ~ 4750 5850
NoConn ~ 5300 4750
NoConn ~ 5500 4750
NoConn ~ 11300 4750
NoConn ~ 11500 4750
$Comp
L +3.3V #PWR?
U 1 1 5A7641EB
P 4400 6450
F 0 "#PWR?" H 4400 6300 50  0001 C CNN
F 1 "+3.3V" H 4400 6590 50  0000 C CNN
F 2 "" H 4400 6450 50  0000 C CNN
F 3 "" H 4400 6450 50  0000 C CNN
	1    4400 6450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 6450 4750 6450
$Comp
L GND #PWR?
U 1 1 5A764D00
P 4550 11050
F 0 "#PWR?" H 4550 10800 50  0001 C CNN
F 1 "GND" H 4550 10900 50  0000 C CNN
F 2 "" H 4550 11050 50  0000 C CNN
F 3 "" H 4550 11050 50  0000 C CNN
	1    4550 11050
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 11050 4750 11050
$Comp
L GND #PWR?
U 1 1 5A7650C8
P 4500 12250
F 0 "#PWR?" H 4500 12000 50  0001 C CNN
F 1 "GND" H 4500 12100 50  0000 C CNN
F 2 "" H 4500 12250 50  0000 C CNN
F 3 "" H 4500 12250 50  0000 C CNN
	1    4500 12250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A7650EA
P 4500 12450
F 0 "#PWR?" H 4500 12200 50  0001 C CNN
F 1 "GND" H 4500 12300 50  0000 C CNN
F 2 "" H 4500 12450 50  0000 C CNN
F 3 "" H 4500 12450 50  0000 C CNN
	1    4500 12450
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 12250 4750 12250
Wire Wire Line
	4500 12450 4750 12450
$Comp
L GND #PWR?
U 1 1 5A765594
P 12100 11050
F 0 "#PWR?" H 12100 10800 50  0001 C CNN
F 1 "GND" H 12100 10900 50  0000 C CNN
F 2 "" H 12100 11050 50  0000 C CNN
F 3 "" H 12100 11050 50  0000 C CNN
	1    12100 11050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11850 11050 12100 11050
NoConn ~ 11850 10850
$Comp
L GND #PWR?
U 1 1 5A765CB2
P 11950 9050
F 0 "#PWR?" H 11950 8800 50  0001 C CNN
F 1 "GND" H 11950 8900 50  0000 C CNN
F 2 "" H 11950 9050 50  0000 C CNN
F 3 "" H 11950 9050 50  0000 C CNN
	1    11950 9050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11850 9050 11950 9050
$Comp
L GND #PWR?
U 1 1 5A766182
P 11950 7850
F 0 "#PWR?" H 11950 7600 50  0001 C CNN
F 1 "GND" H 11950 7700 50  0000 C CNN
F 2 "" H 11950 7850 50  0000 C CNN
F 3 "" H 11950 7850 50  0000 C CNN
	1    11950 7850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11850 7850 11950 7850
$Sheet
S 10600 3050 3650 750 
U 5A76717C
F0 "LED" 60
F1 "LED.sch" 60
$EndSheet
$EndSCHEMATC
