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
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 7650 1000 1700 1700
U 596138FA
F0 "motor_part" 60
F1 "motor_part.sch" 60
F2 "BRAKE_1" I L 7650 1150 60 
F3 "TACHO_1" I L 7650 1300 60 
F4 "DIAG_1" I L 7650 1450 60 
F5 "DIR_1" I L 7650 1600 60 
F6 "EN_1" I L 7650 1750 60 
F7 "BRAKE_2" I L 7650 2000 60 
F8 "TACHO_2" I L 7650 2150 60 
F9 "DIAG_2" I L 7650 2300 60 
F10 "DIR_2" I L 7650 2450 60 
F11 "EN_2" I L 7650 2600 60 
$EndSheet
$Sheet
S 5050 900  1600 2450
U 59613C4C
F0 "uC" 60
F1 "uC.sch" 60
F2 "Ch_A1" I L 5050 3250 60 
F3 "Ch_B1" I L 5050 3100 60 
F4 "Ch_A2" I R 6650 3250 60 
F5 "Ch_B2" I R 6650 3100 60 
F6 "BRAKE_1" I R 6650 1150 60 
F7 "TACHO_1" I R 6650 1300 60 
F8 "DIAG_1" I R 6650 1450 60 
F9 "DIR_1" I R 6650 1600 60 
F10 "EN_1" I R 6650 1750 60 
F11 "BRAKE_2" I R 6650 2000 60 
F12 "DIR_2" I R 6650 2450 60 
F13 "DIAG_2" I R 6650 2300 60 
F14 "TACHO_2" I R 6650 2150 60 
F15 "EN_2" I R 6650 2600 60 
$EndSheet
$Sheet
S 2750 1100 1500 1800
U 59613F6F
F0 "power_part" 60
F1 "power_part.sch" 60
$EndSheet
$Sheet
S 5050 3950 2350 800 
U 5962A1C1
F0 "encodeurs" 60
F1 "encodeurs.sch" 60
F2 "Ch_A1" I L 5050 4100 60 
F3 "Ch_B1" I L 5050 4300 60 
F4 "Ch_A2" I R 7400 4100 60 
F5 "Ch_B2" I R 7400 4300 60 
$EndSheet
Wire Wire Line
	5050 3250 4950 3250
Wire Wire Line
	4950 3250 4950 4100
Wire Wire Line
	4950 4100 5050 4100
Wire Wire Line
	5050 3100 4850 3100
Wire Wire Line
	4850 3100 4850 4300
Wire Wire Line
	4850 4300 5050 4300
Wire Wire Line
	6650 3250 7500 3250
Wire Wire Line
	7500 3250 7500 4100
Wire Wire Line
	7500 4100 7400 4100
Wire Wire Line
	6650 3100 7600 3100
Wire Wire Line
	7600 3100 7600 4300
Wire Wire Line
	7600 4300 7400 4300
Wire Wire Line
	6650 1150 7650 1150
Wire Wire Line
	6650 1300 7650 1300
Wire Wire Line
	6650 1450 7650 1450
Wire Wire Line
	6650 1600 7650 1600
Wire Wire Line
	6650 1750 7650 1750
Wire Wire Line
	7650 2000 6650 2000
Wire Wire Line
	6650 2150 7650 2150
Wire Wire Line
	6650 2300 7650 2300
Wire Wire Line
	6650 2450 7650 2450
Wire Wire Line
	7650 2600 6650 2600
$EndSCHEMATC