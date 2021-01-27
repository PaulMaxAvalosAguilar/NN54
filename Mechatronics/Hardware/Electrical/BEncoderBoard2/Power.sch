EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "Encoder Power System"
Date "2020-06-30"
Rev "v1"
Comp "S.D.T."
Comment1 "Author:Paul Max Avalos Aguilar"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 7100 2750
Wire Wire Line
	7100 3500 7100 2750
$Comp
L power:GND #PWR?
U 1 1 5F136FFF
P 7950 3050
AR Path="/5F136FFF" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F136FFF" Ref="#PWR0129"  Part="1" 
F 0 "#PWR0129" H 7950 2800 50  0001 C CNN
F 1 "GND" H 7955 2877 50  0000 C CNN
F 2 "" H 7950 3050 50  0001 C CNN
F 3 "" H 7950 3050 50  0001 C CNN
	1    7950 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F137006
P 7600 3600
AR Path="/5F137006" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137006" Ref="#PWR0130"  Part="1" 
F 0 "#PWR0130" H 7600 3350 50  0001 C CNN
F 1 "GND" H 7605 3427 50  0000 C CNN
F 2 "" H 7600 3600 50  0001 C CNN
F 3 "" H 7600 3600 50  0001 C CNN
	1    7600 3600
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5F13700C
P 2150 2500
AR Path="/5F13700C" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13700C" Ref="#PWR0131"  Part="1" 
F 0 "#PWR0131" H 2150 2350 50  0001 C CNN
F 1 "VCC" H 2167 2673 50  0000 C CNN
F 2 "" H 2150 2500 50  0001 C CNN
F 3 "" H 2150 2500 50  0001 C CNN
	1    2150 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F137012
P 2150 3050
AR Path="/5F137012" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137012" Ref="#PWR0132"  Part="1" 
F 0 "#PWR0132" H 2150 2800 50  0001 C CNN
F 1 "GND" H 2155 2877 50  0000 C CNN
F 2 "" H 2150 3050 50  0001 C CNN
F 3 "" H 2150 3050 50  0001 C CNN
	1    2150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2950 2150 3050
$Comp
L Device:R R?
U 1 1 5F137029
P 7950 2900
AR Path="/5F137029" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137029" Ref="R9"  Part="1" 
F 0 "R9" H 8020 2946 50  0000 L CNN
F 1 "10K" H 8020 2855 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7880 2900 50  0001 C CNN
F 3 "~" H 7950 2900 50  0001 C CNN
	1    7950 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13702F
P 8950 3450
AR Path="/5F13702F" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13702F" Ref="#PWR0133"  Part="1" 
F 0 "#PWR0133" H 8950 3200 50  0001 C CNN
F 1 "GND" H 8955 3277 50  0000 C CNN
F 2 "" H 8950 3450 50  0001 C CNN
F 3 "" H 8950 3450 50  0001 C CNN
	1    8950 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137035
P 10450 3800
AR Path="/5F137035" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137035" Ref="C18"  Part="1" 
F 0 "C18" H 10565 3846 50  0000 L CNN
F 1 "1uF" H 10565 3755 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10488 3650 50  0001 C CNN
F 3 "~" H 10450 3800 50  0001 C CNN
	1    10450 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 3650 10250 3650
Wire Wire Line
	10250 3650 10250 3700
Wire Wire Line
	10250 3700 10100 3700
Wire Wire Line
	10100 3900 10250 3900
Wire Wire Line
	10250 3900 10250 3950
Wire Wire Line
	10250 3950 10450 3950
$Comp
L Device:C C?
U 1 1 5F137041
P 8600 3550
AR Path="/5F137041" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137041" Ref="C16"  Part="1" 
F 0 "C16" H 8715 3596 50  0000 L CNN
F 1 "1uF" H 8715 3505 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8638 3400 50  0001 C CNN
F 3 "~" H 8600 3550 50  0001 C CNN
	1    8600 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137047
