EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 2
Title "Encoder MCU"
Date "2020-06-30"
Rev "v1"
Comp "S.D.T."
Comment1 "Author:Paul Max Avalos Aguilar"
Comment2 "SingleBoardEncoder2"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R2
U 1 1 5C285A71
P 1050 5000
F 0 "R2" H 1100 5050 50  0000 L CNN
F 1 "560" H 1100 4950 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 980 5000 50  0001 C CNN
F 3 "~" H 1050 5000 50  0001 C CNN
	1    1050 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5C2863B3
P 1050 4050
F 0 "#PWR0110" H 1050 3800 50  0001 C CNN
F 1 "GND" H 1055 3877 50  0000 C CNN
F 2 "" H 1050 4050 50  0001 C CNN
F 3 "" H 1050 4050 50  0001 C CNN
	1    1050 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5C2895D3
P 2100 3450
F 0 "#PWR0112" H 2100 3200 50  0001 C CNN
F 1 "GND" H 2105 3277 50  0000 C CNN
F 2 "" H 2100 3450 50  0001 C CNN
F 3 "" H 2100 3450 50  0001 C CNN
	1    2100 3450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5C289611
P 2100 4550
F 0 "#PWR0113" H 2100 4300 50  0001 C CNN
F 1 "GND" H 2105 4377 50  0000 C CNN
F 2 "" H 2100 4550 50  0001 C CNN
F 3 "" H 2100 4550 50  0001 C CNN
	1    2100 4550
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 3450 2100 3450
Wire Wire Line
	2100 4550 1750 4550
$Comp
L Device:R R4
U 1 1 5C28C76B
P 2250 3400
F 0 "R4" H 2320 3446 50  0000 L CNN
F 1 "1.2K" H 2320 3355 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2180 3400 50  0001 C CNN
F 3 "~" H 2250 3400 50  0001 C CNN
	1    2250 3400
	1    0    0    -1  
$EndComp
Text Label 2350 3650 0    50   ~ 0
SIGB
Wire Wire Line
	1750 3650 2250 3650
Wire Wire Line
	2250 3650 2550 3650
Connection ~ 2250 3650
Text Label 2350 4750 0    50   ~ 0
SIGA
$Comp
L Encoder:Q_PMOS_GSD Q1
U 1 1 5D553268
P 1300 2750
F 0 "Q1" H 1400 2550 50  0000 C CNN
F 1 "Q_PMOS_GSD" H 1400 2450 50  0000 C CNN
F 2 "Encoder:SOT-23" V 1500 2600 50  0001 C CNN
F 3 "~" V 1300 2800 50  0001 C CNN
	1    1300 2750
	1    0    0    -1  
$EndComp
Text Label 1700 2500 2    50   ~ 0
EncEnable
Wire Wire Line
	1700 2500 1300 2500
$Comp
L power:+3.3V #PWR017
U 1 1 5D59FF5B
P 1500 2800
F 0 "#PWR017" H 1500 2650 50  0001 C CNN
F 1 "+3.3V" H 1515 2973 50  0000 C CNN
F 2 "" H 1500 2800 50  0001 C CNN
F 3 "" H 1500 2800 50  0001 C CNN
	1    1500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4450 2600 4450
Wire Wire Line
	2600 4450 2600 3150
Wire Wire Line
	900  4550 900  3450
Wire Wire Line
	900  2800 1100 2800
Wire Wire Line
	900  4550 1150 4550
Connection ~ 900  3150
Wire Wire Line
	900  3150 900  2800
Wire Wire Line
	900  3450 1150 3450
Connection ~ 900  3450
Wire Wire Line
	900  3450 900  3150
$Comp
L Device:C C7
U 1 1 5EFE06DB
P 4550 2700
F 0 "C7" H 4665 2746 50  0000 L CNN
F 1 "4.7uF" H 4665 2655 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4588 2550 50  0001 C CNN
F 3 "~" H 4550 2700 50  0001 C CNN
	1    4550 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 5EFE29C0
