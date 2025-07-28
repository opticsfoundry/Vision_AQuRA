/**************************************************************************
 *
 * Name:   WINDECL.C
 *
 * C header for Computer Boards Windows GPIB driver
 *
 * (c) Copyright 1995-1997, Computer Boards
 * All rights reserved.
 *
 * This header file should be included in all Windows C programs that will
 * call the Computer Boards GPIB library.
 *
 **************************************************************************/

/* Global status variables */
#if defined(_MSC_VER)			/* Microsoft */
    #if defined (__cplusplus)
        extern "C"
        {
    #endif

    #if WINVER >= 0x0400
        _declspec( dllimport ) int  ibsta;
        _declspec( dllimport ) int  iberr;
        _declspec( dllimport ) int  ibcnt;
        _declspec( dllimport ) long ibcntl;
        _declspec( dllimport ) int  ThreadIbsta;
        _declspec( dllimport ) int  ThreadIberr;
        _declspec( dllimport ) int  ThreadIbcnt;
        _declspec( dllimport ) long ThreadIbcntl;
    #else
        extern  int  ibsta;
        extern  int  iberr;
        extern  int  ibcnt;
        extern  long ibcntl;
    #endif

    #if defined (__cplusplus)
        }
    #endif
#endif

#ifdef __BORLANDC__				/* Borland */
    #ifdef __WIN32__
       extern _import int  ibsta;
	   extern _import int  iberr;
	   extern _import int  ibcnt;
	   extern _import long ibcntl;
    #else
        extern _far int  ibsta;
        extern _far int  iberr;
        extern _far int  ibcnt;
        extern _far long ibcntl;
    #endif
#endif

/* GPIB Commands  */
#define GTL  0x01
#define SDC  0x04
#define PPC  0x05
#define GET  0x08
#define TCT  0x09
#define LLO  0x11
#define DCL  0x14
#define PPU  0x15
#define SPE  0x18
#define SPD  0x19
#define UNL  0x3f
#define UNT  0x5f
#define PPE  0x60
#define PPD  0x70


/* Bit specifiers for ibsta status variable and wait mask  */
#define ERR   (unsigned) 0x8000   /* Error                     */
#define TIMO  0x4000   /* Timeout                   */
#define END   0x2000   /* EOI or EOS received       */
#define SRQI  0x1000   /* SRQ received              */
#define RQS   0x0800   /* Device requesting service */
#define SPOLL 0x4000   /* Board's been serially polled  */
#define EVENT 0x2000   /* An event has occured      */
#define CMPL  0x0100   /* Operation completed       */
#define LOK   0x0080   /* Local lockout             */
#define REM   0x0040   /* Remote state              */
#define CIC   0x0020   /* Controller-in-Charge      */
#define ATN   0x0010   /* ATN line asserted         */
#define TACS  0x0008   /* Talker active             */
#define LACS  0x0004   /* Listener active           */
#define DTAS  0x0002   /* Device trigger state      */
#define DCAS  0x0001   /* Device clear state        */


/* System error code */
#define EDVR  0  /* DOS error                                */
#define ECIC  1  /* board should be CIC                      */
#define ENOL  2  /* No Listeners detected                    */
#define EADR  3  /* Board not addressed correctly            */
#define EARG  4  /* Invalid argument specified               */
#define ESAC  5  /* Board should be system controller        */
#define EABO  6  /* I/O operation aborted                    */
#define ENEB  7  /* Invalid interface board sprecified       */
#define EOIP 10  /* I/O operation already running            */
#define ECAP 11  /* Board does not have requested capability */
#define EFSO 12  /* Erro retunred from file system           */
#define EBUS 14  /* Command error on bus                     */
#define ESTB 15  /* Serial poll response byte lost           */
#define ESRQ 16  /* SRQ is still asserted                    */
#define ETAB 20  /* No device responding with ETAB           */
#define EINT 247 /* No interrupt configured on board.        */
#define EWMD 248 /* Windows is not in enhanced mode          */
#define EVDD 249 /* CBGPIB.386 is not installed              */
#define EOVR 250 /* Buffer overflow                          */
#define ESML 251 /* Two library calls running simultaneously */
#define ECFG 252 /* Board type doesn't match GPIB.CFG        */
#define ETMR 253 /* No Windows timers available              */
#define ESLC 254 /* No Windows selectors available           */
#define EBRK 255 /* Ctrl-Break pressed, exiting program      */