P 8600 4050
AR Path="/5F137047" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137047" Ref="C17"  Part="1" 
F 0 "C17" H 8715 4096 50  0000 L CNN
F 1 "4.7uF" H 8715 4005 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8638 3900 50  0001 C CNN
F 3 "~" H 8600 4050 50  0001 C CNN
	1    8600 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13704D
P 8600 4300
AR Path="/5F13704D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13704D" Ref="#PWR0134"  Part="1" 
F 0 "#PWR0134" H 8600 4050 50  0001 C CNN
F 1 "GND" H 8605 4127 50  0000 C CNN
F 2 "" H 8600 4300 50  0001 C CNN
F 3 "" H 8600 4300 50  0001 C CNN
	1    8600 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 3900 8600 3900
Wire Wire Line
	8600 4300 8600 4200
Connection ~ 8600 3700
Wire Wire Line
	8600 3700 8950 3700
$Comp
L power:GND #PWR?
U 1 1 5F137057
P 8600 3400
AR Path="/5F137057" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137057" Ref="#PWR0135"  Part="1" 
F 0 "#PWR0135" H 8600 3150 50  0001 C CNN
F 1 "GND" H 8605 3227 50  0000 C CNN
F 2 "" H 8600 3400 50  0001 C CNN
F 3 "" H 8600 3400 50  0001 C CNN
	1    8600 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7100 2750 7400 2750
Wire Wire Line
	7300 3800 7400 3800
Wire Wire Line
	7400 2850 7400 2750
Connection ~ 7400 2750
Wire Wire Line
	7400 2750 7950 2750
Wire Wire Line
	8450 3700 8450 3000
Wire Wire Line
	8450 3000 10100 3000
Wire Wire Line
	10100 3000 10100 3100
Wire Wire Line
	8450 3700 8600 3700
$Comp
L Device:R R?
U 1 1 5F137066
P 10100 3250
AR Path="/5F137066" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137066" Ref="R10"  Part="1" 
F 0 "R10" H 10170 3296 50  0000 L CNN
F 1 "10K" H 10170 3205 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10030 3250 50  0001 C CNN
F 3 "~" H 10100 3250 50  0001 C CNN
	1    10100 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 3400 10100 3450
$Comp
L power:+3.3V #PWR?
U 1 1 5F13706D
P 8450 3900
AR Path="/5F13706D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13706D" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 8450 3750 50  0001 C CNN
F 1 "+3.3V" V 8465 4028 50  0000 L CNN
F 2 "" H 8450 3900 50  0001 C CNN
F 3 "" H 8450 3900 50  0001 C CNN
	1    8450 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4050 2750 4050 3800
Connection ~ 4050 2750
Wire Wire Line
	4050 2750 3700 2750
Wire Wire Line
	4600 3600 4350 3600
$Comp
L Device:R R?
U 1 1 5F137077
P 4350 4050
AR Path="/5F137077" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137077" Ref="R6"  Part="1" 
F 0 "R6" H 4100 4100 50  0000 L CNN
F 1 "1.5K" H 4050 4000 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4280 4050 50  0001 C CNN
F 3 "~" H 4350 4050 50  0001 C CNN
	1    4350 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13707D
P 4350 4200
AR Path="/5F13707D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13707D" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0137" H 4350 3950 50  0001 C CNN
F 1 "GND" H 4355 4027 50  0000 C CNN
F 2 "" H 4350 4200 50  0001 C CNN
F 3 "" H 4350 4200 50  0001 C CNN
	1    4350 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137083
P 3700 2900
AR Path="/5F137083" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137083" Ref="C14"  Part="1" 
F 0 "C14" H 3815 2946 50  0000 L CNN
F 1 "10uF" H 3815 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3738 2750 50  0001 C CNN
F 3 "~" H 3700 2900 50  0001 C CNN
	1    3700 2900
	1    0    0    -1  
