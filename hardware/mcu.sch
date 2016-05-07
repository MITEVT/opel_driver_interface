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
LIBS:MITEVT_ANALOG
LIBS:MITEVT_CONTACTORS
LIBS:MITEVT_interface
LIBS:MITEVT_mcontrollers
LIBS:MITEVT_OPTO
LIBS:MITEVT_power
LIBS:MITEVT_REG
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
L LPC11C14 U403
U 1 1 563E21A7
P 6350 3650
F 0 "U403" H 5200 5500 60  0000 C CNN
F 1 "LPC11C14" H 7150 1850 60  0000 C CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 6500 2450 60  0001 C CNN
F 3 "" H 6500 2450 60  0000 C CNN
	1    6350 3650
	1    0    0    -1  
$EndComp
Text HLabel 2350 5050 0    60   BiDi ~ 0
CANL
Text HLabel 2350 4850 0    60   BiDi ~ 0
CANH
$Comp
L GND #PWR09
U 1 1 563E2693
P 3850 5800
F 0 "#PWR09" H 3850 5550 50  0001 C CNN
F 1 "GND" H 3850 5650 50  0000 C CNN
F 2 "" H 3850 5800 60  0000 C CNN
F 3 "" H 3850 5800 60  0000 C CNN
	1    3850 5800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 563E26E3
P 3250 5850
F 0 "#PWR010" H 3250 5600 50  0001 C CNN
F 1 "GND" H 3250 5700 50  0000 C CNN
F 2 "" H 3250 5850 60  0000 C CNN
F 3 "" H 3250 5850 60  0000 C CNN
	1    3250 5850
	1    0    0    -1  
$EndComp
$Comp
L C_Small C401
U 1 1 563E2793
P 2500 4300
F 0 "C401" H 2510 4370 50  0000 L CNN
F 1 "10uF" H 2510 4220 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2500 4300 60  0001 C CNN
F 3 "" H 2500 4300 60  0000 C CNN
	1    2500 4300
	1    0    0    -1  
$EndComp
$Comp
L C_Small C402
U 1 1 563E282A
P 2950 4300
F 0 "C402" H 2960 4370 50  0000 L CNN
F 1 "0.1uF" H 2960 4220 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2950 4300 60  0001 C CNN
F 3 "" H 2950 4300 60  0000 C CNN
	1    2950 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 563E2981
P 2500 4650
F 0 "#PWR011" H 2500 4400 50  0001 C CNN
F 1 "GND" H 2500 4500 50  0000 C CNN
F 2 "" H 2500 4650 60  0000 C CNN
F 3 "" H 2500 4650 60  0000 C CNN
	1    2500 4650
	1    0    0    -1  
$EndComp
Text Label 4650 4050 2    60   ~ 0
PIO1_11
Text Label 4650 3950 2    60   ~ 0
PIO1_10
Text Label 4650 3850 2    60   ~ 0
PIO1_4
Text Label 4650 3650 2    60   ~ 0
PIO1_2
Text Label 4650 3550 2    60   ~ 0
PIO1_1
Text Label 4650 3450 2    60   ~ 0
PIO1_0
Text Label 4650 3350 2    60   ~ 0
PIO0_11
NoConn ~ 4950 2550
NoConn ~ 4950 2450
Text HLabel 4500 2250 0    60   Input ~ 0
MCU_PROG_N
$Comp
L R R402
U 1 1 563E3CB3
P 4650 2000
F 0 "R402" V 4730 2000 50  0000 C CNN
F 1 "10k" V 4650 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4580 2000 30  0001 C CNN
F 3 "" H 4650 2000 30  0000 C CNN
	1    4650 2000
	1    0    0    -1  
$EndComp
$Comp
L R R401
U 1 1 563E3FD3
P 3450 2100
F 0 "R401" V 3530 2100 50  0000 C CNN
F 1 "10k" V 3450 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3380 2100 30  0001 C CNN
F 3 "" H 3450 2100 30  0000 C CNN
	1    3450 2100
	1    0    0    -1  
$EndComp
Text HLabel 3050 2350 0    60   Input ~ 0
MCU_RESET_N
Text Label 4650 4550 2    60   ~ 0
PIO1_9
Text Label 4650 4650 2    60   ~ 0
PIO2_4
$Comp
L GND #PWR012
U 1 1 563E497F
P 6250 6200
F 0 "#PWR012" H 6250 5950 50  0001 C CNN
F 1 "GND" H 6250 6050 50  0000 C CNN
F 2 "" H 6250 6200 60  0000 C CNN
F 3 "" H 6250 6200 60  0000 C CNN
	1    6250 6200
	1    0    0    -1  
$EndComp
$Comp
L C_Small C405
U 1 1 563E4BCA
P 6500 1350
F 0 "C405" H 6510 1420 50  0000 L CNN
F 1 "0.1uF" H 6510 1270 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6500 1350 60  0001 C CNN
F 3 "" H 6500 1350 60  0000 C CNN
	1    6500 1350
	1    0    0    -1  