P 4550 2300
F 0 "#PWR0104" H 4550 2150 50  0001 C CNN
F 1 "+3.3V" H 4565 2473 50  0000 C CNN
F 2 "" H 4550 2300 50  0001 C CNN
F 3 "" H 4550 2300 50  0001 C CNN
	1    4550 2300
	1    0    0    -1  
$EndComp
Text Label 4550 2300 3    50   ~ 0
Supply
$Comp
L Device:C C8
U 1 1 5EFA1FD1
P 5100 4050
F 0 "C8" V 5050 3900 50  0000 C CNN
F 1 "100nF" V 5050 3700 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5138 3900 50  0001 C CNN
F 3 "~" H 5100 4050 50  0001 C CNN
	1    5100 4050
	0    1    1    0   
$EndComp
Connection ~ 3850 3900
Wire Wire Line
	3850 3900 3850 4050
Wire Wire Line
	4950 3750 4950 3600
Wire Wire Line
	4350 3600 4950 3600
Wire Wire Line
	4350 3600 4300 3600
Wire Wire Line
	4300 3900 4350 3900
Wire Wire Line
	4000 3600 3850 3600
Wire Wire Line
	3850 3900 3850 3600
Wire Wire Line
	4000 3900 3850 3900
$Comp
L Device:C C3
U 1 1 5EFB0E20
P 4150 3600
F 0 "C3" V 4350 3600 50  0000 C CNN
F 1 "12pF" V 4450 3600 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4188 3450 50  0001 C CNN
F 3 "~" H 4150 3600 50  0001 C CNN
	1    4150 3600
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C5
U 1 1 5EFB0E1A
P 4150 3900
F 0 "C5" V 3898 3900 50  0000 C CNN
F 1 "12pF" V 3989 3900 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4188 3750 50  0001 C CNN
F 3 "~" H 4150 3900 50  0001 C CNN
	1    4150 3900
	0    -1   -1   0   
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5EFB0E14
P 4350 3750
F 0 "Y1" V 4350 3500 50  0000 L CNN
F 1 "8Mhz" V 4450 3450 50  0000 L CNN
F 2 "Encoder:Crystal_HC49-U_Vertical" H 4350 3750 50  0001 C CNN
F 3 "~" H 4350 3750 50  0001 C CNN
	1    4350 3750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5EFD5306
P 3850 4050
F 0 "#PWR0119" H 3850 3800 50  0001 C CNN
F 1 "GND" H 3855 3877 50  0000 C CNN
F 2 "" H 3850 4050 50  0001 C CNN
F 3 "" H 3850 4050 50  0001 C CNN
	1    3850 4050
	1    0    0    -1  
$EndComp
Connection ~ 4350 3600
Connection ~ 4350 3900
$Comp
L power:GND #PWR0120
U 1 1 5F070E6F
P 5850 2400
F 0 "#PWR0120" H 5850 2150 50  0001 C CNN
F 1 "GND" H 5855 2227 50  0000 C CNN
F 2 "" H 5850 2400 50  0001 C CNN
F 3 "" H 5850 2400 50  0001 C CNN
	1    5850 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 5F070E68
P 5850 2550
F 0 "R7" V 5950 2500 50  0000 L CNN
F 1 "10K" V 6050 2500 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5780 2550 50  0001 C CNN
F 3 "~" H 5850 2550 50  0001 C CNN
	1    5850 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5850 3150 5850 2700
Text Label 6000 3050 0    0    ~ 0
LPTIM1_IN2
Text Label 6000 3050 1    0    ~ 0
LPTIM1_IN2
Text Label 6000 3100 1    50   ~ 0
LPTIM1_IN2
Text Label 6300 1600 1    50   ~ 0
SIGB
$Comp
L power:GND #PWR0127
U 1 1 5EFA2AF5
P 4550 4050
F 0 "#PWR0127" H 4550 3800 50  0001 C CNN
F 1 "GND" H 4555 3877 50  0000 C CNN
F 2 "" H 4550 4050 50  0001 C CNN
F 3 "" H 4550 4050 50  0001 C CNN
	1    4550 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 4050 4950 4050
