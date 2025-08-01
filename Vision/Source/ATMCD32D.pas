unit atmcd32d;

interface

const
    DRV_ERROR_CODES	= 20001;
    DRV_SUCCESS	= 20002;
    DRV_VXDNOTINSTALLED	= 20003;
    DRV_ERROR_SCAN	= 20004;
    DRV_ERROR_CHECK_SUM	= 20005;
    DRV_ERROR_FILELOAD	= 20006;
    DRV_UNKNOWN_FUNCTION	= 20007;
    DRV_ERROR_VXD_INIT	= 20008;
    DRV_ERROR_ADDRESS	= 20009;
    DRV_ERROR_PAGELOCK	= 20010;
    DRV_ERROR_PAGEUNLOCK	= 20011;
    DRV_ERROR_BOARDTEST	= 20012;
    DRV_ERROR_ACK	= 20013;
    DRV_ERROR_UP_FIFO	= 20014;
    DRV_ERROR_PATTERN	= 20015;
    DRV_ACQUISITION_ERRORS	= 20017;
    DRV_ACQ_BUFFER	= 20018;
    DRV_ACQ_DOWNFIFO_FULL	= 20019;
    DRV_PROC_UNKONWN_INSTRUCTION	= 20020;
    DRV_ILLEGAL_OP_CODE	= 20021;
    DRV_KINETIC_TIME_NOT_MET	= 20022;
    DRV_ACCUM_TIME_NOT_MET	= 20023;
    DRV_NO_NEW_DATA	= 20024;
    DRV_SPOOLERROR = 20026;
    DRV_TEMPERATURE_CODES	= 20033;
    DRV_TEMPERATURE_OFF	= 20034;
    DRV_TEMPERATURE_NOT_STABILIZED	= 20035;
    DRV_TEMPERATURE_STABILIZED	= 20036;
    DRV_TEMPERATURE_NOT_REACHED	= 20037;
    DRV_TEMPERATURE_OUT_RANGE	= 20038;
    DRV_TEMPERATURE_NOT_SUPPORTED	= 20039;
    DRV_TEMPERATURE_DRIFT	= 20040;
    DRV_TEMP_CODES	= 20033;
    DRV_TEMP_OFF	= 20034;
    DRV_TEMP_NOT_STABILIZED	= 20035;
    DRV_TEMP_STABILIZED	= 20036;
    DRV_TEMP_NOT_REACHED	= 20037;
    DRV_TEMP_OUT_RANGE	= 20038;
    DRV_TEMP_NOT_SUPPORTED	= 20039;
    DRV_TEMP_DRIFT	= 20040;
    DRV_GENERAL_ERRORS	= 20049;
    DRV_INVALID_AUX	= 20050;
    DRV_COF_NOTLOADED	= 20051;
    DRV_FPGAPROG = 20052;
    DRV_FLEXERROR = 20053;
    DRV_GPIBERROR = 20054;
    DRV_DATATYPE	= 20064;
    DRV_DRIVER_ERRORS	= 20065;
    DRV_P1INVALID	= 20066;
    DRV_P2INVALID	= 20067;
    DRV_P3INVALID	= 20068;
    DRV_P4INVALID	= 20069;
    DRV_INIERROR	= 20070;
    DRV_COFERROR	= 20071;
    DRV_ACQUIRING	= 20072;
    DRV_IDLE	= 20073;
    DRV_TEMPCYCLE	= 20074;
    DRV_NOT_INITIALIZED = 20075;
    DRV_P5INVALID	= 20076;
    DRV_P6INVALID	= 20077;
    DRV_INVALID_MODE	= 20078;
    DRV_INVALID_FILTER = 20079;
    DRV_I2CERRORS	= 20080;
    DRV_I2CDEVNOTFOUND	= 20081;
    DRV_I2CTIMEOUT	= 20082;
    DRV_P7INVALID	= 20083;
    DRV_USBERROR = 20089;
    DRV_IOCERROR = 20090;
    DRV_VRMVERSIONERROR = 20091;
    DRV_ERROR_NOCAMERA = 20990;
    DRV_NOT_SUPPORTED = 20991;
    DRV_ERROR_MAP = 20115;
    DRV_ERROR_UNMAP = 20116;
    DRV_ERROR_MDL = 20117;
    DRV_ERROR_UNMDL = 20118;
    DRV_ERROR_BUFFSIZE = 20119;
    DRV_ERROR_NOHANDLE = 20121;
    DRV_GATING_NOT_AVAILABLE = 20130;
    DRV_FPGA_VOLTAGE_ERROR   = 20131;

    function SetExposureTime(time : Single): integer; stdcall;
    function SetNumberAccumulations(number: Integer ): integer; stdcall;
    function SetAccumulationCycleTime(time: Single ): integer; stdcall;
    function SetNumberKinetics(number: Integer ): integer; stdcall;
    function SetKineticCycleTime(time: Single ): integer; stdcall;
    function SetAcquisitionMode(mode: Integer ): integer; stdcall;
    function SetHorizontalSpeed(index: Integer ): integer; stdcall;
    function SetVerticalSpeed(index: Integer ): integer; stdcall;
    function SetReadMode(mode: Integer ): integer; stdcall;
    function SetPhotonCounting(state: Integer ): integer; stdcall;
    function SetPhotonCountingThreshold(min: Integer; max: Integer): integer; stdcall;
    function SetMultiTrack(number: Integer ; height: Integer ; offset: Integer ; var bottom: Integer ; var gap: Integer): integer; stdcall;
    function SetSingleTrack(centre: Integer ; height: Integer ): integer; stdcall;
    function SetFullImage(hbin: Integer ; vbin: Integer ): integer; stdcall;
    function GetAcquisitionTimings(var exposure: Single ; var accumulate: Single ; var kinetic: Single): integer; stdcall;
    function PrepareAcquisition(): integer; stdcall;
    function FreeInternalMemory(): integer; stdcall;
    function StartAcquisition: integer; stdcall;
    function AbortAcquisition: integer; stdcall;
    function GetAcquiredData(arr: Pointer ; size: Integer ): integer; stdcall;
    function GetStatus(var status: Integer): integer; stdcall;
    function SetTriggerMode(mode: Integer ): integer; stdcall;
    function Initialize(dir: PChar ): integer; stdcall;
    function ShutDown: integer; stdcall;
    function SetTemperature(temperature: Integer ): integer; stdcall;
    function GetTemperature(var temperature: Integer): integer; stdcall;
    function GetTemperatureF(var temperature: Single): integer; stdcall;
    function GetTemperatureRange(var mintemp: Integer; var maxtemp: Integer): integer; stdcall;
    function CoolerON: integer; stdcall;
    function CoolerOFF: integer; stdcall;
    function SetShutter(typ: Integer ; mode: Integer ; closingtime: Integer ; openingtime: Integer ): integer; stdcall;
    function OutAuxPort(port: Integer ; state: Integer ): integer; stdcall;
    function InAuxPort(port: Integer ; var state: Integer ): integer; stdcall;
    function GetNumberHorizontalSpeeds(var number: Integer ): integer; stdcall;
    function GetHorizontalSpeed(index: Integer ; speed: Integer): integer; stdcall;
    function GetNumberVerticalSpeeds(var number: Integer): integer; stdcall;
    function GetVerticalSpeed(index: Integer ; var speed: Integer): integer; stdcall;
    function GetDetector(var xpixels: Integer ; var ypixels: Integer): integer; stdcall;
    function GetImagesPerDMA(var images: Integer): integer; stdcall;
    function GetSoftwareVersion(var eprom: Integer; var coffile: Integer; var vxdrev: Integer; var vxdver: Integer; var dllrev: Integer; var dllver: Integer): integer; stdcall;
    function GetHardwareVersion(var PCB: Integer; var Decode: Integer; var SerPar: Integer; var Clocks: Integer; var dummy1: Integer; var dummy2: Integer): integer; stdcall;
    function SetImage(hbin: Integer ; vbin: Integer ; hstart: Integer ; hend: Integer ; vstart: Integer ; vend: Integer ): integer; stdcall;
    function SetUSGenomics(width: Integer ; height: Integer ): integer; stdcall;
    function SetFastKinetics(exposedRows: Integer ; seriesLength: Integer ; time: Single ; mode: Integer ; hbin: Integer ; vbin: Integer ): integer; stdcall;
    function SetFastKineticsEx(exposedRows: Integer ; seriesLength: Integer ; time: Single ; mode: Integer ; hbin: Integer ; vbin: Integer ; offset: Integer): integer; stdcall;
    function GetFKExposureTime(var time: Single): integer; stdcall;
    function GetNumberFKVShiftSpeeds(var number: Integer): integer; stdcall;
    function SetFKVShiftSpeed(index: Integer ): integer; stdcall;
    function GetFKVShiftSpeed(index: Integer ; var speed: Integer): integer; stdcall;
    function GetFKVShiftSpeedF(index: Integer ; var speed: Single): integer; stdcall;
    function SetComplexImage(numAreas: Integer ; areas: PInteger): integer; stdcall;
    function SetRandomTracks(numTracks: Integer ; areas: Integer): integer; stdcall;
    function SetDriverEvent(event: Integer): integer; stdcall;
    function I2CBurstWrite(i2cAddress: Byte; nBytes: Integer; data: PByte): integer; stdcall;
    function I2CBurstRead(i2cAddress: Byte; nBytes: Integer; data: PByte): integer; stdcall;
    function I2CReset(): integer; stdcall;
    function I2CRead(deviceID: Byte; intAddress: Byte; var pdata: Byte): integer; stdcall;
    function I2CWrite(deviceID: Byte; intAddress: Byte; data: Byte): integer; stdcall;
    function SetGain(gain: Integer ): integer; stdcall;
    function SetMCPGating(gating: Integer ): integer; stdcall;
    function SetGateMode(gatemode: Integer ): integer; stdcall;
    function SetFilterMode(mode: Integer ): integer; stdcall;
    function GetFilterMode(var mode: Integer): integer; stdcall;
    function SetFilterParameters(width: Integer ; sensitivity: Single ; range: Integer ; accept: Single ; smooth: Integer ; noise: Integer ): integer; stdcall;
    function GPIBSend(id: Integer ; address: Single ; text:PChar ): integer; stdcall;
    function GPIBReceive(id: Integer ; address: Single ; text:PChar ; size: Integer ): integer; stdcall;
    function SaveAsEDF(path:PChar ; mode: Integer ): integer; stdcall;
    function SaveAsSif(path:PChar ): integer; stdcall;
    function SaveAsCommentedSif(path: PChar; comment: PChar): integer; stdcall;
    function SaveAsTiff(path:PChar ; palette:PChar; position: Integer ; mode: Integer ): integer; stdcall;
    function SetSifComment(comment: PChar): integer; stdcall;
    function GetAvailableCameras(var totalCameras: Integer): integer; stdcall;
    function SetCurrentCamera(cameraHandle: Integer): integer; stdcall;
    function GetCurrentCamera(var cameraHandle: Integer): integer; stdcall;
    function GetCameraHandle(cameraIndex: Integer; var cameraHandle: Integer): integer; stdcall;
    function SetDelayGenerator(board: Integer ; address: Single ; typ: Integer ): integer; stdcall;
    function SetGate(delay: Single ; width: Single ; step: Single ): integer; stdcall;
    function SetSingleTrackHBin(bin: Integer ): integer; stdcall;
    function SetMultiTrackHBin(bin: Integer ): integer; stdcall;
    function SetFVBHBin(bin: Integer ): integer; stdcall;
    function SetCustomTrackHBin(bin: Integer ): integer; stdcall;
    function GetNewData(arr: PInteger; size: Integer ): integer; stdcall;
    function GetAllDMAData(arr: PInteger; size: Integer ): integer; stdcall;
    function GetDDGIOCPulses(var pulses: Integer): integer; stdcall;
    function GetDDGPulse(width:Double ; resolution:Double; var Delay:Double; var Wid:Double): integer; stdcall;
    function SetDDGAddress(t0: Byte; t1: Byte; t2: Byte; tt: Byte; address: Byte): integer; stdcall;
    function SetDDGGain(gain: Integer ): integer; stdcall;
    function SetDDGGateStep(step:Double): integer; stdcall;
    function SetDDGInsertionDelay(state: Integer ): integer; stdcall;
    function SetDDGIntelligate(state: Integer ): integer; stdcall;
    function SetDDGIOC(state: Integer ): integer; stdcall;
    function SetDDGIOCFrequency(frequency:Double): integer; stdcall;
    function SetDDGTimes(t0:Double ; t1:Double; t2:Double): integer; stdcall;
    function SetDDGTriggerMode(mode: Integer ): integer; stdcall;
    function SetDDGVariableGateStep(mode: Integer ; p1:Double; p2:Double): integer; stdcall;
    function SetEMCCDGain(gain: Integer ): integer; stdcall;
    function SaveAsBmp(path:PChar ; palette:PChar; ymin: Integer ; ymax: Integer ): integer; stdcall;
    function SaveToClipboard(palette:PChar): integer; stdcall;
    function SetSpool(active: Integer ; method: Integer ; path:PChar ; framebuffersize: Integer ): integer; stdcall;
    function SetFastExtTrigger(mode: Integer ): integer; stdcall;
    function GetAcquisitionProgress(var acc: Integer; var series: Integer): integer; stdcall;
    function Merge(arr: PInteger; nOrder: Integer; nPoint: Integer; nPixel: Integer; coeff: PSingle; fit: Integer; hbin: Integer; output: PInteger; var start: Single; var step: Single): integer; stdcall;
    function GetNumberADChannels(var channels: Integer): integer; stdcall;
    function SetADChannel(channel: Integer): integer; stdcall;
    function GetNumberHSSpeeds(channel: Integer; typ: Integer; var speeds: Integer): integer; stdcall;
    function GetHSSpeed(channel: Integer; typ: Integer; index: Integer; var speed: Single): integer; stdcall;
    function SetHSSpeed(typ: Integer; index: Integer): integer; stdcall;
    function GetNumberVSSpeeds(var speeds: Integer): integer; stdcall;
    function GetVSSpeed(index: Integer; var speed: Single): integer; stdcall;
    function SetVSSpeed(index: Integer): integer; stdcall;
    function SetVSAmplitude(index: Integer): integer; stdcall;
    function SetFanMode(mode: Integer): integer; stdcall;
    function GetNumberAmp(var amp: Integer): integer; stdcall;
    function GetAmpMaxSpeed(index: Integer; var speed: Single): integer; stdcall;
    function GetAmpDesc(index: Integer; name: PChar; len: Integer): integer; stdcall;
    function SetVerticalRowBuffer(rows: Integer): integer; stdcall;
    function GetRegisterDump(var mode: Integer): integer; stdcall;
    function SetRegisterDump(mode: Integer): integer; stdcall;
    function GetCameraSerialNumber(var number: Integer): integer; stdcall;
    function GetPixelSize(var xSize: Single; var ySize: Single): integer; stdcall;
    function GetHeadModel(name: PChar): integer; stdcall;
    function GetNewData16(arr: PWord; size: Integer): integer; stdcall;
    function GetAcquiredData16(arr: PWord; size: Integer): integer; stdcall;
    function GetNewData8(arr: PByte; size: Integer): integer; stdcall;
    function GetCapabilities(caps: Pointer): integer; stdcall;
    function SetMessageWindow(wnd: Integer): integer; stdcall;
    function SelectDevice(devNum: Integer): integer; stdcall;
    function GetNumberDevices(var numDevs: Integer): integer; stdcall;
    function GetID(devNum: Integer; var id: Integer): integer; stdcall;
    function SetPixelMode(bitdepth: Integer; colorMode: Integer): integer; stdcall;
    function IdAndorDll(): integer; stdcall;
    function SetAcquisitionType(typ: Integer): integer; stdcall;
    function SetDataType(typ: Integer): integer; stdcall;
    function SetBackground(arr: PInteger; size: Integer): integer; stdcall;
    function GetBackground(arr: PInteger; size: Integer): integer; stdcall;
    function GetAcquiredFloatData(arr: PSingle; size: Integer): integer; stdcall;
    function GetNewFloatData(arr: PSingle; size: Integer): integer; stdcall;
    function SetBaselineClamp(state: Integer): integer; stdcall;
    function SetHighCapacity(state: Integer): integer; stdcall;
    function SetOutputAmplifier(type2: Integer): integer; stdcall;
    function GetNumberPreAmpGains(var noGains: Integer): integer; stdcall;
    function GetPreAmpGain(index2: Integer; var gain: Single): integer; stdcall;
    function SetPreAmpGain(index: Integer): integer; stdcall;
    function SetUserEvent(event: Integer): integer; stdcall;
    function SetStorageMode(mode: Integer): integer; stdcall;
    function SetNextAddress(arr: Pointer; lowAdd: Integer; highAdd: Integer; len: Integer; physical: Integer): integer; stdcall;    
    function UnMapPhysicalAddress: integer; stdcall;
    function SetNextAddress16(arr: Pointer; lowAdd: Integer; highAdd: Integer; len: Integer; physical: Integer): integer; stdcall;    
    function GetSpoolProgress(var index: Integer): integer; stdcall;
    function GetTotalNumberImagesAcquired(var index: Integer): integer; stdcall;
    function GetSizeOfCircularBuffer(var index: Integer): integer; stdcall;
    function GetMostRecentImage(arr: PInteger; size: Integer): integer; stdcall;
    function GetOldestImage(arr: PInteger; size: Integer): integer; stdcall;
    function GetNumberNewImages(var first: Integer; var last: Integer): integer; stdcall;
    function GetImages(first: Integer; last: Integer; arr: PInteger; size: Integer; var validfirst: Integer; var validlast: Integer): integer; stdcall;
    function GetMostRecentImage16(arr: PWord; size: Integer): integer; stdcall;
    function GetOldestImage16(arr: PWord; size: Integer): integer; stdcall;
    function GetImages16(first: Integer; last: Integer; arr: PWord; size: Integer; var validfirst: Integer; var validlast: Integer): integer; stdcall;
    function SetFrameTransferMode(mode: Integer ): integer; stdcall;
    function GetNumberVSAmplitudes(var number: Integer): integer; stdcall;
    function GetBitDepth(channel: Integer ; var depth: Integer): integer; stdcall;
    function GetCameraInformation(index: Integer ; var information: Integer): integer; stdcall;
    function SetCoolerMode(mode: Integer ): integer; stdcall;
    function IsPreAmpGainAvailable(channel: Integer; amp: Integer; index: Integer; pa: Integer; var state: Integer): integer; stdcall;
    function GetFastestRecommendedVSSpeed(var index: Integer; var speed: Single): integer; stdcall;
    function GetSlotBusDeviceFunction(var Slot:Integer; var Bus:Integer; var Device:Integer; var DevFunction:Integer): integer; stdcall;
    function InitializeDevice(dir: PChar ): integer; stdcall;
    function SetDMAParameters(MaxImagesPerDMA: Integer; SecondsPerDMA: Single): integer; stdcall;
    function IsInternalMechanicalShutter(var InternalShutter: Integer): integer; stdcall;
    function SaveEEPROMToFile(cFileName: PChar): integer; stdcall;
    function GetHVflag(var flag: Integer): integer; stdcall;
    function GetMCPGain(number: Integer; gain: PInteger; photoepc: PSingle): integer; stdcall;
    function GetMCPVoltage(var voltage: Integer): integer; stdcall;
    function WaitForAcquisition: integer; stdcall;
    function WaitForAcquisitionByHandle(cameraHandle: Integer): integer; stdcall;
    function CancelWait: integer; stdcall;
    function GetMostRecentColorImage16(size: Integer; algorithm: Integer; red: PWord; green: PWord; blue: PWord):integer;stdcall;
    function GetControllerCardModel(controllerCardModel: PChar):integer;stdcall;