/* EOS mode bits                                           */
#define BIN  0x1000
#define XEOS 0x0800
#define REOS 0x0400


/* Timeout values and meanings                             */
#define TNONE    0   /* No timeout        */
#define T10us    1   /* 10  microseconds  */
#define T30us    2   /* 30  microseconds  */
#define T100us   3   /* 100 microseconds  */
#define T300us   4   /* 300 microseconds  */
#define T1ms     5   /* 1   milliseconds  */
#define T3ms     6   /* 3   milliseconds  */
#define T10ms    7   /* 10  milliseconds  */
#define T30ms    8   /* 30  milliseconds  */
#define T100ms   9   /* 100 milliseconds  */
#define T300ms  10   /* 300 milliseconds  */
#define T1s     11   /* 1   seconds       */
#define T3s     12   /* 3   seconds       */
#define T10s    13   /* 10  seconds       */
#define T30s    14   /* 30  seconds       */
#define T100s   15   /* 100 seconds       */
#define T300s   16   /* 300 seconds       */
#define T1000s  17   /* 1000 seconds      */


/*  IBLN Constants                              */
#define NO_SAD   0   /* No secondary addresses  */
#define ALL_SAD -1   /* All secondary addresses */




typedef short Addr4882_t;       /* Address type for 488.2 calls */

/* Macro to create an entry in address list (type Addr4882_t) for 488.2
  functions. Combines primary and secondary addresses into a single address */
#define  MakeAddr(pad, sad)   ((Addr4882_t)(((pad)&0xFF) | ((sad)<<8)))
#define  GetPAD(adr)    ((adr) & 0xff)
#define  GetSAD(adr)    (((adr) >> 8) & 0xff)

/* Miscellaneous                                      */
#define S    0x08    /* parallel poll  bit            */
#define LF   0x0a    /* linefeed character            */
#define NOADDR    (Addr4882_t)((unsigned short)0xFFFF)
#define NULLend   0       /* Send() EOTMODE - Do nothing at the end of a transfer.*/
#define NLend     1       /* Send() EOTMODE - Send NL with EOI after a transfer.  */
#define DABend    02      /* Send() EOTMODE - Send EOI with the last DAB.         */
#define STOPend   0x0100  /* Receive()( termination */
#define EventDTAS 1       /* used by IBEVENT() */
#define EventDCAS 2       /* used by IBEVENT() */


/*  These constants are used with ibconfig() to specify one of the
    configurable options. */
#define  IbcPAD           0x0001    /* Primary Address                   */
#define  IbcSAD           0x0002    /* Secondary Address                 */
#define  IbcTMO           0x0003    /* Timeout                           */
#define  IbcEOT           0x0004    /* Send EOI with last data byte      */
#define  IbcPPC           0x0005    /* Parallel Poll configure           */
#define  IbcREADDR        0x0006    /* Repeat Addressing                 */
#define  IbcAUTOPOLL      0x0007    /* Disable Automatic Serial Polling  */
#define  IbcCICPROT       0x0008    /* Use CIC Protocol                  */
#define  IbcIRQ           0x0009    /* Interrupt level (or 0)            */
#define  IbcSC            0x000A    /* Board is system controller        */
#define  IbcSRE           0x000B    /* Assert SRE for dev calls          */
#define  IbcEOSrd         0x000C    /* Terminate read on EOS             */
#define  IbcEOSwrt        0x000D    /* Send EOI with EOS                 */
#define  IbcEOScmp        0x000E    /* Use 7 or 8-bit compare with EOS   */
#define  IbcEOSchar       0x000F    /* EOS character                     */
#define  IbcPP2           0x0010    /* Use PP Mode 2.                    */
#define  IbcTIMING        0x0011    /* Normal, high or very high timing  */
#define  IbcDMA           0x0012    /* DMA channel (or 0 for none)       */
#define  IbcReadAdjust    0x0013    /* Swap bytes on ibrd                */
#define  IbcWriteAdjust   0x0014    /* Swap bytes on ibwrt               */
#define  IbcEventQueue    0x0015    /* Use event queue                   */
#define  IbcSPollBit      0x0016    /* Serial poll bit used              */
#define  IbcSendLLO       0x0017    /* Automatically send LLO            */
#define  IbcSPollTime     0x0018    /* Serial poll timeout               */
#define  IbcPPollTime     0x0019    /* Parallel poll timeout             */
#define  IbcNoEndBitOnEOS 0x001A    /* Don't set END bit on EOS          */