$EndComp
Connection ~ 3700 2750
$Comp
L power:GND #PWR?
U 1 1 5F13708A
P 4550 3900
AR Path="/5F13708A" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13708A" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0138" H 4550 3650 50  0001 C CNN
F 1 "GND" H 4555 3727 50  0000 C CNN
F 2 "" H 4550 3900 50  0001 C CNN
F 3 "" H 4550 3900 50  0001 C CNN
	1    4550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3900 4550 3700
Wire Wire Line
	4550 3700 4600 3700
Wire Wire Line
	6000 3500 6000 2750
Connection ~ 6000 2750
$Comp
L Device:R R?
U 1 1 5F1370A0
P 6300 4050
AR Path="/5F1370A0" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F1370A0" Ref="R8"  Part="1" 
F 0 "R8" V 6507 4050 50  0000 C CNN
F 1 "100" V 6416 4050 50  0000 C CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6230 4050 50  0001 C CNN
F 3 "~" H 6300 4050 50  0001 C CNN
	1    6300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4700 6600 4700
Wire Wire Line
	6600 4700 6600 4300
$Comp
L Device:C C?
U 1 1 5F1370A8
P 6300 4450
AR Path="/5F1370A8" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F1370A8" Ref="C15"  Part="1" 
F 0 "C15" H 6415 4496 50  0000 L CNN
F 1 "100nF" H 6415 4405 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6338 4300 50  0001 C CNN
F 3 "~" H 6300 4450 50  0001 C CNN
	1    6300 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4300 6300 4200
Connection ~ 6300 4300
Wire Wire Line
	6300 4300 6600 4300
Wire Wire Line
	6300 4600 6200 4600
NoConn ~ 6200 4800
Wire Wire Line
	5200 4600 4700 4600
Wire Wire Line
	4700 4600 4700 5250
Wire Wire Line
	4700 5250 5250 5250
$Comp
L Device:R R?
U 1 1 5F1370BC
P 4850 4850
AR Path="/5F1370BC" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F1370BC" Ref="R7"  Part="1" 
F 0 "R7" V 5057 4850 50  0000 C CNN
F 1 "1K" V 4966 4850 50  0000 C CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4780 4850 50  0001 C CNN
F 3 "~" H 4850 4850 50  0001 C CNN
	1    4850 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 4700 4850 4700
$Comp
L power:GND #PWR?
U 1 1 5F1370C3
P 4850 5000
AR Path="/5F1370C3" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370C3" Ref="#PWR0139"  Part="1" 
F 0 "#PWR0139" H 4850 4750 50  0001 C CNN
F 1 "GND" H 4855 4827 50  0000 C CNN
F 2 "" H 4850 5000 50  0001 C CNN
F 3 "" H 4850 5000 50  0001 C CNN
	1    4850 5000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4050 3800 4600 3800
Wire Wire Line
	6300 3900 6300 3800
Connection ~ 6300 3800
Wire Wire Line
	2150 2950 2150 2850
Wire Wire Line
	2150 2850 1600 2850
Connection ~ 2150 2950
Connection ~ 2150 2750
$Comp
L power:GND #PWR?
U 1 1 5F1370E3
P 3700 3050
AR Path="/5F1370E3" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370E3" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0140" H 3700 2800 50  0001 C CNN
F 1 "GND" H 3705 2877 50  0000 C CNN
F 2 "" H 3700 3050 50  0001 C CNN
F 3 "" H 3700 3050 50  0001 C CNN
	1    3700 3050
	1    0    0    -1  
$EndComp
Connection ~ 7400 3800
$Comp
L power:GND #PWR?
U 1 1 5F1370F4
P 4150 3500
AR Path="/5F1370F4" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370F4" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0141" H 4150 3250 50  0001 C CNN
F 1 "GND" H 4155 3327 50  0000 C CNN
F 2 "" H 4150 3500 50  0001 C CNN
F 3 "" H 4150 3500 50  0001 C CNN
	1    4150 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3500 4150 3500
Wire Wire Line
	4350 3600 4350 3900
Wire Wire Line
	6000 2750 7100 2750