implementation

    function SetExposureTime; external 'atmcd32d.dll' name 'SetExposureTime';
    function SetNumberAccumulations; external 'atmcd32d.dll' name 'SetNumberAccumulations';
    function SetAccumulationCycleTime; external 'atmcd32d.dll' name 'SetAccumulationCycleTime';
    function SetNumberKinetics; external 'atmcd32d.dll' name 'SetNumberKinetics';
    function SetKineticCycleTime; external 'atmcd32d.dll' name 'SetKineticCycleTime';
    function SetAcquisitionMode; external 'atmcd32d.dll' name 'SetAcquisitionMode';
    function SetHorizontalSpeed; external 'atmcd32d.dll' name 'SetHorizontalSpeed';
    function SetVerticalSpeed; external 'atmcd32d.dll' name 'SetVerticalSpeed';
    function SetReadMode; external 'atmcd32d.dll' name 'SetReadMode';
    function SetPhotonCounting; external 'atmcd32d.dll' name 'SetPhotonCounting';
    function SetPhotonCountingThreshold; external 'atmcd32d.dll' name 'SetPhotonCountingThreshold';
    function SetMultiTrack; external 'atmcd32d.dll' name 'SetMultiTrack';
    function SetSingleTrack; external 'atmcd32d.dll' name 'SetSingleTrack';
    function SetFullImage; external 'atmcd32d.dll' name 'SetFullImage';
    function GetAcquisitionTimings; external 'atmcd32d.dll' name 'GetAcquisitionTimings';
    function PrepareAcquisition; external 'atmcd32d.dll' name 'PrepareAcquisition';
    function FreeInternalMemory; external 'atmcd32d.dll' name 'FreeInternalMemory';
    function StartAcquisition; external 'atmcd32d.dll' name 'StartAcquisition';
    function AbortAcquisition; external 'atmcd32d.dll' name 'AbortAcquisition';
    function GetAcquiredData; external 'atmcd32d.dll' name 'GetAcquiredData';
    function GetStatus; external 'atmcd32d.dll' name 'GetStatus';
    function SetTriggerMode; external 'atmcd32d.dll' name 'SetTriggerMode';
    function Initialize; external 'atmcd32d.dll' name 'Initialize';
    function ShutDown; external 'atmcd32d.dll' name 'ShutDown';
    function SetTemperature; external 'atmcd32d.dll' name 'SetTemperature';
    function GetTemperature; external 'atmcd32d.dll' name 'GetTemperature';
    function GetTemperatureF; external 'atmcd32d.dll' name 'GetTemperatureF';
    function GetTemperatureRange; external 'atmcd32d.dll' name 'GetTemperatureRange';
    function CoolerON; external 'atmcd32d.dll' name 'CoolerON';
    function CoolerOFF; external 'atmcd32d.dll' name 'CoolerOFF';
    function SetShutter; external 'atmcd32d.dll' name 'SetShutter';
    function OutAuxPort; external 'atmcd32d.dll' name 'OutAuxPort';
    function InAuxPort; external 'atmcd32d.dll' name 'InAuxPort';
    function GetNumberHorizontalSpeeds; external 'atmcd32d.dll' name 'GetNumberHorizontalSpeeds';
    function GetHorizontalSpeed; external 'atmcd32d.dll' name 'GetHorizontalSpeed';
    function GetNumberVerticalSpeeds; external 'atmcd32d.dll' name 'GetNumberVerticalSpeeds';
    function GetVerticalSpeed; external 'atmcd32d.dll' name 'GetVerticalSpeed';
    function GetDetector; external 'atmcd32d.dll' name 'GetDetector';
    function GetImagesPerDMA; external 'atmcd32d.dll' name 'GetImagesPerDMA';
    function GetSoftwareVersion; external 'atmcd32d.dll' name 'GetSoftwareVersion';
    function GetHardwareVersion; external 'atmcd32d.dll' name 'GetHardwareVersion';
    function SetImage; external 'atmcd32d.dll' name 'SetImage';
    function SetUSGenomics; external 'atmcd32d.dll' name 'SetUSGenomics';
    function SetFastKinetics; external 'atmcd32d.dll' name 'SetFastKinetics';
    function SetFastKineticsEx; external 'atmcd32d.dll' name 'SetFastKineticsEx';
    function GetFKExposureTime; external 'atmcd32d.dll' name 'GetFKExposureTime';
    function GetNumberFKVShiftSpeeds; external 'atmcd32d.dll' name 'GetNumberFKVShiftSpeeds';
    function SetFKVShiftSpeed; external 'atmcd32d.dll' name 'SetFKVShiftSpeed';
    function GetFKVShiftSpeed; external 'atmcd32d.dll' name 'GetFKVShiftSpeed';
    function GetFKVShiftSpeedF; external 'atmcd32d.dll' name 'GetFKVShiftSpeedF';
    function SetComplexImage; external 'atmcd32d.dll' name 'SetComplexImage';
    function SetRandomTracks; external 'atmcd32d.dll' name 'SetRandomTracks';
    function SetDriverEvent; external 'atmcd32d.dll' name 'SetDriverEvent';
    function I2CBurstWrite; external 'atmcd32d.dll' name 'I2CBurstWrite';
    function I2CBurstRead; external 'atmcd32d.dll' name 'I2CBurstRead';
    function I2CReset; external 'atmcd32d.dll' name 'I2CReset';
    function I2CRead; external 'atmcd32d.dll' name 'I2CRead';
    function I2CWrite; external 'atmcd32d.dll' name 'I2CWrite';
    function SetGain; external 'atmcd32d.dll' name 'SetGain';
    function SetMCPGating; external 'atmcd32d.dll' name 'SetMCPGating';
    function SetGateMode; external 'atmcd32d.dll' name 'SetGateMode';
    function SetFilterMode; external 'atmcd32d.dll' name 'SetFilterMode';
    function GetFilterMode; external 'atmcd32d.dll' name 'GetFilterMode';
    function SetFilterParameters; external 'atmcd32d.dll' name 'SetFilterParameters';
    function GPIBSend; external 'atmcd32d.dll' name 'GPIBSend';
    function GPIBReceive; external 'atmcd32d.dll' name 'GPIBReceive';
    function SaveAsEDF; external 'atmcd32d.dll' name 'SaveAsEDF';
    function SaveAsSif; external 'atmcd32d.dll' name 'SaveAsSif';
    function SaveAsCommentedSif; external 'atmcd32d.dll' name 'SaveAsCommentedSif';
    function SetSifComment; external 'atmcd32d.dll' name 'SetSifComment';
    function GetAvailableCameras; external 'atmcd32d.dll' name 'GetAvailableCameras';
    function SetCurrentCamera; external 'atmcd32d.dll' name 'SetCurrentCamera';
    function GetCurrentCamera; external 'atmcd32d.dll' name 'GetCurrentCamera';
    function GetCameraHandle; external 'atmcd32d.dll' name 'GetCameraHandle';
    function SetDelayGenerator; external 'atmcd32d.dll' name 'SetDelayGenerator';
    function SetGate; external 'atmcd32d.dll' name 'SetGate';
    function SetSingleTrackHBin; external 'atmcd32d.dll' name 'SetSingleTrackHBin';
    function SetMultiTrackHBin; external 'atmcd32d.dll' name 'SetMultiTrackHBin';
    function SetFVBHBin; external 'atmcd32d.dll' name 'SetFVBHBin';
    function SetCustomTrackHBin; external 'atmcd32d.dll' name 'SetCustomTrackHBin';
    function GetNewData; external 'atmcd32d.dll' name 'GetNewData';
    function GetAllDMAData; external 'atmcd32d.dll' name 'GetAllDMAData';
    function GetDDGIOCPulses; external 'atmcd32d.dll' name 'GetDDGIOCPulses';
    function GetDDGPulse; external 'atmcd32d.dll' name 'GetDDGPulse';
    function SetDDGAddress; external 'atmcd32d.dll' name 'SetDDGAddress';
    function SetDDGGain; external 'atmcd32d.dll' name 'SetDDGGain';
    function SetDDGGateStep; external 'atmcd32d.dll' name 'SetDDGGateStep';
    function SetDDGInsertionDelay; external 'atmcd32d.dll' name 'SetDDGInsertionDelay';
    function SetDDGIntelligate; external 'atmcd32d.dll' name 'SetDDGIntelligate';
    function SetDDGIOC; external 'atmcd32d.dll' name 'SetDDGIOC';
    function SetDDGIOCFrequency; external 'atmcd32d.dll' name 'SetDDGIOCFrequency';
    function SetDDGTimes; external 'atmcd32d.dll' name 'SetDDGTimes';
    function SetDDGTriggerMode; external 'atmcd32d.dll' name 'SetDDGTriggerMode';
    function SetDDGVariableGateStep; external 'atmcd32d.dll' name 'SetDDGVariableGateStep';
    function SetEMCCDGain; external 'atmcd32d.dll' name 'SetEMCCDGain';
    function SaveAsBmp; external 'atmcd32d.dll' name 'SaveAsBmp';
    function SaveToClipboard; external 'atmcd32d.dll' name 'SaveToClipboard';    
    function SetSpool; external 'atmcd32d.dll' name 'SetSpool';
    function SetFastExtTrigger; external 'atmcd32d.dll' name 'SetFastExtTrigger';
    function GetAcquisitionProgress; external 'atmcd32d.dll' name 'GetAcquisitionProgress';
    function Merge; external 'atmcd32d.dll' name 'Merge';
    function GetNumberADChannels; external 'atmcd32d.dll' name 'GetNumberADChannels';
    function SetADChannel; external 'atmcd32d.dll' name 'SetADChannel';
    function GetNumberHSSpeeds; external 'atmcd32d.dll' name 'GetNumberHSSpeeds';
    function GetHSSpeed; external 'atmcd32d.dll' name 'GetHSSpeed';
    function SetHSSpeed; external 'atmcd32d.dll' name 'SetHSSpeed';
    function GetNumberVSSpeeds; external 'atmcd32d.dll' name 'GetNumberVSSpeeds';
    function GetVSSpeed; external 'atmcd32d.dll' name 'GetVSSpeed';
    function SetVSSpeed; external 'atmcd32d.dll' name 'SetVSSpeed';
    function SetVSAmplitude; external 'atmcd32d.dll' name 'SetVSAmplitude';
    function SetFanMode; external 'atmcd32d.dll' name 'SetFanMode';
    function GetNumberAmp; external 'atmcd32d.dll' name 'GetNumberAmp';
    function GetAmpMaxSpeed; external 'atmcd32d.dll' name 'GetAmpMaxSpeed';
    function GetAmpDesc; external 'atmcd32d.dll' name 'GetAmpDesc';
    function SetVerticalRowBuffer; external 'atmcd32d.dll' name 'SetVerticalRowBuffer';
    function GetRegisterDump; external 'atmcd32d.dll' name 'GetRegisterDump';
    function SetRegisterDump; external 'atmcd32d.dll' name 'SetRegisterDump';
    function GetCameraSerialNumber; external 'atmcd32d.dll' name 'GetCameraSerialNumber';
    function GetPixelSize; external 'atmcd32d.dll' name 'GetPixelSize';
    function GetHeadModel; external 'atmcd32d.dll' name 'GetHeadModel';
    function GetNewData16; external 'atmcd32d.dll' name 'GetNewData16';
    function GetAcquiredData16; external 'atmcd32d.dll' name 'GetAcquiredData16';
    function GetNewData8; external 'atmcd32d.dll' name 'GetNewData8';
    function GetCapabilities; external 'atmcd32d.dll' name 'GetCapabilities';
    function SetMessageWindow; external 'atmcd32d.dll' name 'SetMessageWindow';
    function SelectDevice; external 'atmcd32d.dll' name 'SelectDevice';
    function GetNumberDevices; external 'atmcd32d.dll' name 'GetNumberDevices';
    function GetID; external 'atmcd32d.dll' name 'GetID';
    function SetPixelMode; external 'atmcd32d.dll' name 'SetPixelMode';
    function IdAndorDll; external 'atmcd32d.dll' name 'IdAndorDll';
    function SetAcquisitionType; external 'atmcd32d.dll' name 'SetAcquisitionType';
    function SetDataType; external 'atmcd32d.dll' name 'SetDataType';
    function SetBackground; external 'atmcd32d.dll' name 'SetBackground';
    function GetBackground; external 'atmcd32d.dll' name 'GetBackground';
    function GetAcquiredFloatData; external 'atmcd32d.dll' name 'GetAcquiredFloatData';
    function GetNewFloatData; external 'atmcd32d.dll' name 'GetNewFloatData';
    function SetBaselineClamp; external 'atmcd32d.dll' name 'SetBaselineClamp';
    function SetHighCapacity; external 'atmcd32d.dll' name 'SetHighCapacity';
    function SetOutputAmplifier; external 'atmcd32d.dll' name 'SetOutputAmplifier';
    function GetNumberPreAmpGains; external 'atmcd32d.dll' name 'GetNumberPreAmpGains';
    function GetPreAmpGain; external 'atmcd32d.dll' name 'GetPreAmpGain';
    function SetPreAmpGain; external 'atmcd32d.dll' name 'SetPreAmpGain';
    function SetUserEvent; external 'atmcd32d.dll' name 'SetUserEvent';
    function SetStorageMode; external 'atmcd32d.dll' name 'SetStorageMode';
    function SetNextAddress; external 'atmcd32d.dll' name 'SetNextAddress'; 
    function UnMapPhysicalAddress; external 'atmcd32d.dll' name 'UnMapPhysicalAddress';
    function SetNextAddress16; external 'atmcd32d.dll' name 'SetNextAddress16'; 
    function GetSpoolProgress; external 'atmcd32d.dll' name 'GetSpoolProgress'; 
    function GetTotalNumberImagesAcquired; external 'atmcd32d.dll' name 'GetTotalNumberImagesAcquired'; 
    function GetSizeOfCircularBuffer; external 'atmcd32d.dll' name 'GetSizeOfCircularBuffer'; 
    function GetMostRecentImage; external 'atmcd32d.dll' name 'GetMostRecentImage'; 
    function GetOldestImage; external 'atmcd32d.dll' name 'GetOldestImage'; 
    function GetNumberNewImages; external 'atmcd32d.dll' name 'GetNumberNewImages'; 
    function GetImages; external 'atmcd32d.dll' name 'GetImages'; 
    function GetMostRecentImage16; external 'atmcd32d.dll' name 'GetMostRecentImage16'; 
    function GetOldestImage16; external 'atmcd32d.dll' name 'GetOldestImage16'; 
    function GetImages16; external 'atmcd32d.dll' name 'GetImages16'; 
    function SetFrameTransferMode; external 'atmcd32d.dll' name 'SetFrameTransferMode';
    function GetNumberVSAmplitudes; external 'atmcd32d.dll' name 'GetNumberVSAmplitudes';
    function GetBitDepth; external 'atmcd32d.dll' name 'GetBitDepth';
    function GetCameraInformation; external 'atmcd32d.dll' name 'GetCameraInformation';
    function SetCoolerMode; external 'atmcd32d.dll' name 'SetCoolerMode';
    function IsPreAmpGainAvailable; external 'atmcd32d.dll' name 'IsPreAmpGainAvailable';
    function GetFastestRecommendedVSSpeed; external 'atmcd32d.dll' name 'GetFastestRecommendedVSSpeed';
    function IsInternalMechanicalShutter; external 'atmcd32d.dll' name 'IsInternalMechanicalShutter';
    function SaveEEPROMToFile; external 'atmcd32d.dll' name 'SaveEEPROMToFile';
    function GetSlotBusDeviceFunction; external 'atmcd32d.dll' name 'GetSlotBusDeviceFunction';
    function InitializeDevice; external 'atmcd32d.dll' name 'InitializeDevice';
    function SetDMAParameters; external 'atmcd32d.dll' name 'SetDMAParameters';
    function SaveAsTiff; external 'atmcd32d.dll' name 'SaveAsTiff';
    function GetHVflag; external 'atmcd32d.dll' name 'GetHVflag';
    function GetMCPGain; external 'atmcd32d.dll' name 'GetMCPGain';
    function GetMCPVoltage; external 'atmcd32d.dll' name 'GetMCPVoltage';
    function WaitForAcquisition; external 'atmcd32d.dll' name 'WaitForAcquisition';
    function WaitForAcquisitionByHandle; external 'atmcd32d.dll' name 'WaitForAcquisitionByHandle';
    function CancelWait; external 'atmcd32d.dll' name 'CancelWait';
    function GetMostRecentColorImage16; external 'atmcd32d.dll' name 'GetMostRecentColorImage16';
    function GetControllerCardModel; external 'atmcd32d.dll' name 'GetControllerCardModel';
end.


