Attribute VB_Name = "Module2"

Global Const DRV_ERROR_CODES = 20001
Global Const DRV_SUCCESS = 20002
Global Const DRV_VXDNOTINSTALLED = 20003
Global Const DRV_ERROR_SCAN = 20004
Global Const DRV_ERROR_CHECK_SUM = 20005
Global Const DRV_ERROR_FILELOAD = 20006
Global Const DRV_UNKNOWN_FUNCTION = 20007
Global Const DRV_ERROR_VXD_INIT = 20008
Global Const DRV_ERROR_ADDRESS = 20009
Global Const DRV_ERROR_PAGELOCK = 20010
Global Const DRV_ERROR_PAGEUNLOCK = 20011
Global Const DRV_ERROR_BOARDTEST = 20012
Global Const DRV_ERROR_ACK = 20013
Global Const DRV_ERROR_UP_FIFO = 20014
Global Const DRV_ERROR_PATTERN = 20015

Global Const DRV_ACQUISITION_ERRORS = 20017
Global Const DRV_ACQ_BUFFER = 20018
Global Const DRV_ACQ_DOWNFIFO_FULL = 20019
Global Const DRV_PROC_UNKONWN_INSTRUCTION = 20020
Global Const DRV_ILLEGAL_OP_CODE = 20021
Global Const DRV_KINETIC_TIME_NOT_MET = 20022
Global Const DRV_ACCUM_TIME_NOT_MET = 20023

Global Const DRV_TEMPERATURE_CODES = 20033
Global Const DRV_TEMPERATURE_OFF = 20034
Global Const DRV_TEMPERATURE_STABILIZED = 20036
Global Const DRV_TEMPERATURE_NOT_REACHED = 20037
Global Const DRV_TEMPERATURE_OUT_RANGE = 20038
Global Const DRV_TEMPERATURE_DRIFT = 20040
Global Const DRV_TEMPERATURE_NOT_SUPPORTED = 20039
Global Const DRV_TEMPERATURE_NOT_STABILIZED = 20035

Global Const DRV_GENERAL_ERRORS = 20049
Global Const DRV_INVALID_AUX = 20050
Global Const DRV_COF_NOTLOADED = 20051
Global Const DRV_FPGAPROG = 20052
Global Const DRV_FLEXERROR = 20053

Global Const DRV_DRIVER_ERRORS = 20065
Global Const DRV_P1INVALID = 20066
Global Const DRV_P2INVALID = 20067
Global Const DRV_P3INVALID = 20068
Global Const DRV_P4INVALID = 20069
Global Const DRV_INIERROR = 20070
Global Const DRV_COFERROR = 20071
Global Const DRV_ACQUIRING = 20072
Global Const DRV_IDLE = 20073
Global Const DRV_TEMPCYCLE = 20074
Global Const DRV_NOT_INITIALIZED = 20075
Global Const DRV_P5INVALID = 20076
Global Const DRV_P6INVALID = 20077
Global Const DRV_INVALID_MODE = 20078
Global Const DRV_INVALID_FILTER = 20079


Global Const DRV_I2CERRORS = 20080
Global Const DRV_I2CDEVNOTFOUND = 20081
Global Const DRV_I2CTIMEOUT = 20082

