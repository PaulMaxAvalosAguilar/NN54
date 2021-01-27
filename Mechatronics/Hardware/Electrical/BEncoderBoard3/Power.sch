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
Connection ~ 7500 2750
Wire Wire Line
	7500 3500 7500 2750
$Comp
L power:GND #PWR?
U 1 1 5F136FFF
P 8350 3050
AR Path="/5F136FFF" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F136FFF" Ref="#PWR0129"  Part="1" 
F 0 "#PWR0129" H 8350 2800 50  0001 C CNN
F 1 "GND" H 8355 2877 50  0000 C CNN
F 2 "" H 8350 3050 50  0001 C CNN
F 3 "" H 8350 3050 50  0001 C CNN
	1    8350 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F137006
P 8000 3600
AR Path="/5F137006" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137006" Ref="#PWR0130"  Part="1" 
F 0 "#PWR0130" H 8000 3350 50  0001 C CNN
F 1 "GND" H 8005 3427 50  0000 C CNN
F 2 "" H 8000 3600 50  0001 C CNN
F 3 "" H 8000 3600 50  0001 C CNN
	1    8000 3600
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 5F13700C
P 2800 2500
AR Path="/5F13700C" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13700C" Ref="#PWR0131"  Part="1" 
F 0 "#PWR0131" H 2800 2350 50  0001 C CNN
F 1 "VCC" H 2817 2673 50  0000 C CNN
F 2 "" H 2800 2500 50  0001 C CNN
F 3 "" H 2800 2500 50  0001 C CNN
	1    2800 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F137012
P 2500 3000
AR Path="/5F137012" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137012" Ref="#PWR0132"  Part="1" 
F 0 "#PWR0132" H 2500 2750 50  0001 C CNN
F 1 "GND" H 2505 2827 50  0000 C CNN
F 2 "" H 2500 3000 50  0001 C CNN
F 3 "" H 2500 3000 50  0001 C CNN
	1    2500 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F137029
P 8350 2900
AR Path="/5F137029" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137029" Ref="R9"  Part="1" 
F 0 "R9" H 8420 2946 50  0000 L CNN
F 1 "10K" H 8420 2855 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8280 2900 50  0001 C CNN
F 3 "~" H 8350 2900 50  0001 C CNN
	1    8350 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13702F
P 9350 3450
AR Path="/5F13702F" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13702F" Ref="#PWR0133"  Part="1" 
F 0 "#PWR0133" H 9350 3200 50  0001 C CNN
F 1 "GND" H 9355 3277 50  0000 C CNN
F 2 "" H 9350 3450 50  0001 C CNN
F 3 "" H 9350 3450 50  0001 C CNN
	1    9350 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137035
P 10850 3800
AR Path="/5F137035" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137035" Ref="C18"  Part="1" 
F 0 "C18" H 10965 3846 50  0000 L CNN
F 1 "1uF" H 10965 3755 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10888 3650 50  0001 C CNN
F 3 "~" H 10850 3800 50  0001 C CNN
	1    10850 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10850 3650 10650 3650
Wire Wire Line
	10650 3650 10650 3700
Wire Wire Line
	10650 3700 10500 3700
Wire Wire Line
	10500 3900 10650 3900
Wire Wire Line
	10650 3900 10650 3950
Wire Wire Line
	10650 3950 10850 3950
$Comp
L Device:C C?
U 1 1 5F137041
P 9000 3550
AR Path="/5F137041" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137041" Ref="C16"  Part="1" 
F 0 "C16" H 9115 3596 50  0000 L CNN
F 1 "1uF" H 9115 3505 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9038 3400 50  0001 C CNN
F 3 "~" H 9000 3550 50  0001 C CNN
	1    9000 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137047
P 9000 4050
AR Path="/5F137047" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137047" Ref="C17"  Part="1" 
F 0 "C17" H 9115 4096 50  0000 L CNN
F 1 "4.7uF" H 9115 4005 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9038 3900 50  0001 C CNN
F 3 "~" H 9000 4050 50  0001 C CNN
	1    9000 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13704D