$Comp
L Encoder:Q_PMOS_GSD Q?
U 1 1 5F1370FD
P 7100 3750
AR Path="/5F1370FD" Ref="Q?"  Part="1" 
AR Path="/5F11DB77/5F1370FD" Ref="Q2"  Part="1" 
F 0 "Q2" H 7100 3550 50  0000 C CNN
F 1 "Q_PMOS_GSD" H 7100 3459 50  0000 C CNN
F 2 "Encoder:SOT-23" V 7300 3600 50  0001 C CNN
F 3 "~" V 7100 3800 50  0001 C CNN
	1    7100 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:D D?
U 1 1 5F137103
P 7400 3000
AR Path="/5F137103" Ref="D?"  Part="1" 
AR Path="/5F11DB77/5F137103" Ref="D1"  Part="1" 
F 0 "D1" V 7446 2921 50  0000 R CNN
F 1 "D" V 7355 2921 50  0000 R CNN
F 2 "Encoder:D_SMA" H 7400 3000 50  0001 C CNN
F 3 "~" H 7400 3000 50  0001 C CNN
	1    7400 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7400 3150 7400 3800
Wire Wire Line
	2150 2750 2450 2750
$Comp
L Device:C C?
U 1 1 5F13710D
P 2900 2900
AR Path="/5F13710D" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F13710D" Ref="C12"  Part="1" 
F 0 "C12" H 3015 2946 50  0000 L CNN
F 1 "1uF" H 3015 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2938 2750 50  0001 C CNN
F 3 "~" H 2900 2900 50  0001 C CNN
	1    2900 2900
	1    0    0    -1  
$EndComp
Connection ~ 2900 2750
Wire Wire Line
	3300 3050 3300 3200
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137115
P 3300 3200
AR Path="/5F137115" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137115" Ref="#PWR0142"  Part="1" 
F 0 "#PWR0142" H 3300 3000 50  0001 C CNN
F 1 "GNDPWR" H 3304 3046 50  0000 C CNN
F 2 "" H 3300 3150 50  0001 C CNN
F 3 "" H 3300 3150 50  0001 C CNN
	1    3300 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 5F13711B
P 2450 4550
AR Path="/5F13711B" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13711B" Ref="#PWR0143"  Part="1" 
F 0 "#PWR0143" H 2450 4350 50  0001 C CNN
F 1 "GNDPWR" H 2454 4396 50  0000 C CNN
F 2 "" H 2450 4500 50  0001 C CNN
F 3 "" H 2450 4500 50  0001 C CNN
	1    2450 4550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Barrel_Jack_Switch J?
U 1 1 5F137121
P 1300 2850
AR Path="/5F137121" Ref="J?"  Part="1" 
AR Path="/5F11DB77/5F137121" Ref="J3"  Part="1" 
F 0 "J3" H 1355 3167 50  0000 C CNN
F 1 "Barrel_Jack_Switch" H 1355 3076 50  0000 C CNN
F 2 "Encoder:BarrelJack_Horizontal" H 1350 2810 50  0001 C CNN
F 3 "~" H 1350 2810 50  0001 C CNN
	1    1300 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2750 3300 2750
NoConn ~ 2250 3450
NoConn ~ 2650 3450
$Comp
L Device:C C?
U 1 1 5F13712C
P 3300 2900
AR Path="/5F13712C" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F13712C" Ref="C13"  Part="1" 
F 0 "C13" H 3415 2946 50  0000 L CNN
F 1 "1uF" H 3415 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3338 2750 50  0001 C CNN
F 3 "~" H 3300 2900 50  0001 C CNN
	1    3300 2900
	1    0    0    -1  
$EndComp
Connection ~ 3300 2750
Wire Wire Line
	2900 3050 2900 3200
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137134
P 2900 3200
AR Path="/5F137134" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137134" Ref="#PWR0144"  Part="1" 
F 0 "#PWR0144" H 2900 3000 50  0001 C CNN
F 1 "GNDPWR" H 2904 3046 50  0000 C CNN
F 2 "" H 2900 3150 50  0001 C CNN
F 3 "" H 2900 3150 50  0001 C CNN
	1    2900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4600 6300 5100