$Comp
L power:GND #PWR0128
U 1 1 5F196248
P 1050 5150
F 0 "#PWR0128" H 1050 4900 50  0001 C CNN
F 1 "GND" H 1055 4977 50  0000 C CNN
F 2 "" H 1050 5150 50  0001 C CNN
F 3 "" H 1050 5150 50  0001 C CNN
	1    1050 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 4400 8900 4400
Wire Wire Line
	9250 4500 8900 4500
$Comp
L power:GND #PWR01
U 1 1 5F235EDB
P 8700 4300
F 0 "#PWR01" H 8700 4050 50  0001 C CNN
F 1 "GND" H 8650 4150 50  0000 C CNN
F 2 "" H 8700 4300 50  0001 C CNN
F 3 "" H 8700 4300 50  0001 C CNN
	1    8700 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 4300 9250 4300
Text Label 9250 3700 2    50   ~ 0
SWDIO
Wire Wire Line
	9250 3700 8900 3700
Wire Wire Line
	9250 3600 8900 3600
Text Label 9250 3600 2    50   ~ 0
SWCLK
Text Label 6000 1600 1    50   ~ 0
SIGA
Wire Wire Line
	6450 3150 6450 2350
Text Label 6450 2700 3    50   ~ 0
EncEnable
Text GLabel 6600 6200 2    50   Input ~ 0
Batt+
Text GLabel 7950 4050 2    50   Input ~ 0
VCC
$Comp
L Device:R R8
U 1 1 5F0F6150
P 6450 6200
F 0 "R8" H 6520 6246 50  0000 L CNN
F 1 "10K" H 6520 6155 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6380 6200 50  0001 C CNN
F 3 "~" H 6450 6200 50  0001 C CNN
	1    6450 6200
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5F0F6BEE
P 6150 6200
F 0 "R9" H 6220 6246 50  0000 L CNN
F 1 "10K" H 6220 6155 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6080 6200 50  0001 C CNN
F 3 "~" H 6150 6200 50  0001 C CNN
	1    6150 6200
	0    1    1    0   
$EndComp
Connection ~ 6300 6200
$Comp
L power:GND #PWR0105
U 1 1 5F0FACD0
P 6000 6200
F 0 "#PWR0105" H 6000 5950 50  0001 C CNN
F 1 "GND" H 6005 6027 50  0000 C CNN
F 2 "" H 6000 6200 50  0001 C CNN
F 3 "" H 6000 6200 50  0001 C CNN
	1    6000 6200
	0    1    1    0   
$EndComp
Text Notes 7950 4050 2    50   ~ 0
*Config as digital\ninput always!!\n
$Comp
L power:GND #PWR0106
U 1 1 5F0AE8F0
P 4550 2850
F 0 "#PWR0106" H 4550 2600 50  0001 C CNN
F 1 "GND" H 4555 2677 50  0000 C CNN
F 2 "" H 4550 2850 50  0001 C CNN
F 3 "" H 4550 2850 50  0001 C CNN
	1    4550 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 2300 4550 2550
$Comp
L power:GND #PWR0111
U 1 1 5F0A6625
P 8700 3500
F 0 "#PWR0111" H 8700 3250 50  0001 C CNN
F 1 "GND" H 8650 3350 50  0000 C CNN
F 2 "" H 8700 3500 50  0001 C CNN
F 3 "" H 8700 3500 50  0001 C CNN
	1    8700 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	8700 3500 9250 3500
$Comp
L Encoder:OPTO_Conn Q4
U 1 1 5F1557B4
P 1450 4650
F 0 "Q4" H 1450 4950 50  0000 C CNN
F 1 "OPTO_Conn" H 1450 4850 50  0000 C CNN
F 2 "" H 1450 4650 50  0001 C CNN
F 3 "" H 1450 4650 50  0001 C CNN
	1    1450 4650
	1    0    0    -1  