P 9000 4300
AR Path="/5F13704D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13704D" Ref="#PWR0134"  Part="1" 
F 0 "#PWR0134" H 9000 4050 50  0001 C CNN
F 1 "GND" H 9005 4127 50  0000 C CNN
F 2 "" H 9000 4300 50  0001 C CNN
F 3 "" H 9000 4300 50  0001 C CNN
	1    9000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3900 9000 3900
Wire Wire Line
	9000 4300 9000 4200
Connection ~ 9000 3700
Wire Wire Line
	9000 3700 9350 3700
$Comp
L power:GND #PWR?
U 1 1 5F137057
P 9000 3400
AR Path="/5F137057" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137057" Ref="#PWR0135"  Part="1" 
F 0 "#PWR0135" H 9000 3150 50  0001 C CNN
F 1 "GND" H 9005 3227 50  0000 C CNN
F 2 "" H 9000 3400 50  0001 C CNN
F 3 "" H 9000 3400 50  0001 C CNN
	1    9000 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7500 2750 7800 2750
Wire Wire Line
	7700 3800 7800 3800
Wire Wire Line
	7800 2850 7800 2750
Connection ~ 7800 2750
Wire Wire Line
	7800 2750 8350 2750
Wire Wire Line
	8850 3700 8850 3000
Wire Wire Line
	8850 3000 10500 3000
Wire Wire Line
	10500 3000 10500 3100
Wire Wire Line
	8850 3700 9000 3700
$Comp
L Device:R R?
U 1 1 5F137066
P 10500 3250
AR Path="/5F137066" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137066" Ref="R10"  Part="1" 
F 0 "R10" H 10570 3296 50  0000 L CNN
F 1 "10K" H 10570 3205 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10430 3250 50  0001 C CNN
F 3 "~" H 10500 3250 50  0001 C CNN
	1    10500 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3400 10500 3450
$Comp
L power:+3.3V #PWR?
U 1 1 5F13706D
P 8850 3900
AR Path="/5F13706D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13706D" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 8850 3750 50  0001 C CNN
F 1 "+3.3V" V 8865 4028 50  0000 L CNN
F 2 "" H 8850 3900 50  0001 C CNN
F 3 "" H 8850 3900 50  0001 C CNN
	1    8850 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4450 2750 4450 3800
Connection ~ 4450 2750
Wire Wire Line
	4450 2750 4100 2750
Wire Wire Line
	5000 3600 4750 3600
$Comp
L Device:R R?
U 1 1 5F137077
P 4750 4050
AR Path="/5F137077" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F137077" Ref="R6"  Part="1" 
F 0 "R6" H 4500 4100 50  0000 L CNN
F 1 "1.5K" H 4450 4000 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4680 4050 50  0001 C CNN
F 3 "~" H 4750 4050 50  0001 C CNN
	1    4750 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13707D
P 4750 4200
AR Path="/5F13707D" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13707D" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0137" H 4750 3950 50  0001 C CNN
F 1 "GND" H 4755 4027 50  0000 C CNN
F 2 "" H 4750 4200 50  0001 C CNN
F 3 "" H 4750 4200 50  0001 C CNN
	1    4750 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F137083
P 4100 2900
AR Path="/5F137083" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F137083" Ref="C14"  Part="1" 
F 0 "C14" H 4215 2946 50  0000 L CNN
F 1 "10uF" H 4215 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4138 2750 50  0001 C CNN
F 3 "~" H 4100 2900 50  0001 C CNN
	1    4100 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F13708A
P 4950 3900
AR Path="/5F13708A" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13708A" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0138" H 4950 3650 50  0001 C CNN
F 1 "GND" H 4955 3727 50  0000 C CNN
F 2 "" H 4950 3900 50  0001 C CNN
F 3 "" H 4950 3900 50  0001 C CNN
	1    4950 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3900 4950 3700
Wire Wire Line
	4950 3700 5000 3700
Wire Wire Line
	6400 3500 6400 2750