#define  IbcBNA            0x0200   /* A device's access board.                  */
#define  IbcBaseAddr       0x0201   /* A GPIB board's base I/O address.          */

/* CBI config items - they don't exist in NI software */
#define  IbcBoardType     0x0300    /* Board Type */
#define  IbcChipType      0x0301    /* Chip Type */
#define  IbcSlotNum       0x0302    /* Slot Number */
#define  IbcPCIIndex      0x0303    /* PCI Index */
#define  IbcBaseAdr2      0x0304    /* 2nd base address for PCI board */
#define  IbcUseFIFOs      0x0305    /* When to use FIFO's */

/* These are included for the sake of compatability with NI's library */
#define  IbaPAD           0x0001
#define  IbaSAD           0x0002
#define  IbaTMO           0x0003
#define  IbaEOT           0x0004
#define  IbaPPC           0x0005
#define  IbaREADDR        0x0006
#define  IbaAUTOPOLL      0x0007
#define  IbaCICPROT       0x0008
#define  IbaIRQ           0x0009
#define  IbaSC            0x000A
#define  IbaSRE           0x000B
#define  IbaEOSrd         0x000C
#define  IbaEOSwrt        0x000D
#define  IbaEOScmp        0x000E
#define  IbaEOSchar       0x000F
#define  IbaPP2           0x0010
#define  IbaTIMING        0x0011
#define  IbaDMA           0x0012
#define  IbaReadAdjust    0x0013
#define  IbaWriteAdjust   0x0014
#define  IbaEventQueue    0x0015
#define  IbaSPollBit      0x0016
#define  IbaSendLLO       0x0017
#define  IbaSPollTime     0x0018
#define  IbaPPollTime     0x0019
#define  IbaNoEndBitOnEOS 0x001A

#define  IbaBNA            0x0200
#define  IbaBaseAddr       0x0201

#define  IbaBoardType     0x0300
#define  IbaChipType      0x0301
#define  IbaSlotNum       0x0302
#define  IbaPCIIndex      0x0303
#define  IbaBaseAdr2      0x0304
#define  IbaUseFIFOs      0x0305


/* These bits specify which lines can be monitored by IBLINES */
#define  ValidEOI  (unsigned int) 0x0080
#define  ValidATN  (unsigned int) 0x0040
#define  ValidSRQ  (unsigned int) 0x0020
#define  ValidREN  (unsigned int) 0x0010
#define  ValidIFC  (unsigned int) 0x0008
#define  ValidNRFD (unsigned int) 0x0004
#define  ValidNDAC (unsigned int) 0x0002
#define  ValidDAV  (unsigned int) 0x0001

#define  BusDAV    (unsigned int) 0x0100
#define  BusNDAC   (unsigned int) 0x0200
#define  BusNRFD   (unsigned int) 0x0400
#define  BusIFC    (unsigned int) 0x0800
#define  BusREN    (unsigned int) 0x1000
#define  BusSRQ    (unsigned int) 0x2000
#define  BusATN    (unsigned int) 0x4000
#define  BusEOI    (unsigned int) 0x8000