$EndComp
$Comp
L Encoder:OPTO_Conn Q3
U 1 1 5F15DEA7
P 1450 3550
F 0 "Q3" H 1450 3875 50  0000 C CNN
F 1 "OPTO_Conn" H 1450 3750 50  0000 C CNN
F 2 "" H 1450 3550 50  0001 C CNN
F 3 "" H 1450 3550 50  0001 C CNN
	1    1450 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 3750 1050 3650
Wire Wire Line
	1050 3650 1150 3650
Wire Wire Line
	1050 4850 1050 4750
Wire Wire Line
	1050 4750 1150 4750
$Comp
L Encoder:STM32G431KBT6 U1
U 1 1 5FFFCFFF
P 6250 4100
F 0 "U1" H 6200 4100 50  0000 L CNN
F 1 "STM32G431KBT6" H 5950 3950 50  0000 L CNN
F 2 "Package_QFP:LQFP-32_7x7mm_P0.8mm" H 5900 4400 50  0001 C CNN
F 3 "" H 5900 4400 50  0001 C CNN
	1    6250 4100
	1    0    0    -1  
$EndComp
Text Label 6000 5200 3    50   ~ 0
Sens_SPIMISO
Text Label 5850 5200 3    50   ~ 0
Sens_SPISCK
Wire Wire Line
	5850 5150 5850 5700
Wire Wire Line
	6000 5150 6000 5700
Text Label 7250 4350 0    50   ~ 0
I2C2SCL
Text Label 7250 4500 0    50   ~ 0
I2C2SDA
Wire Wire Line
	7200 4350 7850 4350
Wire Wire Line
	7200 4500 7850 4500
$Comp
L Device:C C6
U 1 1 6001D7FF
P 7450 4950
F 0 "C6" H 7565 4996 50  0000 L CNN
F 1 "100nF" H 7565 4905 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7488 4800 50  0001 C CNN
F 3 "~" H 7450 4950 50  0001 C CNN
	1    7450 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 4800 7450 4750
$Comp
L power:GND #PWR0102
U 1 1 6001D806
P 7450 5100
F 0 "#PWR0102" H 7450 4850 50  0001 C CNN
F 1 "GND" H 7455 4927 50  0000 C CNN
F 2 "" H 7450 5100 50  0001 C CNN
F 3 "" H 7450 5100 50  0001 C CNN
	1    7450 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 4750 8150 4750
Connection ~ 7450 4750
Wire Wire Line
	7450 4750 7450 4650
Text Label 7800 4750 0    50   ~ 0
Supply
Wire Wire Line
	7450 4650 7200 4650
$Comp
L power:GND #PWR0103
U 1 1 60023E61
P 6750 5150
F 0 "#PWR0103" H 6750 4900 50  0001 C CNN
F 1 "GND" H 6755 4977 50  0000 C CNN
F 2 "" H 6750 5150 50  0001 C CNN
F 3 "" H 6750 5150 50  0001 C CNN
	1    6750 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3600 7700 3600
Wire Wire Line
	7700 3750 7200 3750
Text Label 7600 3750 2    50   ~ 0
SWDIO
Text Label 7600 3600 2    50   ~ 0
SWCLK
$Comp
L Device:C C9
U 1 1 600372BF
P 6600 5650
F 0 "C9" H 6450 5750 50  0000 L CNN
F 1 "10nF" H 6400 5550 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6638 5500 50  0001 C CNN
F 3 "~" H 6600 5650 50  0001 C CNN
	1    6600 5650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0107
U 1 1 600372C5
P 6900 5500
F 0 "#PWR0107" H 6900 5350 50  0001 C CNN
F 1 "+3.3V" H 6915 5628 50  0000 L CNN
F 2 "" H 6900 5500 50  0001 C CNN
F 3 "" H 6900 5500 50  0001 C CNN
	1    6900 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 600372CB
P 6600 5800
F 0 "#PWR0108" H 6600 5550 50  0001 C CNN
F 1 "GND" H 6605 5627 50  0000 C CNN
F 2 "" H 6600 5800 50  0001 C CNN
F 3 "" H 6600 5800 50  0001 C CNN
	1    6600 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 600372D1