Wire Wire Line
	6300 5100 6500 5100
Connection ~ 6300 4600
Connection ~ 6750 5100
$Comp
L Connector:Conn_01x01_Male J?
U 1 1 5F13713F
P 1700 3900
AR Path="/5F13713F" Ref="J?"  Part="1" 
AR Path="/5F11DB77/5F13713F" Ref="J4"  Part="1" 
F 0 "J4" H 1673 3830 50  0000 R CNN
F 1 "GNDPWR" H 1673 3921 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical" H 1700 3900 50  0001 C CNN
F 3 "~" H 1700 3900 50  0001 C CNN
	1    1700 3900
	0    1    1    0   
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137145
P 1700 4200
AR Path="/5F137145" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137145" Ref="#PWR0145"  Part="1" 
F 0 "#PWR0145" H 1700 4000 50  0001 C CNN
F 1 "GNDPWR" H 1704 4046 50  0000 C CNN
F 2 "" H 1700 4150 50  0001 C CNN
F 3 "" H 1700 4150 50  0001 C CNN
	1    1700 4200
	1    0    0    -1  
$EndComp
$Comp
L Encoder:LTC3240 U?
U 1 1 5F13714B
P 9500 3750
AR Path="/5F13714B" Ref="U?"  Part="1" 
AR Path="/5F11DB77/5F13714B" Ref="U6"  Part="1" 
F 0 "U6" H 9525 4315 50  0000 C CNN
F 1 "LTC3240" H 9525 4224 50  0000 C CNN
F 2 "Encoder:DFN-6_DC" H 9500 3750 50  0001 C CNN
F 3 "" H 9500 3750 50  0001 C CNN
	1    9500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2750 3700 2750
Wire Wire Line
	2450 3450 2450 2750
Connection ~ 2450 2750
Wire Wire Line
	2450 2750 2900 2750
NoConn ~ 6000 3600
NoConn ~ 6000 3700
Wire Wire Line
	5200 4800 5150 4800
Wire Wire Line
	5150 4800 5150 5150
Wire Wire Line
	5150 5150 5650 5150
Wire Wire Line
	5000 5600 4750 5600
Wire Wire Line
	4750 5600 4750 5950
Wire Wire Line
	4750 5950 6750 5950
$Comp
L power:GND #PWR?
U 1 1 5F137162
P 6100 5600
AR Path="/5F137162" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137162" Ref="#PWR0146"  Part="1" 
F 0 "#PWR0146" H 6100 5350 50  0001 C CNN
F 1 "GND" H 6105 5427 50  0000 C CNN
F 2 "" H 6100 5600 50  0001 C CNN
F 3 "" H 6100 5600 50  0001 C CNN
	1    6100 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137168
P 6350 5600
AR Path="/5F137168" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137168" Ref="#PWR0147"  Part="1" 
F 0 "#PWR0147" H 6350 5400 50  0001 C CNN
F 1 "GNDPWR" H 6354 5446 50  0000 C CNN
F 2 "" H 6350 5550 50  0001 C CNN
F 3 "" H 6350 5550 50  0001 C CNN
	1    6350 5600
	1    0    0    -1  
$EndComp
NoConn ~ 5450 5850
NoConn ~ 5450 5300
Wire Wire Line
	6750 5100 6750 5950
Wire Wire Line
	5650 5150 5650 5250
Wire Wire Line
	2450 4350 2450 4550
Connection ~ 8450 3700
Wire Wire Line
	8600 3900 8450 3900
Connection ~ 8600 3900
Wire Wire Line
	6000 3800 6300 3800
NoConn ~ 2650 4350
NoConn ~ 2250 4350
Wire Wire Line
	1600 2750 1900 2750
