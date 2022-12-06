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
L Encoder:ITR8102 Q3
U 1 1 5C59CA18
P 5900 4500
F 0 "Q3" H 5900 4825 50  0000 C CNN
F 1 "ITR8102" H 5900 4734 50  0000 C CNN
F 2 "Encoder:itr8102" H 5900 4500 50  0001 C CNN
F 3 "" H 5900 4500 50  0001 C CNN
	1    5900 4500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 5C59CA1F
P 5150 4400
F 0 "J1" H 5256 4578 50  0000 C CNN
F 1 "Conn_01x01_Male" H 5256 4487 50  0000 C CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 5150 4400 50  0001 C CNN
F 3 "~" H 5150 4400 50  0001 C CNN
	1    5150 4400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 5C59CA26
P 5150 4600
F 0 "J2" H 5256 4778 50  0000 C CNN
F 1 "Conn_01x01_Male" H 5256 4687 50  0000 C CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 5150 4600 50  0001 C CNN
F 3 "~" H 5150 4600 50  0001 C CNN
	1    5150 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J4
U 1 1 5C59CA2D
P 6700 4600
F 0 "J4" H 6673 4530 50  0000 R CNN
F 1 "Conn_01x01_Male" H 6673 4621 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 6700 4600 50  0001 C CNN
F 3 "~" H 6700 4600 50  0001 C CNN
	1    6700 4600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 5C59CA34
P 6700 4400
F 0 "J3" H 6673 4330 50  0000 R CNN
F 1 "Conn_01x01_Male" H 6673 4421 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 6700 4400 50  0001 C CNN
F 3 "~" H 6700 4400 50  0001 C CNN
	1    6700 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 4400 6200 4400
Wire Wire Line
	6200 4600 6500 4600
Wire Wire Line
	5600 4600 5350 4600
Wire Wire Line
	5350 4400 5600 4400
$Comp
L Encoder:ITR8102 Q4
U 1 1 6081B0B0
P 5900 5250
F 0 "Q4" H 5900 5575 50  0000 C CNN
F 1 "ITR8102" H 5900 5484 50  0000 C CNN
F 2 "Encoder:itr8102" H 5900 5250 50  0001 C CNN
F 3 "" H 5900 5250 50  0001 C CNN
	1    5900 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J5
U 1 1 6081B0B6
P 5150 5150
F 0 "J5" H 5256 5328 50  0000 C CNN
F 1 "Conn_01x01_Male" H 5256 5237 50  0000 C CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 5150 5150 50  0001 C CNN
F 3 "~" H 5150 5150 50  0001 C CNN
	1    5150 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J6
U 1 1 6081B0BC
P 5150 5350
F 0 "J6" H 5256 5528 50  0000 C CNN
F 1 "Conn_01x01_Male" H 5256 5437 50  0000 C CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 5150 5350 50  0001 C CNN
F 3 "~" H 5150 5350 50  0001 C CNN
	1    5150 5350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J8
U 1 1 6081B0C2
P 6700 5350
F 0 "J8" H 6673 5280 50  0000 R CNN
F 1 "Conn_01x01_Male" H 6673 5371 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 6700 5350 50  0001 C CNN
F 3 "~" H 6700 5350 50  0001 C CNN
	1    6700 5350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J7
U 1 1 6081B0C8
P 6700 5150
F 0 "J7" H 6673 5080 50  0000 R CNN
F 1 "Conn_01x01_Male" H 6673 5171 50  0000 R CNN
F 2 "Encoder:PinHeader_1x01_P2.54mm_Vertical_NO3D" H 6700 5150 50  0001 C CNN
F 3 "~" H 6700 5150 50  0001 C CNN
	1    6700 5150
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 5150 6200 5150
Wire Wire Line
	6200 5350 6500 5350
Wire Wire Line
	5600 5350 5350 5350
Wire Wire Line
	5350 5150 5600 5150
$EndSCHEMATC