P 6900 5800
F 0 "#PWR0109" H 6900 5550 50  0001 C CNN
F 1 "GND" H 6905 5627 50  0000 C CNN
F 2 "" H 6900 5800 50  0001 C CNN
F 3 "" H 6900 5800 50  0001 C CNN
	1    6900 5800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 600372DA
P 6900 5650
F 0 "C10" H 6700 5750 50  0000 L CNN
F 1 "1uF" H 6700 5550 50  0000 L CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6938 5500 50  0001 C CNN
F 3 "~" H 6900 5650 50  0001 C CNN
	1    6900 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 6003AFC8
P 6450 5150
F 0 "#PWR0114" H 6450 4900 50  0001 C CNN
F 1 "GND" H 6455 4977 50  0000 C CNN
F 2 "" H 6450 5150 50  0001 C CNN
F 3 "" H 6450 5150 50  0001 C CNN
	1    6450 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5500 6600 5500
Connection ~ 6900 5500
Wire Wire Line
	6600 5500 6600 5150
Connection ~ 6600 5500
$Comp
L power:GND #PWR0115
U 1 1 6004F229
P 4850 3300
F 0 "#PWR0115" H 4850 3050 50  0001 C CNN
F 1 "GND" H 4855 3127 50  0000 C CNN
F 2 "" H 4850 3300 50  0001 C CNN
F 3 "" H 4850 3300 50  0001 C CNN
	1    4850 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 6004F22F
P 5000 3300
F 0 "C4" V 4748 3300 50  0000 C CNN
F 1 "100nF" V 4839 3300 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5038 3150 50  0001 C CNN
F 3 "~" H 5000 3300 50  0001 C CNN
	1    5000 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 2850 5250 3300
Wire Wire Line
	5250 3300 5150 3300
Connection ~ 5250 3300
Text Label 5250 2950 3    50   ~ 0
Supply
Wire Wire Line
	5250 3300 5250 3600
Wire Wire Line
	5250 4500 4400 4500
Text Label 4800 4500 0    50   ~ 0
USART1TX
Text Label 5200 4650 2    50   ~ 0
USART1RX
Text Label 4400 4650 0    50   ~ 0
BLETX
Text Label 4400 4500 0    50   ~ 0
BLERX
Wire Wire Line
	5250 4650 4400 4650
Wire Wire Line
	4950 3750 5250 3750
Wire Wire Line
	4350 3900 5250 3900
$Comp
L power:GND #PWR0116
U 1 1 60087A8E
P 5700 3150
F 0 "#PWR0116" H 5700 2900 50  0001 C CNN
F 1 "GND" H 5705 2977 50  0000 C CNN
F 2 "" H 5700 3150 50  0001 C CNN
F 3 "" H 5700 3150 50  0001 C CNN
	1    5700 3150
	-1   0    0    1   
$EndComp
Text Label 6600 3100 1    50   ~ 0
TIM2_CH2
Text Label 6750 3100 1    50   ~ 0
TIM2_CH1
Wire Wire Line
	6750 3050 6750 2650
Wire Wire Line
	6750 3150 6750 3050
Connection ~ 6750 3050
Wire Wire Line
	6000 3150 6000 3050
Connection ~ 6000 3050
Wire Wire Line
	6000 3050 6750 3050
Wire Wire Line
	6600 2950 6600 2650
Wire Wire Line
	6600 3150 6600 2950
Connection ~ 6600 2950
Wire Wire Line
	6300 3150 6300 2950
Connection ~ 6300 2950
Wire Wire Line
	6300 2950 6600 2950
Wire Wire Line
	7200 4050 7950 4050
NoConn ~ 7200 3900
Text Label 8900 4500 0    50   ~ 0
I2C2SCL
Text Label 8900 4400 0    50   ~ 0
I2C2SDA
NoConn ~ 7200 4200
$Comp
L Connector:Conn_01x03_Male J2
U 1 1 5FF5A42B
P 9450 3600
F 0 "J2" H 9650 3200 50  0000 R CNN
F 1 "Conn_01x03_Male" H 9800 3350 50  0000 R CNN
F 2 "Encoder:PinHeader_1x03_P2.54mm_Vertical" H 9450 3600 50  0001 C CNN
F 3 "~" H 9450 3600 50  0001 C CNN
	1    9450 3600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5FF6974D