/* GPIB function prototypes */
#if defined (__cplusplus)
    extern "C"
    {
#endif
    int WINAPI ibask (int board, int item, int *value);
    int WINAPI ibbna  (int device, char FAR *boardname);
    int WINAPI ibbnaA (int device, char FAR *boardname);
    int WINAPI ibcac  (int device, int sync);
    int WINAPI ibclr  (int device);
    int WINAPI ibcmd  (int board, char FAR *cmnd, long bytecount);
    int WINAPI ibcmda  (int board, char FAR *cmnd, long bytecount);
    int WINAPI ibconfig (int board, int item, int value);
    int WINAPI ibdev  (int boardindex, int pad, int sad, int timeout,
                 int eot, int eos);
    int WINAPI ibdma  (int board, int dma);
    int WINAPI ibeos  (int boarddev, int eos);
    int WINAPI ibeot  (int boarddev, int eot);
    int WINAPI ibevent (int board, unsigned short *event);
    int WINAPI ibfind (char FAR *name);
    int WINAPI ibfindA (char FAR *name);
    int WINAPI ibgts  (int board, int handshake);
    int WINAPI ibist  (int board, int statusbit);
    int WINAPI ibinit (int board);
    int WINAPI iblines  (int board, short FAR *clines);
    int WINAPI ibln  (int board, int pad, int sad, short FAR *listenflag);
    int WINAPI ibloc  (int boarddev);
    int WINAPI ibonl  (int boarddev, int online);
    int WINAPI ibpad  (int boarddev, int address);
    int WINAPI ibpct  (int device);
    int WINAPI ibppc  (int board, int command);
    int WINAPI ibptrs (int FAR *ibsta, int FAR *iberr, int FAR *ibcnt, long FAR *ibcntl);
    int WINAPI ibrd   (int boarddev, void FAR *readstr, long bytecount);
    int WINAPI ibrda   (int boarddev, void FAR *readstr,long bytecount);
    int WINAPI ibrdf  (int boarddev, char FAR *filename);
    int WINAPI ibrdfA  (int boarddev, char FAR *filename);
    int WINAPI ibrpp  (int boarddev, char FAR *response);
    int WINAPI ibrsc  (int board, int control);
    int WINAPI ibrsp  (int device, char FAR *serialpollbyte);
    int WINAPI ibrsv  (int boarddev, int statusbyte);
    int WINAPI ibsad  (int boarddev, int address);
    int WINAPI ibsic  (int board);
    int WINAPI ibsre  (int board, int ren);
    int WINAPI ibstop  (int boarddev);
    int WINAPI ibsrq  (void (FAR *func)(void) );
    int WINAPI ibtmo  (int boarddev, int timeout);
    int WINAPI ibtrg  (int device);
    int WINAPI ibwait (int boarddev, int mask);
    int WINAPI ibwrt  (int boarddev, void FAR *writestr, long bytecount);
    int WINAPI ibwrta  (int boarddev, void FAR *writestr, long bytecount);
    int WINAPI ibwrtf (int boarddev, char FAR *filename);
    int WINAPI ibwrtfA (int boarddev, char FAR *filename);

    VOID WINAPI AllSpoll (int board, short FAR *taddrs, short FAR *res);
    VOID WINAPI DevClear (int board, short address);
    VOID WINAPI DevClearList (int board, short FAR *addrlist);
    VOID WINAPI EnableLocal (int board, short FAR *laddrs);
    VOID WINAPI EnableRemote (int board, short FAR *laddrs);
    VOID WINAPI FindLstn (int board, short FAR *pads, short FAR *results, int limit);
    VOID WINAPI FindRQS (int board, short FAR *taddrs, short FAR *dev_stat);
    VOID WINAPI PassControl (int board, short talker);
    VOID WINAPI PPoll (int board, short FAR *res_ptr);
    VOID WINAPI PPollConfig (int board, short laddr, int dataLine, int lineSense);
    VOID WINAPI PPollUnconfig (int board, short FAR *laddrs);
    VOID WINAPI ReadStatusByte (int board, short talker, short FAR *result);
    VOID WINAPI RcvRespMsg (int board, void FAR *buffer, long cnt, int eotMode);
    VOID WINAPI Receive (int board, short talker, void FAR *buffer, long cnt, int eotMode);
    VOID WINAPI ReceiveSetup (int board, short talker);
    VOID WINAPI ResetSys (int board, short FAR *laddrs);
    VOID WINAPI Send (int board, short listener, void FAR *databuf, long datacnt, int eotMode);
    VOID WINAPI SendCmds (int board, void FAR *buf, long cnt);
    VOID WINAPI SendIFC (int board);
    VOID WINAPI SendDataBytes (int board, void FAR *buffer, long cnt, int eot_mode);
    VOID WINAPI SendList (int board, short FAR *listeners, void FAR *databuf, long datacnt, int eotMode);
    VOID WINAPI SendLLO (int board);
    VOID WINAPI SendSetup (int board, short FAR *listen);
    VOID WINAPI SetRWLS (int board, short FAR *laddrs);
    VOID WINAPI TestSRQ (int board, short FAR *result);
    VOID WINAPI TestSys (int board, short FAR *addrs, short FAR *result);
    VOID WINAPI Trigger (int board, short laddrs);
    VOID WINAPI TriggerList (int board, short FAR *laddrs);
    VOID WINAPI WaitSRQ (int board, short FAR *result);

#if defined (__cplusplus)
    }
#endif