Connection ~ 6400 2750
$Comp
L Device:R R?
U 1 1 5F1370A0
P 6700 4050
AR Path="/5F1370A0" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F1370A0" Ref="R8"  Part="1" 
F 0 "R8" V 6907 4050 50  0000 C CNN
F 1 "100" V 6816 4050 50  0000 C CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6630 4050 50  0001 C CNN
F 3 "~" H 6700 4050 50  0001 C CNN
	1    6700 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 4700 7000 4700
Wire Wire Line
	7000 4700 7000 4300
$Comp
L Device:C C?
U 1 1 5F1370A8
P 6700 4450
AR Path="/5F1370A8" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F1370A8" Ref="C15"  Part="1" 
F 0 "C15" H 6815 4496 50  0000 L CNN
F 1 "100nF" H 6815 4405 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6738 4300 50  0001 C CNN
F 3 "~" H 6700 4450 50  0001 C CNN
	1    6700 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4300 6700 4200
Connection ~ 6700 4300
Wire Wire Line
	6700 4300 7000 4300
Wire Wire Line
	6700 4600 6600 4600
NoConn ~ 6600 4800
Wire Wire Line
	5600 4600 5100 4600
Wire Wire Line
	5100 4600 5100 5250
Wire Wire Line
	5100 5250 5650 5250
$Comp
L Device:R R?
U 1 1 5F1370BC
P 5250 4850
AR Path="/5F1370BC" Ref="R?"  Part="1" 
AR Path="/5F11DB77/5F1370BC" Ref="R7"  Part="1" 
F 0 "R7" V 5457 4850 50  0000 C CNN
F 1 "1K" V 5366 4850 50  0000 C CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5180 4850 50  0001 C CNN
F 3 "~" H 5250 4850 50  0001 C CNN
	1    5250 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4700 5250 4700
$Comp
L power:GND #PWR?
U 1 1 5F1370C3
P 5250 5000
AR Path="/5F1370C3" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370C3" Ref="#PWR0139"  Part="1" 
F 0 "#PWR0139" H 5250 4750 50  0001 C CNN
F 1 "GND" H 5255 4827 50  0000 C CNN
F 2 "" H 5250 5000 50  0001 C CNN
F 3 "" H 5250 5000 50  0001 C CNN
	1    5250 5000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4450 3800 5000 3800
Wire Wire Line
	6700 3900 6700 3800
Connection ~ 6700 3800
Connection ~ 2800 2750
$Comp
L power:GND #PWR?
U 1 1 5F1370E3
P 4100 3050
AR Path="/5F1370E3" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370E3" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0140" H 4100 2800 50  0001 C CNN
F 1 "GND" H 4105 2877 50  0000 C CNN
F 2 "" H 4100 3050 50  0001 C CNN
F 3 "" H 4100 3050 50  0001 C CNN
	1    4100 3050
	1    0    0    -1  
$EndComp
Connection ~ 7800 3800
$Comp
L power:GND #PWR?
U 1 1 5F1370F4
P 4550 3500
AR Path="/5F1370F4" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F1370F4" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0141" H 4550 3250 50  0001 C CNN
F 1 "GND" H 4555 3327 50  0000 C CNN
F 2 "" H 4550 3500 50  0001 C CNN
F 3 "" H 4550 3500 50  0001 C CNN
	1    4550 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3500 4550 3500
Wire Wire Line
	4750 3600 4750 3900
Wire Wire Line
	6400 2750 7500 2750
$Comp
L Encoder:Q_PMOS_GSD Q?
U 1 1 5F1370FD
P 7500 3750
AR Path="/5F1370FD" Ref="Q?"  Part="1" 
AR Path="/5F11DB77/5F1370FD" Ref="Q2"  Part="1" 
F 0 "Q2" H 7500 3550 50  0000 C CNN
F 1 "Q_PMOS_GSD" H 7500 3459 50  0000 C CNN
F 2 "Encoder:SOT-23" V 7700 3600 50  0001 C CNN
F 3 "~" V 7500 3800 50  0001 C CNN
	1    7500 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:D D?