P 9450 4400
F 0 "J3" H 9500 4700 50  0000 R CNN
F 1 "Conn_01x04_Male" H 9800 4600 50  0000 R CNN
F 2 "Encoder:JST_EH_B4B-EH-A_1x04_P2.50mm_Vertical" H 9450 4400 50  0001 C CNN
F 3 "~" H 9450 4400 50  0001 C CNN
	1    9450 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	9250 4200 8900 4200
$Comp
L power:+3.3V #PWR012
U 1 1 5FF9AA73
P 8900 4200
F 0 "#PWR012" H 8900 4050 50  0001 C CNN
F 1 "+3.3V" H 8915 4373 50  0000 C CNN
F 2 "" H 8900 4200 50  0001 C CNN
F 3 "" H 8900 4200 50  0001 C CNN
	1    8900 4200
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 5FF9F077
P 5400 5750
F 0 "#PWR011" H 5400 5600 50  0001 C CNN
F 1 "+3.3V" H 5415 5923 50  0000 C CNN
F 2 "" H 5400 5750 50  0001 C CNN
F 3 "" H 5400 5750 50  0001 C CNN
	1    5400 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5750 4800 5750
$Comp
L power:GND #PWR02
U 1 1 5FFA70BB
P 5400 5850
F 0 "#PWR02" H 5400 5600 50  0001 C CNN
F 1 "GND" H 5350 5700 50  0000 C CNN
F 2 "" H 5400 5850 50  0001 C CNN
F 3 "" H 5400 5850 50  0001 C CNN
	1    5400 5850
	0    -1   -1   0   
$EndComp
Text Label 5400 5950 2    50   ~ 0
Sens_SPINSS
Text Label 5400 6050 2    50   ~ 0
Sens_SPISCK
Text Label 5400 6150 2    50   ~ 0
Sens_SPIMISO
Text Label 5400 6250 2    50   ~ 0
Sens_SPIMOSI
Wire Wire Line
	4800 5950 5400 5950
Wire Wire Line
	4800 6050 5400 6050
Wire Wire Line
	4800 6150 5400 6150
Wire Wire Line
	4800 6250 5400 6250
$Comp
L Connector:Conn_01x06_Male J6
U 1 1 5FFDB180
P 4600 5950
F 0 "J6" H 4572 5832 50  0000 R CNN
F 1 "Conn_01x06_Male" H 4572 5923 50  0000 R CNN
F 2 "Encoder:JST_EH_B6B-EH-A_1x06_P2.50mm_Vertical" H 4600 5950 50  0001 C CNN
F 3 "~" H 4600 5950 50  0001 C CNN
	1    4600 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5850 4800 5850
$Comp
L Connector:Conn_01x08_Male J1
U 1 1 5FFB5CBE
P 2950 3500
F 0 "J1" H 3050 4000 50  0000 C CNN
F 1 "Conn_01x08_Male" H 3050 3900 50  0000 C CNN
F 2 "Encoder:JST_EH_B8B-EH-A_1x08_P2.50mm_Vertical" H 2950 3500 50  0001 C CNN
F 3 "~" H 2950 3500 50  0001 C CNN
	1    2950 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 3800 3450 3800
Wire Wire Line
	3150 3900 3450 3900
Wire Wire Line
	3150 3700 3450 3700
Wire Wire Line
	3150 3600 3450 3600
Wire Wire Line
	3150 3500 3450 3500
Wire Wire Line
	3150 3400 3450 3400
Wire Wire Line
	3150 3300 3450 3300
Wire Wire Line
	3150 3200 3450 3200