Declare Function Initialize Lib "atmcd32d.dll" (ByVal dirName As String) As Long
Declare Function GetDetector Lib "atmcd32d.dll" (xpixels As Long, ypixels As Long) As Long
Declare Function SetExposureTime Lib "atmcd32d.dll" (ByVal expTime As Single) As Long
Declare Function StartAcquisition Lib "atmcd32d.dll" () As Long
Declare Function GetAcquisitionTimings Lib "atmcd32d.dll" (expo As Single, acc As Single, kin As Single) As Long
Declare Function GetAcquiredData Lib "atmcd32d.dll" (DArray As Any, ByVal sizeArray As Long) As Long
Declare Function GetStatus Lib "atmcd32d.dll" (status As Long) As Long
Declare Function SetTriggerMode Lib "atmcd32d.dll" (ByVal Trigger As Long) As Long
Declare Function SetReadMode Lib "atmcd32d.dll" (ByVal readMode As Long) As Long
Declare Function AbortAcquisition Lib "atmcd32d.dll" () As Long
Declare Function ShutDown Lib "atmcd32d.dll" () As Long
Declare Function SetAccumulationCycleTime Lib "atmcd32d.dll" (ByVal AccTime As Single) As Long
Declare Function SetKineticCycleTime Lib "atmcd32d.dll" (ByVal kineticTime As Single) As Long
Declare Function SetTemperature Lib "atmcd32d.dll" (ByVal temperature As Long) As Long
Declare Function CoolerON Lib "atmcd32d.dll" () As Long
Declare Function CoolerOFF Lib "atmcd32d.dll" () As Long
Declare Function SetAcquisitionMode Lib "atmcd32d.dll" (ByVal AcqMode As Long) As Long
Declare Function SetNumberAccumulations Lib "atmcd32d.dll" (ByVal number As Long) As Long
Declare Function SetNumberKinetics Lib "atmcd32d.dll" (ByVal number As Long) As Long
Declare Function SetFullImage Lib "atmcd32d.dll" (ByVal hbin As Long, ByVal vbin As Long) As Long
Declare Function SetMultiTrack Lib "atmcd32d.dll" (ByVal number As Long, ByVal height As Long, ByVal offset As Long, bottom As Long, gap As Long) As Long
Declare Function SetSingleTrack Lib "atmcd32d.dll" (ByVal centre As Long, ByVal height As Long) As Long
Declare Function SetHorizontalSpeed Lib "atmcd32d.dll" (ByVal index As Long) As Long
Declare Function SetVerticalSpeed Lib "atmcd32d.dll" (ByVal index As Long) As Long
Declare Function GetTemperature Lib "atmcd32d.dll" (temperature As Long) As Long
Declare Function GetTemperatureRange Lib "atmcd32d.dll" (temperatureMin As Long, temperatureMax As Long) As Long
Declare Function SetShutter Lib "atmcd32d.dll" (ByVal ShutterType As Long, ByVal mode As Long, ByVal closingTime As Long, ByVal openingTime As Long) As Long
Declare Function OutAuxPort Lib "atmcd32d.dll" (ByVal port As Long, ByVal state As Long) As Long
Declare Function InAuxPort Lib "atmcd32d.dll" (ByVal port As Long, state As Long) As Long
Declare Function GetNumberHorizontalSpeeds Lib "atmcd32d.dll" (number As Long) As Long
Declare Function GetHorizontalSpeed Lib "atmcd32d.dll" (ByVal index As Long, speed As Long) As Long
Declare Function GetNumberVerticalSpeeds Lib "atmcd32d.dll" (number As Long) As Long
Declare Function GetVerticalSpeed Lib "atmcd32d.dll" (ByVal index As Long, speed As Long) As Long
Declare Function GetHardwareVersion Lib "atmcd32d.dll" (PCB As Long, Decode As Long, SerPar As Long, Clocks As Long, dummy1 As Long, dummy2 As Long) As Long
Declare Function GetSoftwareVersion Lib "atmcd32d.dll" (Eprom As Long, CofFile As Long, VXDrev As Long, VXDver As Long, DLLrev As Long, DLLver As Long) As Long

Declare Function SetImage Lib "atmcd32d.dll" (ByVal hbin As Long, ByVal vbin As Long, ByVal hstart As Long, ByVal hend As Long, ByVal vstart As Long, ByVal vend As Long) As Long
Declare Function SetComplexImage Lib "atmcd32d.dll" (ByVal numAreas As Long, areas As Long) As Long
Declare Function SetRandomTracks Lib "atmcd32d.dll" (ByVal numTracks As Long, areas As Long) As Long
Declare Function SetFilterMode Lib "atmcd32d.dll" (ByVal mode As Long) As Long
Declare Function GetFilterMode Lib "atmcd32d.dll" (mode As Long) As Long
Declare Function SetFilterParameters Lib "atmcd32d.dll" (ByVal width As Long, ByVal sensitivity As Single, ByVal range As Long, ByVal accept As Single, ByVal smooth As Long, ByVal noise As Long) As Long
Declare Function GPIBSend Lib "atmcd32d.dll" (ByVal id As Long, ByVal address As Integer, ByVal text As String) As Long
Declare Function GPIBReceive Lib "atmcd32d.dll" (ByVal id As Long, ByVal address As Integer, text As String, ByVal size As Long) As Long
Declare Function SaveAsSif Lib "atmcd32d.dll" (ByVal path As String) As Long

Declare Function I2CBurstWrite Lib "atmcd32d.dll" (ByVal i2cAddress As Byte, ByVal nBytes As Long, data As Byte) As Long
Declare Function I2CBurstRead Lib "atmcd32d.dll" (ByVal i2cAddress As Byte, ByVal nBytes As Long, data As Byte) As Long
Declare Function I2CReset Lib "atmcd32d.dll" () As Long
Declare Function I2CRead Lib "atmcd32d.dll" (ByVal deviceID As Byte, ByVal intAddress As Byte, pdata As Byte) As Long
Declare Function I2CWrite Lib "atmcd32d.dll" (ByVal deviceID As Byte, ByVal intAddress As Byte, ByVal data As Byte) As Long
Declare Function SetGain Lib "atmcd32d.dll" (ByVal gain As Long) As Long
Declare Function SetMCPGating Lib "atmcd32d.dll" (ByVal gating As Long) As Long
Declare Function SetGateMode Lib "atmcd32d.dll" (ByVal gatemode As Long) As Long

Declare Function SetFastKinetics Lib "atmcd32d.dll" (ByVal exposedRows As Long, ByVal seriesLength As Long, ByVal time As Single, ByVal mode As Long, ByVal hbin As Long, ByVal vbin As Long) As Long
Declare Function GetFKExposureTime Lib "atmcd32d.dll" (time As Single) As Long
Declare Function GetNumberFKVShiftSpeeds Lib "atmcd32d.dll" (number As Long) As Long
Declare Function SetFKVShiftSpeed Lib "atmcd32d.dll" (ByVal index As Long) As Long
Declare Function GetFKVShiftSpeed Lib "atmcd32d.dll" (ByVal index, speed As Long) As Long