U 1 1 5F137103
P 7800 3000
AR Path="/5F137103" Ref="D?"  Part="1" 
AR Path="/5F11DB77/5F137103" Ref="D1"  Part="1" 
F 0 "D1" V 7846 2921 50  0000 R CNN
F 1 "D" V 7755 2921 50  0000 R CNN
F 2 "Encoder:D_SMA" H 7800 3000 50  0001 C CNN
F 3 "~" H 7800 3000 50  0001 C CNN
	1    7800 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7800 3150 7800 3800
$Comp
L Device:C C?
U 1 1 5F13710D
P 3300 2900
AR Path="/5F13710D" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F13710D" Ref="C12"  Part="1" 
F 0 "C12" H 3415 2946 50  0000 L CNN
F 1 "1uF" H 3415 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3338 2750 50  0001 C CNN
F 3 "~" H 3300 2900 50  0001 C CNN
	1    3300 2900
	1    0    0    -1  
$EndComp
Connection ~ 3300 2750
Wire Wire Line
	3700 3050 3700 3200
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137115
P 3700 3200
AR Path="/5F137115" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137115" Ref="#PWR0142"  Part="1" 
F 0 "#PWR0142" H 3700 3000 50  0001 C CNN
F 1 "GNDPWR" H 3704 3046 50  0000 C CNN
F 2 "" H 3700 3150 50  0001 C CNN
F 3 "" H 3700 3150 50  0001 C CNN
	1    3700 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 5F13711B
P 2150 5100
AR Path="/5F13711B" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F13711B" Ref="#PWR0143"  Part="1" 
F 0 "#PWR0143" H 2150 4900 50  0001 C CNN
F 1 "GNDPWR" H 2154 4946 50  0000 C CNN
F 2 "" H 2150 5050 50  0001 C CNN
F 3 "" H 2150 5050 50  0001 C CNN
	1    2150 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2750 3700 2750
NoConn ~ 1950 4200
NoConn ~ 2350 4200
$Comp
L Device:C C?
U 1 1 5F13712C
P 3700 2900
AR Path="/5F13712C" Ref="C?"  Part="1" 
AR Path="/5F11DB77/5F13712C" Ref="C13"  Part="1" 
F 0 "C13" H 3815 2946 50  0000 L CNN
F 1 "1uF" H 3815 2855 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3738 2750 50  0001 C CNN
F 3 "~" H 3700 2900 50  0001 C CNN
	1    3700 2900
	1    0    0    -1  
$EndComp
Connection ~ 3700 2750
Wire Wire Line
	3300 3050 3300 3200
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137134
P 3300 3200
AR Path="/5F137134" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137134" Ref="#PWR0144"  Part="1" 
F 0 "#PWR0144" H 3300 3000 50  0001 C CNN
F 1 "GNDPWR" H 3304 3046 50  0000 C CNN
F 2 "" H 3300 3150 50  0001 C CNN
F 3 "" H 3300 3150 50  0001 C CNN
	1    3300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4600 6700 5100
Wire Wire Line
	6700 5100 6900 5100
Connection ~ 6700 4600
Connection ~ 7150 5100
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137145
P 1950 3750
AR Path="/5F137145" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137145" Ref="#PWR0145"  Part="1" 
F 0 "#PWR0145" H 1950 3550 50  0001 C CNN
F 1 "GNDPWR" H 1954 3596 50  0000 C CNN
F 2 "" H 1950 3700 50  0001 C CNN
F 3 "" H 1950 3700 50  0001 C CNN
	1    1950 3750
	1    0    0    -1  
$EndComp
$Comp
L Encoder:LTC3240 U?
U 1 1 5F13714B
P 9900 3750
AR Path="/5F13714B" Ref="U?"  Part="1" 
AR Path="/5F11DB77/5F13714B" Ref="U6"  Part="1" 
F 0 "U6" H 9925 4315 50  0000 C CNN
F 1 "LTC3240" H 9925 4224 50  0000 C CNN
F 2 "Encoder:DFN-6_DC" H 9900 3750 50  0001 C CNN
F 3 "" H 9900 3750 50  0001 C CNN
	1    9900 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2750 4100 2750
