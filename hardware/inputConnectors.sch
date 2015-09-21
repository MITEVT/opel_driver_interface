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
LIBS:opel_driver_interface-cache
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
Wire Wire Line
	3200 2450 3600 2450
Wire Wire Line
	3400 2650 3400 2550
Wire Wire Line
	3400 2550 3200 2550
$Comp
L CONN_01X02 P?
U 1 1 55FDABAC
P 3000 2500
F 0 "P?" H 3000 2650 50  0000 C CNN
F 1 "CONN_01X02" V 3100 2500 50  0000 C CNN
F 2 "" H 3000 2500 60  0000 C CNN
F 3 "" H 3000 2500 60  0000 C CNN
	1    3000 2500
	-1   0    0    1   
$EndComp
Text HLabel 3600 2450 2    60   Output ~ 0
+12V
$Comp
L GND #PWR?
U 1 1 55FDAC37
P 3400 2650
F 0 "#PWR?" H 3400 2400 50  0001 C CNN
F 1 "GND" H 3400 2500 50  0000 C CNN
F 2 "" H 3400 2650 60  0000 C CNN
F 3 "" H 3400 2650 60  0000 C CNN
	1    3400 2650
	1    0    0    -1  
$EndComp
Text HLabel 5000 3000 2    60   Output ~ 0
RXD
Text HLabel 5000 2900 2    60   Input ~ 0
TXD
$Comp
L CONN_01X06 P?
U 1 1 55FDBF5F
P 4600 2850
F 0 "P?" H 4600 3200 50  0000 C CNN
F 1 "CONN_01X06" V 4700 2850 50  0000 C CNN
F 2 "" H 4600 2850 60  0000 C CNN
F 3 "" H 4600 2850 60  0000 C CNN
	1    4600 2850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 2900 4800 2900
Wire Wire Line
	4800 3000 5000 3000
Wire Wire Line
	4800 3100 5000 3100
Wire Wire Line
	5000 3100 5000 3200
$Comp
L GND #PWR?
U 1 1 55FDBFE6
P 5000 3200
F 0 "#PWR?" H 5000 2950 50  0001 C CNN
F 1 "GND" H 5000 3050 50  0000 C CNN
F 2 "" H 5000 3200 60  0000 C CNN
F 3 "" H 5000 3200 60  0000 C CNN
	1    5000 3200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 55FDC000
P 5000 2500
F 0 "#PWR?" H 5000 2350 50  0001 C CNN
F 1 "+3.3V" H 5000 2640 50  0000 C CNN
F 2 "" H 5000 2500 60  0000 C CNN
F 3 "" H 5000 2500 60  0000 C CNN
	1    5000 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 2600 5000 2600
Wire Wire Line
	5000 2600 5000 2500
Wire Wire Line
	4800 2700 5050 2700
Wire Wire Line
	4800 2800 5050 2800
Text HLabel 5050 2700 2    60   Output ~ 0
MCU_RESET_N
Text HLabel 5050 2800 2    60   Output ~ 0
MCU_PROG_N
$EndSCHEMATC