$EndComp
$Comp
L C_Small C406
U 1 1 563E4C7A
P 6950 1350
F 0 "C406" H 6960 1420 50  0000 L CNN
F 1 "10uF" H 6960 1270 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6950 1350 60  0001 C CNN
F 3 "" H 6950 1350 60  0000 C CNN
	1    6950 1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 563E4F46
P 6950 1650
F 0 "#PWR013" H 6950 1400 50  0001 C CNN
F 1 "GND" H 6950 1500 50  0000 C CNN
F 2 "" H 6950 1650 60  0000 C CNN
F 3 "" H 6950 1650 60  0000 C CNN
	1    6950 1650
	1    0    0    -1  
$EndComp
Text HLabel 8000 2350 2    60   Input ~ 0
RXD
Text HLabel 8000 2450 2    60   Output ~ 0
TXD
Text Label 8000 3300 0    60   ~ 0
PIO2_2
Text Label 8000 3400 0    60   ~ 0
PIO2_3
Text Label 8000 3500 0    60   ~ 0
PIO2_1
Text Label 8000 3600 0    60   ~ 0
PIO0_1
Text Label 8000 3800 0    60   ~ 0
PIO0_4
Text Label 8000 3900 0    60   ~ 0
PIO0_5
Text Label 8000 4150 0    60   ~ 0
PIO0_3
Text Label 8000 4350 0    60   ~ 0
PIO1_5
Text Label 8000 4450 0    60   ~ 0
PIO1_8
Text Label 8000 4550 0    60   ~ 0
PIO2_6
Text Label 8000 4650 0    60   ~ 0
PIO2_7
Text Label 8000 4750 0    60   ~ 0
PIO2_8
Text Label 8000 4950 0    60   ~ 0
PIO3_0
Text Label 8000 5050 0    60   ~ 0
PIO3_1
Text Label 8000 5150 0    60   ~ 0
PIO3_2
Text Label 8000 5250 0    60   ~ 0
PIO3_3
Text HLabel 8750 4950 2    60   Output ~ 0
LED7
Text HLabel 8750 5050 2    60   Output ~ 0
LED6
Text HLabel 4200 3750 0    60   Output ~ 0
LED5
Text HLabel 8750 5250 2    60   Output ~ 0
LED4
Text HLabel 8750 4550 2    60   Output ~ 0
LED3
Text HLabel 8750 4450 2    60   Output ~ 0
LED2
Text HLabel 8750 4650 2    60   Output ~ 0
LED1
Text HLabel 8750 4750 2    60   Output ~ 0
LED0
$Comp
L +3.3V #PWR014
U 1 1 5647852E
P 3450 1750
F 0 "#PWR014" H 3450 1600 50  0001 C CNN
F 1 "+3.3V" H 3450 1890 50  0000 C CNN
F 2 "" H 3450 1750 60  0000 C CNN
F 3 "" H 3450 1750 60  0000 C CNN
	1    3450 1750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR015
U 1 1 56478628
P 4650 1750
F 0 "#PWR015" H 4650 1600 50  0001 C CNN
F 1 "+3.3V" H 4650 1890 50  0000 C CNN
F 2 "" H 4650 1750 60  0000 C CNN
F 3 "" H 4650 1750 60  0000 C CNN
	1    4650 1750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR016
U 1 1 56478697
P 6250 1000
F 0 "#PWR016" H 6250 850 50  0001 C CNN
F 1 "+3.3V" H 6250 1140 50  0000 C CNN
F 2 "" H 6250 1000 60  0000 C CNN
F 3 "" H 6250 1000 60  0000 C CNN
	1    6250 1000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR017
U 1 1 564789D8
P 2750 3650
F 0 "#PWR017" H 2750 3500 50  0001 C CNN
F 1 "+5V" H 2750 3790 50  0000 C CNN
F 2 "" H 2750 3650 60  0000 C CNN
F 3 "" H 2750 3650 60  0000 C CNN
	1    2750 3650
	1    0    0    -1  
$EndComp
Text Label 4250 4750 0    60   ~ 0
CAN_RXD
Text Label 4250 4850 0    60   ~ 0
CAN_TXD
Text Label 4650 3750 2    60   ~ 0
PIO1_3
Wire Wire Line
	3750 4750 4950 4750
Wire Wire Line
	3750 4850 4950 4850
Wire Wire Line
	2350 4850 2750 4850
Wire Wire Line
	2350 5050 2750 5050
Wire Wire Line
	3250 3650 3250 4550
Wire Wire Line
	3250 3650 2750 3650
Wire Wire Line
	2500 4200 2500 3900
Wire Wire Line
	2500 3900 3250 3900
Wire Wire Line
	2950 3900 2950 4200
Wire Wire Line
	2500 4400 2500 4650
Wire Wire Line
	2500 4500 2950 4500
Wire Wire Line
	2950 4500 2950 4400