NoConn ~ 6400 3600
NoConn ~ 6400 3700
Wire Wire Line
	5600 4800 5550 4800
Wire Wire Line
	5550 4800 5550 5150
Wire Wire Line
	5550 5150 6050 5150
Wire Wire Line
	5400 5600 5150 5600
Wire Wire Line
	5150 5600 5150 5950
Wire Wire Line
	5150 5950 7150 5950
$Comp
L power:GND #PWR?
U 1 1 5F137162
P 6500 5600
AR Path="/5F137162" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137162" Ref="#PWR0146"  Part="1" 
F 0 "#PWR0146" H 6500 5350 50  0001 C CNN
F 1 "GND" H 6505 5427 50  0000 C CNN
F 2 "" H 6500 5600 50  0001 C CNN
F 3 "" H 6500 5600 50  0001 C CNN
	1    6500 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 5F137168
P 6750 5600
AR Path="/5F137168" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/5F137168" Ref="#PWR0147"  Part="1" 
F 0 "#PWR0147" H 6750 5400 50  0001 C CNN
F 1 "GNDPWR" H 6754 5446 50  0000 C CNN
F 2 "" H 6750 5550 50  0001 C CNN
F 3 "" H 6750 5550 50  0001 C CNN
	1    6750 5600
	1    0    0    -1  
$EndComp
NoConn ~ 5850 5850
NoConn ~ 5850 5300
Wire Wire Line
	7150 5100 7150 5950
Wire Wire Line
	6050 5150 6050 5250
Connection ~ 8850 3700
Wire Wire Line
	9000 3900 8850 3900
Connection ~ 9000 3900
Wire Wire Line
	6400 3800 6700 3800
NoConn ~ 2350 5100
NoConn ~ 1950 5100
$Comp
L Encoder:82402305 U5
U 1 1 5F2B218E
P 2150 4550
F 0 "U5" H 2478 4496 50  0000 L CNN
F 1 "82402305" H 2478 4405 50  0000 L CNN
F 2 "Encoder:Sot-23(6)" H 2150 4050 50  0001 C CNN
F 3 "" H 2150 4050 50  0001 C CNN
	1    2150 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2500 2800 2750
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5F2C34E4
P 2550 2750
F 0 "#FLG0101" H 2550 2825 50  0001 C CNN
F 1 "PWR_FLAG" H 2550 2923 50  0000 C CNN
F 2 "" H 2550 2750 50  0001 C CNN
F 3 "~" H 2550 2750 50  0001 C CNN
	1    2550 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2750 2800 2750
$Comp
L Encoder:TP4056 IC1
U 1 1 5F2D92FF
P 5000 3500
F 0 "IC1" H 5700 3765 50  0000 C CNN
F 1 "TP4056" H 5700 3674 50  0000 C CNN
F 2 "Encoder:Sop(8)" H 6250 3600 50  0001 L CNN
F 3 "https://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/TP4056.pdf" H 6250 3500 50  0001 L CNN
F 4 "1A Standalone Linear Li-lon Battery Charger, SOP-8" H 6250 3400 50  0001 L CNN "Description"
F 5 "1.75" H 6250 3300 50  0001 L CNN "Height"
F 6 "NanJing Top Power" H 6250 3200 50  0001 L CNN "Manufacturer_Name"
F 7 "TP4056" H 6250 3100 50  0001 L CNN "Manufacturer_Part_Number"
	1    5000 3500
	1    0    0    -1  
$EndComp
Connection ~ 6500 5600
Wire Wire Line
	6500 5600 6750 5600
Wire Wire Line
	8500 3700 8650 3700
Wire Wire Line
	8650 3700 8850 3700
Connection ~ 8650 3700
$Comp
L power:PWR_FLAG #FLG0104
U 1 1 5F29970B
P 8650 3700
F 0 "#FLG0104" H 8650 3775 50  0001 C CNN
F 1 "PWR_FLAG" H 8650 3873 50  0000 C CNN
F 2 "" H 8650 3700 50  0001 C CNN
F 3 "~" H 8650 3700 50  0001 C CNN
	1    8650 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7150 5100 7400 5100