$Comp
L Encoder:82402305 U5
U 1 1 5F2B218E
P 2450 3800
F 0 "U5" H 2778 3746 50  0000 L CNN
F 1 "82402305" H 2778 3655 50  0000 L CNN
F 2 "Encoder:Sot-23(6)" H 2450 3300 50  0001 C CNN
F 3 "" H 2450 3300 50  0001 C CNN
	1    2450 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2500 2150 2750
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5F2C34E4
P 1900 2750
F 0 "#FLG0101" H 1900 2825 50  0001 C CNN
F 1 "PWR_FLAG" H 1900 2923 50  0000 C CNN
F 2 "" H 1900 2750 50  0001 C CNN
F 3 "~" H 1900 2750 50  0001 C CNN
	1    1900 2750
	1    0    0    -1  
$EndComp
Connection ~ 1900 2750
Wire Wire Line
	1900 2750 2150 2750
$Comp
L Encoder:TP4056 IC1
U 1 1 5F2D92FF
P 4600 3500
F 0 "IC1" H 5300 3765 50  0000 C CNN
F 1 "TP4056" H 5300 3674 50  0000 C CNN
F 2 "Encoder:Sop(8)" H 5850 3600 50  0001 L CNN
F 3 "https://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/TP4056.pdf" H 5850 3500 50  0001 L CNN
F 4 "1A Standalone Linear Li-lon Battery Charger, SOP-8" H 5850 3400 50  0001 L CNN "Description"
F 5 "1.75" H 5850 3300 50  0001 L CNN "Height"
F 6 "NanJing Top Power" H 5850 3200 50  0001 L CNN "Manufacturer_Name"
F 7 "TP4056" H 5850 3100 50  0001 L CNN "Manufacturer_Part_Number"
	1    4600 3500
	1    0    0    -1  
$EndComp
Connection ~ 6100 5600
Wire Wire Line
	6100 5600 6350 5600
Wire Wire Line
	8100 3700 8250 3700
Wire Wire Line
	8250 3700 8450 3700
Connection ~ 8250 3700
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 5F29970B
P 8250 3700
F 0 "#FLG0104" H 8250 3775 50  0001 C CNN
F 1 "PWR_FLAG" H 8250 3873 50  0000 C CNN
F 2 "" H 8250 3700 50  0001 C CNN
F 3 "~" H 8250 3700 50  0001 C CNN
	1    8250 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6750 5100 7000 5100
Wire Wire Line
	1700 4100 1700 4200
Wire Wire Line
	1600 2950 1800 2950
$Comp
L Encoder:DW01+G IC2
U 1 1 5F2EE54D
P 5200 4600
F 0 "IC2" H 5700 4865 50  0000 C CNN
F 1 "DW01+G" H 5700 4774 50  0000 C CNN
F 2 "Encoder:Sot-23(6)" H 6050 4700 50  0001 L CNN
F 3 "https://datasheet.lcsc.com/szlcsc/Fortune-Semicon-DW01-G_C14213.pdf" H 6050 4600 50  0001 L CNN
F 4 "One Cell Lithium-ion/Polymer Battery Protection IC" H 6050 4500 50  0001 L CNN "Description"
F 5 "1.45" H 6050 4400 50  0001 L CNN "Height"
F 6 "fortune" H 6050 4300 50  0001 L CNN "Manufacturer_Name"
F 7 "DW01+G" H 6050 4200 50  0001 L CNN "Manufacturer_Part_Number"
	1    5200 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5600 6100 5600
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5F2F6061
P 6600 4700
F 0 "#FLG0103" H 6600 4775 50  0001 C CNN
F 1 "PWR_FLAG" H 6600 4873 50  0000 C CNN
F 2 "" H 6600 4700 50  0001 C CNN
F 3 "~" H 6600 4700 50  0001 C CNN
	1    6600 4700
	0    1    -1   0   