Connection ~ 2500 4500
Connection ~ 3250 3900
Connection ~ 2950 3900
Wire Wire Line
	4950 4050 4650 4050
Wire Wire Line
	4650 3950 4950 3950
Wire Wire Line
	4200 3450 4950 3450
Wire Wire Line
	4650 3550 4950 3550
Wire Wire Line
	4650 3650 4950 3650
Wire Wire Line
	4200 3750 4950 3750
Wire Wire Line
	4650 3850 4950 3850
Wire Wire Line
	4500 2250 4950 2250
Wire Wire Line
	4650 2150 4650 2250
Connection ~ 4650 2250
Wire Wire Line
	4650 1850 4650 1750
Wire Wire Line
	3050 2350 4950 2350
Wire Wire Line
	3450 2250 3450 2350
Connection ~ 3450 2350
Wire Wire Line
	3450 1750 3450 1950
Wire Wire Line
	4650 4550 4950 4550
Wire Wire Line
	4650 4650 4950 4650
Wire Wire Line
	6150 5600 6150 6050
Wire Wire Line
	6150 6050 6350 6050
Wire Wire Line
	6350 6050 6350 5600
Wire Wire Line
	6250 6050 6250 6200
Connection ~ 6250 6050
Wire Wire Line
	6250 1000 6250 1650
Wire Wire Line
	6050 1650 6050 1500
Wire Wire Line
	6050 1500 6250 1500
Connection ~ 6250 1500
Wire Wire Line
	6250 1100 6950 1100
Wire Wire Line
	6950 1100 6950 1250
Connection ~ 6250 1100
Wire Wire Line
	6500 1250 6500 1100
Connection ~ 6500 1100
Wire Wire Line
	6950 1450 6950 1650
Wire Wire Line
	6950 1550 6500 1550
Wire Wire Line
	6500 1550 6500 1450
Connection ~ 6950 1550
Wire Wire Line
	8000 2350 7600 2350
Wire Wire Line
	8000 2450 7600 2450
Wire Wire Line
	8000 3300 7600 3300
Wire Wire Line
	7600 3400 8000 3400
Wire Wire Line
	7600 3500 8000 3500
Wire Wire Line
	7600 3600 8000 3600
Wire Wire Line
	8000 3800 7600 3800
Wire Wire Line
	8000 3900 7600 3900
Wire Wire Line
	8000 4150 7600 4150
Wire Wire Line
	8000 4350 7600 4350
Wire Wire Line
	7600 4450 8750 4450
Wire Wire Line
	7600 4550 8750 4550
Wire Wire Line
	7600 4650 8750 4650
Wire Wire Line
	7600 4750 8750 4750
Wire Wire Line
	7600 4950 8750 4950
Wire Wire Line
	7600 5050 8750 5050
Wire Wire Line
	7600 5150 8000 5150
Wire Wire Line
	7600 5250 8750 5250
$Comp
L MCP2551-I/SN U401
U 1 1 5690B2F9
P 3250 4950
F 0 "U401" H 2850 5300 50  0000 L CNN
F 1 "MCP2551-I/SN" H 3350 5300 50  0000 L CNN
F 2 "Housings_SSOP:MSOP-8_3x3mm_Pitch0.65mm" H 3250 4950 50  0001 C CIN
F 3 "" H 3250 4950 50  0000 C CNN
	1    3250 4950
	-1   0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5690B4B6
P 3850 5450
F 0 "R1" V 3930 5450 50  0000 C CNN
F 1 "20k" V 3850 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 3780 5450 50  0001 C CNN
F 3 "" H 3850 5450 50  0000 C CNN
	1    3850 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5150 3850 5150
Wire Wire Line
	3850 5150 3850 5300
NoConn ~ 3750 5050
Wire Wire Line
	3850 5600 3850 5800
Wire Wire Line
	3250 5350 3250 5850
NoConn ~ 7600 2600
NoConn ~ 7600 2700
NoConn ~ 7600 2800
NoConn ~ 7600 2900
NoConn ~ 7600 3100
NoConn ~ 4950 4950
NoConn ~ 4950 5050
NoConn ~ 4950 5150
NoConn ~ 4950 5250
NoConn ~ 7600 4850
NoConn ~ 4650 3550
NoConn ~ 4650 3650
NoConn ~ 4650 3850
NoConn ~ 4650 3950
NoConn ~ 4650 4050
NoConn ~ 4650 4550
NoConn ~ 4650 4650
NoConn ~ 8000 5150
NoConn ~ 8000 4350
NoConn ~ 8000 4150
NoConn ~ 8000 3900
NoConn ~ 8000 3800
NoConn ~ 8000 3600
NoConn ~ 8000 3500
NoConn ~ 8000 3400
NoConn ~ 8000 3300
Wire Wire Line
	4200 3350 4950 3350
Text HLabel 4200 3350 0    60   Input ~ 0
CAR_ON
Text HLabel 4200 3450 0    60   Output ~ 0
LV_ON
$EndSCHEMATC