$Comp
L Encoder:DW01+G IC2
U 1 1 5F2EE54D
P 5600 4600
F 0 "IC2" H 6100 4865 50  0000 C CNN
F 1 "DW01+G" H 6100 4774 50  0000 C CNN
F 2 "Encoder:Sot-23(6)" H 6450 4700 50  0001 L CNN
F 3 "https://datasheet.lcsc.com/szlcsc/Fortune-Semicon-DW01-G_C14213.pdf" H 6450 4600 50  0001 L CNN
F 4 "One Cell Lithium-ion/Polymer Battery Protection IC" H 6450 4500 50  0001 L CNN "Description"
F 5 "1.45" H 6450 4400 50  0001 L CNN "Height"
F 6 "fortune" H 6450 4300 50  0001 L CNN "Manufacturer_Name"
F 7 "DW01+G" H 6450 4200 50  0001 L CNN "Manufacturer_Part_Number"
	1    5600 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 5600 6500 5600
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 5F2F6061
P 7000 4700
F 0 "#FLG0103" H 7000 4775 50  0001 C CNN
F 1 "PWR_FLAG" H 7000 4873 50  0000 C CNN
F 2 "" H 7000 4700 50  0001 C CNN
F 3 "~" H 7000 4700 50  0001 C CNN
	1    7000 4700
	0    1    -1   0   
$EndComp
Connection ~ 7000 4700
Text GLabel 6900 3800 1    50   Input ~ 0
Batt+
Wire Wire Line
	4450 2750 6400 2750
Text GLabel 5000 2750 1    50   Input ~ 0
VCC
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 5F03A757
P 8650 5050
F 0 "J5" H 8622 4932 50  0000 R CNN
F 1 "Batt_Conn" H 8622 5023 50  0000 R CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 8650 5050 50  0001 C CNN
F 3 "~" H 8650 5050 50  0001 C CNN
	1    8650 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	6700 3800 7300 3800
Text GLabel 7400 5100 2    50   Input ~ 0
Batt-
Text GLabel 8450 5050 0    50   Input ~ 0
Batt-
Text GLabel 8450 4950 0    50   Input ~ 0
Batt+
$Comp
L Encoder:DMosfet Q1
U 1 1 5F0365A7
P 5850 5600
F 0 "Q1" H 6344 5696 50  0000 L CNN
F 1 "DMosfet" H 6344 5605 50  0000 L CNN
F 2 "Encoder:Sot-23(6)" V 5750 5500 50  0001 C CNN
F 3 "~" V 5650 5700 50  0001 C CNN
	1    5850 5600
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0106
U 1 1 5F08AA9A
P 6900 5100
F 0 "#FLG0106" H 6900 5175 50  0001 C CNN
F 1 "PWR_FLAG" H 6900 5273 50  0000 C CNN
F 2 "" H 6900 5100 50  0001 C CNN
F 3 "~" H 6900 5100 50  0001 C CNN
	1    6900 5100
	-1   0    0    -1  
$EndComp
Connection ~ 6900 5100
Wire Wire Line
	6900 5100 7150 5100
Wire Wire Line
	8000 3600 8100 3600
Wire Wire Line
	7800 3800 8100 3800
$Comp
L Encoder:EG1218 S1
U 1 1 5F1D7B37
P 8300 3700
F 0 "S1" H 8350 3500 50  0000 C CNN
F 1 "EG1218_Conn" H 8400 4000 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B3B-EH-A_1x03_P2.50mm_Vertical" H 8500 3900 50  0001 L CNN
F 3 "http://spec_sheets.e-switch.com/specs/P040040.pdf" H 8500 4000 60  0001 L CNN
F 4 "EG1903-ND" H 8500 4100 60  0001 L CNN "Digi-Key_PN"
F 5 "EG1218" H 8500 4200 60  0001 L CNN "MPN"
F 6 "Switches" H 8500 4300 60  0001 L CNN "Category"
F 7 "Slide Switches" H 8500 4400 60  0001 L CNN "Family"
F 8 "http://spec_sheets.e-switch.com/specs/P040040.pdf" H 8500 4500 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/e-switch/EG1218/EG1903-ND/101726" H 8500 4600 60  0001 L CNN "DK_Detail_Page"
F 10 "SWITCH SLIDE SPDT 200MA 30V" H 8500 4700 60  0001 L CNN "Description"
F 11 "E-Switch" H 8500 4800 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8500 4900 60  0001 L CNN "Status"
	1    8300 3700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5FF02BF1