$EndComp
Connection ~ 6600 4700
$Comp
L power:PWR_FLAG #FLG0105
U 1 1 5F2F74EC
P 1800 2950
F 0 "#FLG0105" H 1800 3025 50  0001 C CNN
F 1 "PWR_FLAG" H 1800 3123 50  0000 C CNN
F 2 "" H 1800 2950 50  0001 C CNN
F 3 "~" H 1800 2950 50  0001 C CNN
	1    1800 2950
	1    0    0    1   
$EndComp
Connection ~ 1800 2950
Wire Wire Line
	1800 2950 2150 2950
Text GLabel 6500 3800 1    50   Input ~ 0
Batt+
Wire Wire Line
	4050 2750 6000 2750
Text GLabel 4600 2750 1    50   Input ~ 0
VCC
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 5F03A757
P 8250 5050
F 0 "J5" H 8222 4932 50  0000 R CNN
F 1 "Batt_Conn" H 8222 5023 50  0000 R CNN
F 2 "Encoder:PinHeader_1x02_P2.54mm_Vertical" H 8250 5050 50  0001 C CNN
F 3 "~" H 8250 5050 50  0001 C CNN
	1    8250 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	6300 3800 6900 3800
Text GLabel 7000 5100 2    50   Input ~ 0
Batt-
Text GLabel 8050 5050 0    50   Input ~ 0
Batt-
Text GLabel 8050 4950 0    50   Input ~ 0
Batt+
$Comp
L Encoder:DMosfet Q1
U 1 1 5F0365A7
P 5450 5600
F 0 "Q1" H 5944 5696 50  0000 L CNN
F 1 "DMosfet" H 5944 5605 50  0000 L CNN
F 2 "Encoder:Sot-23(6)" V 5350 5500 50  0001 C CNN
F 3 "~" V 5250 5700 50  0001 C CNN
	1    5450 5600
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0106
U 1 1 5F08AA9A
P 6500 5100
F 0 "#FLG0106" H 6500 5175 50  0001 C CNN
F 1 "PWR_FLAG" H 6500 5273 50  0000 C CNN
F 2 "" H 6500 5100 50  0001 C CNN
F 3 "~" H 6500 5100 50  0001 C CNN
	1    6500 5100
	-1   0    0    -1  
$EndComp
Connection ~ 6500 5100
Wire Wire Line
	6500 5100 6750 5100
Wire Wire Line
	7600 3600 7700 3600
Wire Wire Line
	7400 3800 7700 3800
$Comp
L Encoder:EG1218 S1
U 1 1 5F1D7B37
P 7900 3700
F 0 "S1" H 7950 3500 50  0000 C CNN
F 1 "EG1218_Conn" H 8000 4000 50  0000 C CNN
F 2 "Encoder:PinHeader_1x03_P2.54mm_Vertical" H 8100 3900 50  0001 L CNN
F 3 "http://spec_sheets.e-switch.com/specs/P040040.pdf" H 8100 4000 60  0001 L CNN
F 4 "EG1903-ND" H 8100 4100 60  0001 L CNN "Digi-Key_PN"
F 5 "EG1218" H 8100 4200 60  0001 L CNN "MPN"
F 6 "Switches" H 8100 4300 60  0001 L CNN "Category"
F 7 "Slide Switches" H 8100 4400 60  0001 L CNN "Family"
F 8 "http://spec_sheets.e-switch.com/specs/P040040.pdf" H 8100 4500 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/e-switch/EG1218/EG1903-ND/101726" H 8100 4600 60  0001 L CNN "DK_Detail_Page"
F 10 "SWITCH SLIDE SPDT 200MA 30V" H 8100 4700 60  0001 L CNN "Description"
F 11 "E-Switch" H 8100 4800 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8100 4900 60  0001 L CNN "Status"
	1    7900 3700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF02BF1
P 5300 4300
F 0 "#PWR?" H 5300 4050 50  0001 C CNN
F 1 "GND" H 5300 4150 50  0000 C CNN
F 2 "" H 5300 4300 50  0001 C CNN
F 3 "" H 5300 4300 50  0001 C CNN
	1    5300 4300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
