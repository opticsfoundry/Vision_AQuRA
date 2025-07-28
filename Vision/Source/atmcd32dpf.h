#ifndef ATMCD32DPF_H
#define ATMCD32DPF_H

typedef unsigned int WINAPI (*pfI2CWrite)(byte deviceID, byte intAddress, byte data);
typedef unsigned int WINAPI (*pfI2CRead)(byte deviceID, byte intAddress, byte* data);
typedef unsigned int WINAPI (*pfI2CBurstWrite)(byte i2cAddress, long nBytes, byte* data);
typedef unsigned int WINAPI (*pfI2CBurstRead)(byte i2cAddress, long nBytes, byte* data);
typedef unsigned int WINAPI (*pfInitialize)(char* path);
typedef unsigned int WINAPI (*pfShutDown)(void);


#endif