P 5700 4300
F 0 "#PWR0101" H 5700 4050 50  0001 C CNN
F 1 "GND" H 5700 4150 50  0000 C CNN
F 2 "" H 5700 4300 50  0001 C CNN
F 3 "" H 5700 4300 50  0001 C CNN
	1    5700 4300
	1    0    0    -1  
$EndComp
$Comp
L Encoder:TYPE-C-31-M-12 J3
U 1 1 600B3E9D
P 1450 3100
F 0 "J3" H 1450 3900 50  0000 C CNN
F 1 "TYPE-C-31-M-12" H 1450 3800 50  0000 C CNN
F 2 "Encoder:HRO_TYPE-C-31-M-12" H 1200 2100 50  0001 L BNN
F 3 "" H 1450 3150 50  0001 L BNN
	1    1450 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2750 3300 2750
Connection ~ 4100 2750
$Comp
L Device:R R14
U 1 1 60100186
P 2800 3250
F 0 "R14" H 2870 3296 50  0000 L CNN
F 1 "5.1k" H 2870 3205 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2730 3250 50  0001 C CNN
F 3 "~" H 2800 3250 50  0001 C CNN
	1    2800 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601006C0
P 2800 3400
AR Path="/601006C0" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/601006C0" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 2800 3150 50  0001 C CNN
F 1 "GND" H 2805 3227 50  0000 C CNN
F 2 "" H 2800 3400 50  0001 C CNN
F 3 "" H 2800 3400 50  0001 C CNN
	1    2800 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 6010B29F
P 2450 3600
F 0 "R13" H 2520 3646 50  0000 L CNN
F 1 "5.1K" H 2520 3555 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2380 3600 50  0001 C CNN
F 3 "~" H 2450 3600 50  0001 C CNN
	1    2450 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6010B5D4
P 2450 3750
AR Path="/6010B5D4" Ref="#PWR?"  Part="1" 
AR Path="/5F11DB77/6010B5D4" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 2450 3500 50  0001 C CNN
F 1 "GND" H 2455 3577 50  0000 C CNN
F 2 "" H 2450 3750 50  0001 C CNN
F 3 "" H 2450 3750 50  0001 C CNN
	1    2450 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3450 2450 3450
Wire Wire Line
	1950 2850 2800 2850
Wire Wire Line
	2800 2850 2800 3100
Wire Wire Line
	1950 2650 2150 2650
Wire Wire Line
	2150 2650 2150 2750
Wire Wire Line
	2150 2750 2550 2750
Connection ~ 2550 2750
Wire Wire Line
	1950 3550 2150 3550
Wire Wire Line
	2150 3550 2150 2750
Connection ~ 2150 2750
Wire Wire Line
	1950 3650 2300 3650
Wire Wire Line
	2300 3650 2300 3000
Wire Wire Line
	2300 3000 2500 3000
Wire Wire Line
	2300 3000 2300 2550
Wire Wire Line
	2300 2550 1950 2550
Connection ~ 2300 3000
Wire Wire Line
	1950 3750 2300 3750
Wire Wire Line
	2300 3750 2300 3650
Connection ~ 1950 3750
Connection ~ 2300 3650
NoConn ~ 1950 2750
NoConn ~ 1950 2950
NoConn ~ 1950 3050
NoConn ~ 1950 3150
NoConn ~ 1950 3250
NoConn ~ 1950 3350
Wire Wire Line
	2150 3550 2150 4200
Connection ~ 2150 3550
$EndSCHEMATC
