EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector:Conn_01x04_Male J1
U 1 1 6053225A
P 4650 5350
F 0 "J1" V 4804 5062 50  0000 R CNN
F 1 "Conn_01x04_Male" V 4713 5062 50  0000 R CNN
F 2 "Encoder:SolderWirePad_1x04_SMD_5x10mm_copy" H 4650 5350 50  0001 C CNN
F 3 "~" H 4650 5350 50  0001 C CNN
	1    4650 5350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 6053A901
P 4750 4900
F 0 "#PWR0107" H 4750 4650 50  0001 C CNN
F 1 "GND" H 4750 4750 50  0000 C CNN
F 2 "" H 4750 4900 50  0001 C CNN
F 3 "" H 4750 4900 50  0001 C CNN
	1    4750 4900
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 6053ACF9
P 4850 4900
F 0 "#PWR0108" H 4850 4750 50  0001 C CNN
F 1 "+3.3V" H 4900 5050 50  0000 C CNN
F 2 "" H 4850 4900 50  0001 C CNN
F 3 "" H 4850 4900 50  0001 C CNN
	1    4850 4900
	0    1    1    0   
$EndComp
$Comp
L Device:C C21
U 1 1 60531985
P 5250 4800
F 0 "C21" H 5365 4846 50  0000 L CNN
F 1 "1uF" H 5365 4755 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5288 4650 50  0001 C CNN
F 3 "~" H 5250 4800 50  0001 C CNN
	1    5250 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3700 4050 3700
Wire Wire Line
	4050 3700 4050 3800
$Comp
L power:GND #PWR0101
U 1 1 60896E95
P 4050 3800
F 0 "#PWR0101" H 4050 3550 50  0001 C CNN
F 1 "GND" H 4055 3627 50  0000 C CNN
F 2 "" H 4050 3800 50  0001 C CNN
F 3 "" H 4050 3800 50  0001 C CNN
	1    4050 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3900 5450 3900
Wire Wire Line
	5450 3900 5450 4000
$Comp
L power:GND #PWR0102
U 1 1 608973CB
P 5450 4000
F 0 "#PWR0102" H 5450 3750 50  0001 C CNN
F 1 "GND" H 5455 3827 50  0000 C CNN
F 2 "" H 5450 4000 50  0001 C CNN
F 3 "" H 5450 4000 50  0001 C CNN
	1    5450 4000
	1    0    0    -1  
$EndComp
$Comp
L Encoder:RN4871 U8
U 1 1 608938A9
P 4650 4050
F 0 "U8" H 4700 4815 50  0000 C CNN
F 1 "RN4871" H 4700 4724 50  0000 C CNN
F 2 "Encoder:Bluetooth_Module_RN4871" H 4600 4150 50  0001 C CNN
F 3 "" H 4600 4150 50  0001 C CNN
	1    4650 4050
	1    0    0    -1  
$EndComp
NoConn ~ 5150 4000
NoConn ~ 4950 4500
NoConn ~ 4750 4500
NoConn ~ 4450 4500
NoConn ~ 4250 4000
NoConn ~ 4250 3900
NoConn ~ 4250 3800
NoConn ~ 4250 3600
NoConn ~ 5150 3700
Wire Wire Line
	5150 3800 5300 3800
Wire Wire Line
	5750 3800 5750 3750
$Comp
L power:+3.3V #PWR0103
U 1 1 608989C5
P 5750 3750
F 0 "#PWR0103" H 5750 3600 50  0001 C CNN
F 1 "+3.3V" H 5765 3923 50  0000 C CNN
F 2 "" H 5750 3750 50  0001 C CNN
F 3 "" H 5750 3750 50  0001 C CNN
	1    5750 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 60899ED0
P 5650 4150
F 0 "#PWR0104" H 5650 3900 50  0001 C CNN
F 1 "GND" H 5655 3977 50  0000 C CNN
F 2 "" H 5650 4150 50  0001 C CNN
F 3 "" H 5650 4150 50  0001 C CNN
	1    5650 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 60899ED6
P 5650 4000
F 0 "C20" H 5765 4046 50  0000 L CNN
F 1 "10uF" H 5765 3955 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5688 3850 50  0001 C CNN
F 3 "~" H 5650 4000 50  0001 C CNN
	1    5650 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3850 5650 3800
Connection ~ 5650 3800
Wire Wire Line
	5650 3800 5750 3800
$Comp
L power:+3.3V #PWR0105
U 1 1 6089BAE2
P 5250 4550
F 0 "#PWR0105" H 5250 4400 50  0001 C CNN
F 1 "+3.3V" H 5265 4723 50  0000 C CNN
F 2 "" H 5250 4550 50  0001 C CNN
F 3 "" H 5250 4550 50  0001 C CNN
	1    5250 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 4550 4850 4550
Wire Wire Line
	4850 4550 4850 4500
Wire Wire Line
	5250 4550 5250 4650
Connection ~ 5250 4550
$Comp
L power:GND #PWR0106
U 1 1 6089C77B
P 5250 4950
F 0 "#PWR0106" H 5250 4700 50  0001 C CNN
F 1 "GND" H 5255 4777 50  0000 C CNN
F 2 "" H 5250 4950 50  0001 C CNN
F 3 "" H 5250 4950 50  0001 C CNN
	1    5250 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 4500 4550 5150
Wire Wire Line
	4650 4500 4650 5150
Wire Wire Line
	4750 5150 4750 4900
Wire Wire Line
	4850 5150 4850 4900
Wire Wire Line
	5300 3800 5300 3600
Wire Wire Line
	5300 3600 5150 3600
Connection ~ 5300 3800
Wire Wire Line
	5300 3800 5650 3800
$EndSCHEMATC
