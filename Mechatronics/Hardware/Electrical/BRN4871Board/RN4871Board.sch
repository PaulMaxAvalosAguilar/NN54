EESchema Schematic File Version 4
LIBS:RN4871Board-cache
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
L Encoder:RN4871 U1
U 1 1 5EB34403
P 3450 2750
F 0 "U1" H 3500 3515 50  0000 C CNN
F 1 "RN4871" H 3500 3424 50  0000 C CNN
F 2 "Encoder:Bluetooth_Module_RN4871" H 3400 2850 50  0001 C CNN
F 3 "" H 3400 2850 50  0001 C CNN
	1    3450 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5EB353B8
P 2600 2400
F 0 "#PWR0101" H 2600 2150 50  0001 C CNN
F 1 "GND" H 2605 2227 50  0000 C CNN
F 2 "" H 2600 2400 50  0001 C CNN
F 3 "" H 2600 2400 50  0001 C CNN
	1    2600 2400
	1    0    0    -1  
$EndComp
NoConn ~ 3050 2300
NoConn ~ 3050 2500
NoConn ~ 3050 2600
NoConn ~ 3050 2700
NoConn ~ 3250 3200
NoConn ~ 3550 3200
NoConn ~ 3750 3200
NoConn ~ 3950 2400
$Comp
L power:GND #PWR0102
U 1 1 5EB35853
P 4750 2600
F 0 "#PWR0102" H 4750 2350 50  0001 C CNN
F 1 "GND" H 4755 2427 50  0000 C CNN
F 2 "" H 4750 2600 50  0001 C CNN
F 3 "" H 4750 2600 50  0001 C CNN
	1    4750 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2600 3950 2600
$Comp
L Connector:Conn_01x01_Male J4
U 1 1 5EB36697
P 4750 2300
F 0 "J4" V 4812 2344 50  0000 L CNN
F 1 "Conn_01x01_Male" V 4903 2344 50  0000 L CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical" H 4750 2300 50  0001 C CNN
F 3 "~" H 4750 2300 50  0001 C CNN
	1    4750 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 2500 4450 2500
$Comp
L Device:LED D1
U 1 1 5EB37166
P 4800 3200
F 0 "D1" H 4793 3416 50  0000 C CNN
F 1 "LED" H 4793 3325 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 4800 3200 50  0001 C CNN
F 3 "~" H 4800 3200 50  0001 C CNN
	1    4800 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EB37E07
P 4400 3050
F 0 "R1" H 4470 3096 50  0000 L CNN
F 1 "330" H 4470 3005 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4330 3050 50  0001 C CNN
F 3 "~" H 4400 3050 50  0001 C CNN
	1    4400 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2700 4400 2700
Wire Wire Line
	4400 2700 4400 2900
Wire Wire Line
	4400 3200 4650 3200
$Comp
L power:+3.3V #PWR0103
U 1 1 5EB39322
P 4950 3200
F 0 "#PWR0103" H 4950 3050 50  0001 C CNN
F 1 "+3.3V" H 4965 3373 50  0000 C CNN
F 2 "" H 4950 3200 50  0001 C CNN
F 3 "" H 4950 3200 50  0001 C CNN
	1    4950 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5EB3A163
P 5600 2500
F 0 "#PWR0104" H 5600 2350 50  0001 C CNN
F 1 "+3.3V" H 5615 2673 50  0000 C CNN
F 2 "" H 5600 2500 50  0001 C CNN
F 3 "" H 5600 2500 50  0001 C CNN
	1    5600 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2500 5450 2500
Connection ~ 4750 2500
Wire Wire Line
	3350 3200 3350 3500
Wire Wire Line
	3450 3500 3450 3200
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 5EB3AAC2
P 3350 3700
F 0 "J2" V 3504 3612 50  0000 R CNN
F 1 "Conn_01x01_Male" V 3413 3612 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical" H 3350 3700 50  0001 C CNN
F 3 "~" H 3350 3700 50  0001 C CNN
	1    3350 3700
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 5EB3B90B
P 3450 3700
F 0 "J3" V 3604 3612 50  0000 R CNN
F 1 "Conn_01x01_Male" V 3513 3612 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical" H 3450 3700 50  0001 C CNN
F 3 "~" H 3450 3700 50  0001 C CNN
	1    3450 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 2400 2600 2400
Wire Wire Line
	2600 2400 2300 2400
Connection ~ 2600 2400
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 5EB3BEB1
P 2300 2200
F 0 "J1" V 2362 2244 50  0000 L CNN
F 1 "Conn_01x01_Male" V 2453 2244 50  0000 L CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical" H 2300 2200 50  0001 C CNN
F 3 "~" H 2300 2200 50  0001 C CNN
	1    2300 2200
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5EB3C810
P 5450 2650
F 0 "C2" H 5565 2696 50  0000 L CNN
F 1 "10uF" H 5565 2605 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5488 2500 50  0001 C CNN
F 3 "~" H 5450 2650 50  0001 C CNN
	1    5450 2650
	1    0    0    -1  
$EndComp
Connection ~ 5450 2500
Wire Wire Line
	5450 2500 4750 2500
$Comp
L power:GND #PWR0105
U 1 1 5EB3CD36
P 5450 2800
F 0 "#PWR0105" H 5450 2550 50  0001 C CNN
F 1 "GND" H 5455 2627 50  0000 C CNN
F 2 "" H 5450 2800 50  0001 C CNN
F 3 "" H 5450 2800 50  0001 C CNN
	1    5450 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3200 4950 3450
Connection ~ 4950 3200
$Comp
L Device:R R2
U 1 1 5EB3D384
P 4950 3600
F 0 "R2" H 5020 3646 50  0000 L CNN
F 1 "10K" H 5020 3555 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4880 3600 50  0001 C CNN
F 3 "~" H 4950 3600 50  0001 C CNN
	1    4950 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3750 3650 3750
Wire Wire Line
	3650 3750 3650 3200
Wire Wire Line
	4950 3750 4950 3900
Connection ~ 4950 3750
$Comp
L Device:C C1
U 1 1 5EB3DD36
P 4950 4050
F 0 "C1" H 5065 4096 50  0000 L CNN
F 1 "1uF" H 5065 4005 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4988 3900 50  0001 C CNN
F 3 "~" H 4950 4050 50  0001 C CNN
	1    4950 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5EB3E94B
P 4950 4200
F 0 "#PWR0106" H 4950 3950 50  0001 C CNN
F 1 "GND" H 4955 4027 50  0000 C CNN
F 2 "" H 4950 4200 50  0001 C CNN
F 3 "" H 4950 4200 50  0001 C CNN
	1    4950 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2300 4450 2300
Wire Wire Line
	4450 2300 4450 2500
Connection ~ 4450 2500
Wire Wire Line
	4450 2500 3950 2500
$EndSCHEMATC