Text Label 950  4550 0    50   ~ 0
E+(a)
Text Label 1150 4750 2    50   ~ 0
E-(a)
Text Label 2050 4550 2    50   ~ 0
D-(a)
Wire Wire Line
	2250 3650 2250 3550
Wire Wire Line
	900  3150 2250 3150
Wire Wire Line
	2250 3250 2250 3150
Connection ~ 2250 3150
Wire Wire Line
	2250 3150 2600 3150
Text Label 2050 3450 2    50   ~ 0
D-(b)
Text Label 2050 3650 2    50   ~ 0
D+(b)
Wire Wire Line
	2250 4750 2550 4750
Text Label 950  3450 0    50   ~ 0
E+(b)
Text Label 1150 3650 2    50   ~ 0
E-(b)
Text Label 3200 3500 0    50   ~ 0
E+(a)
Text Label 3200 3400 0    50   ~ 0
E-(a)
Text Label 3200 3200 0    50   ~ 0
D+(a)
Text Label 3200 3300 0    50   ~ 0
D-(a)
Text Label 3200 3600 0    50   ~ 0
D+(b)
Text Label 3200 3700 0    50   ~ 0
D-(b)
Text Label 3200 3900 0    50   ~ 0
E+(b)
Text Label 3200 3800 0    50   ~ 0
E-(b)
NoConn ~ 5250 4350
NoConn ~ 5250 4200
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 6070198F
P 3300 4550
F 0 "J4" H 3408 4831 50  0000 C CNN
F 1 "Conn_01x04_Male" H 3408 4740 50  0000 C CNN
F 2 "Encoder:JST_EH_B4B-EH-A_1x04_P2.50mm_Vertical" H 3300 4550 50  0001 C CNN
F 3 "~" H 3300 4550 50  0001 C CNN
	1    3300 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4550 3500 4550
Wire Wire Line
	3800 4650 3500 4650
Text Label 3550 4550 0    50   ~ 0
BLERX
Text Label 3550 4650 0    50   ~ 0
BLETX
$Comp
L power:+3.3V #PWR015
U 1 1 60708F52
P 3800 4450
F 0 "#PWR015" H 3800 4300 50  0001 C CNN
F 1 "+3.3V" H 3815 4623 50  0000 C CNN
F 2 "" H 3800 4450 50  0001 C CNN
F 3 "" H 3800 4450 50  0001 C CNN
	1    3800 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 4450 3500 4450
$Comp
L power:GND #PWR016
U 1 1 6070EBFE
P 4050 4750
F 0 "#PWR016" H 4050 4500 50  0001 C CNN
F 1 "GND" H 4000 4600 50  0000 C CNN
F 2 "" H 4050 4750 50  0001 C CNN
F 3 "" H 4050 4750 50  0001 C CNN
	1    4050 4750
	-1   0    0    1   
$EndComp
Wire Wire Line
	4050 4750 3500 4750
$Comp
L Device:R R1
U 1 1 5C293966
P 2250 4600
F 0 "R1" H 2320 4646 50  0000 L CNN
F 1 "1.2K" H 2320 4555 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2180 4600 50  0001 C CNN
F 3 "~" H 2250 4600 50  0001 C CNN
	1    2250 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5C285A10
P 1050 3900
F 0 "R6" H 1120 3946 50  0000 L CNN
F 1 "560" H 1120 3855 50  0000 L CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 980 3900 50  0001 C CNN
F 3 "~" H 1050 3900 50  0001 C CNN
	1    1050 3900
	1    0    0    -1  
$EndComp
NoConn ~ 6150 3150
Wire Wire Line
	6150 5150 6150 5700
Wire Wire Line
	5700 5150 5700 5700
Text Label 6150 5200 3    50   ~ 0
Sens_SPIMOSI
Text Label 5700 5200 3    50   ~ 0
Sens_SPINSS
Wire Wire Line
	6300 5150 6300 6200
