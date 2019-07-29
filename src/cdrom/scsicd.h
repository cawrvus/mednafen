#ifndef __PCFX_SCSICD_H
#define __PCFX_SCSICD_H

namespace Mednafen
{

typedef int32 scsicd_timestamp_t;

typedef struct
{
 // Data bus(FIXME: we should have a variable for the target and the initiator, and OR them together to be truly accurate).
 uint8 DB;

 uint32 signals;

 // Signals under our(the "target") control.
 //bool BSY, MSG, CD, REQ, IO;

 // Signals under the control of the initiator(not us!)
 //bool kingACK, kingRST, kingSEL, kingATN;
} scsicd_bus_t;

extern scsicd_bus_t cd_bus; // Don't access this structure directly by name outside of scsicd.c, but use the macros below.

// Signals under our(the "target") control.
#define SCSICD_IO_mask	0x001
#define SCSICD_CD_mask	0x002
#define SCSICD_MSG_mask	0x004
#define SCSICD_REQ_mask	0x008
#define SCSICD_BSY_mask	0x010

// Signals under the control of the initiator(not us!)
#define SCSICD_kingRST_mask	0x020
#define SCSICD_kingACK_mask	0x040
#define SCSICD_kingATN_mask	0x080
#define SCSICD_kingSEL_mask	0x100

#define BSY_signal ((bool)(cd_bus.signals & SCSICD_BSY_mask))
#define ACK_signal ((bool)(cd_bus.signals & SCSICD_kingACK_mask))
#define RST_signal ((bool)(cd_bus.signals & SCSICD_kingRST_mask))
#define MSG_signal ((bool)(cd_bus.signals & SCSICD_MSG_mask))
#define SEL_signal ((bool)(cd_bus.signals & SCSICD_kingSEL_mask))
#define REQ_signal ((bool)(cd_bus.signals & SCSICD_REQ_mask))
#define IO_signal ((bool)(cd_bus.signals & SCSICD_IO_mask))
#define CD_signal ((bool)(cd_bus.signals & SCSICD_CD_mask))
#define ATN_signal ((bool)(cd_bus.signals & SCSICD_kingATN_mask))

#define DB_signal ((uint8)cd_bus.DB)

#define SCSICD_GetDB() DB_signal
#define SCSICD_GetBSY() BSY_signal
#define SCSICD_GetIO() IO_signal
#define SCSICD_GetCD() CD_signal
#define SCSICD_GetMSG() MSG_signal
#define SCSICD_GetREQ() REQ_signal

// Should we phase out getting these initiator-driven signals like this(the initiator really should keep track of them itself)?
#define SCSICD_GetACK() ACK_signal
#define SCSICD_GetRST() RST_signal
#define SCSICD_GetSEL() SEL_signal
#define SCSICD_GetATN() ATN_signal

void SCSICD_Power(scsicd_timestamp_t system_timestamp);
void SCSICD_SetDB(uint8 data);

// These SCSICD_Set* functions are kind of misnomers, at least in comparison to the SCSICD_Get* functions...
// They will set/clear the bits corresponding to the KING's side of the bus.
void SCSICD_SetACK(bool set);
void SCSICD_SetSEL(bool set);
void SCSICD_SetRST(bool set);
void SCSICD_SetATN(bool set);

uint32 SCSICD_Run(scsicd_timestamp_t);
void SCSICD_ResetTS(uint32 ts_base);

enum
{
 SCSICD_PCE = 1,
 SCSICD_PCFX
};

enum
{
 SCSICD_IRQ_DATA_TRANSFER_DONE = 1,
 SCSICD_IRQ_DATA_TRANSFER_READY,
 SCSICD_IRQ_MAGICAL_REQ,
};

void SCSICD_GetCDDAValues(int16 &left, int16 &right);

void SCSICD_SetLog(void (*logfunc)(const char *, const char *, ...));

void SCSICD_Init(int type, int CDDATimeDiv, int32* left_hrbuf, int32* right_hrbuf, uint32 TransferRate, uint32 SystemClock, void (*IRQFunc)(int), void (*SSCFunc)(uint8, int)) MDFN_COLD;
void SCSICD_Close(void) MDFN_COLD;

void SCSICD_SetTransferRate(uint32 TransferRate);
void SCSICD_SetCDDAVolume(double left, double right);
void SCSICD_StateAction(StateMem *sm, const unsigned load, const bool data_only, const char *sname);

void SCSICD_SetDisc(bool tray_open, CDInterface* cdif, bool no_emu_side_effects = false);

}
#endif