$Sheet
S 8200 1250 700  350 
U 5F11DB77
F0 "Power" 50
F1 "Power.sch" 50
$EndSheet
$Comp
L Device:R R3
U 1 1 61E0DFAC
P 6000 1800
F 0 "R3" H 5930 1754 50  0000 R CNN
F 1 "R" H 5930 1845 50  0000 R CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5930 1800 50  0001 C CNN
F 3 "~" H 6000 1800 50  0001 C CNN
	1    6000 1800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R5
U 1 1 61E0E9A4
P 6300 1800
F 0 "R5" H 6250 1750 50  0000 R CNN
F 1 "R" H 6250 1850 50  0000 R CNN
F 2 "Encoder:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6230 1800 50  0001 C CNN
F 3 "~" H 6300 1800 50  0001 C CNN
	1    6300 1800
	-1   0    0    1   
$EndComp
$Comp
L Device:C C1
U 1 1 61E0EB93
P 5850 1950
F 0 "C1" V 6000 1950 50  0000 C CNN
F 1 "C" V 5850 1950 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5888 1800 50  0001 C CNN
F 3 "~" H 5850 1950 50  0001 C CNN
	1    5850 1950
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 61E0F967
P 6450 1950
F 0 "C2" V 6600 1950 50  0000 C CNN
F 1 "C" V 6450 1950 50  0000 C CNN
F 2 "Encoder:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6488 1800 50  0001 C CNN
F 3 "~" H 6450 1950 50  0001 C CNN
	1    6450 1950
	0    1    1    0   
$EndComp
Connection ~ 2250 4750
Text Label 2050 4750 2    50   ~ 0
D+(a)
Wire Wire Line
	1750 4750 2250 4750
$Comp
L power:GND #PWR0121
U 1 1 61E42979
P 6600 1950
F 0 "#PWR0121" H 6600 1700 50  0001 C CNN
F 1 "GND" V 6605 1822 50  0000 R CNN
F 2 "" H 6600 1950 50  0001 C CNN
F 3 "" H 6600 1950 50  0001 C CNN
	1    6600 1950
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 61E43969
P 5700 1950
F 0 "#PWR0122" H 5700 1700 50  0001 C CNN
F 1 "GND" V 5705 1822 50  0000 R CNN
F 2 "" H 5700 1950 50  0001 C CNN
F 3 "" H 5700 1950 50  0001 C CNN
	1    5700 1950
	0    1    1    0   
$EndComp
Text Label 900  2800 0    50   ~ 0
power
Text Label 6300 3100 1    50   ~ 0
LPTIM1_IN1
Connection ~ 6000 1950
Wire Wire Line
	6000 1950 6000 3050
Wire Wire Line
	6000 1650 6000 1350
Connection ~ 6300 1950
Wire Wire Line
	6300 1650 6300 1350
Wire Wire Line
	6300 1950 6300 2950
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 61ED1F0E
P 2000 1400
F 0 "H1" H 2100 1449 50  0000 L CNN
F 1 "MountingHole_Pad" H 2100 1358 50  0000 L CNN
F 2 "Encoder:MountingHole_3.2mm_M3_Pad" H 2000 1400 50  0001 C CNN
F 3 "~" H 2000 1400 50  0001 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 61ED244A
P 2000 900
F 0 "H2" H 2100 949 50  0000 L CNN
F 1 "MountingHole_Pad" H 2100 858 50  0000 L CNN
F 2 "Encoder:MountingHole_3.2mm_M3_Pad" H 2000 900 50  0001 C CNN
F 3 "~" H 2000 900 50  0001 C CNN
	1    2000 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 61ED3018
P 2000 1000
F 0 "#PWR0123" H 2000 750 50  0001 C CNN
F 1 "GND" H 2005 827 50  0000 C CNN
F 2 "" H 2000 1000 50  0001 C CNN
F 3 "" H 2000 1000 50  0001 C CNN
	1    2000 1000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 61ED3933
P 2000 1500
F 0 "#PWR0124" H 2000 1250 50  0001 C CNN
F 1 "GND" H 2005 1327 50  0000 C CNN
F 2 "" H 2000 1500 50  0001 C CNN
F 3 "" H 2000 1500 50  0001 C CNN
	1    2000 1500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
