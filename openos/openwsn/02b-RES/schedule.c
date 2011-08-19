/**
\brief Manages the IEEE802.15.4e schedule

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, August 2011
*/

#include "openwsn.h"
#include "schedule.h"
#include "openserial.h"
#include "idmanager.h"

//=========================== variables =======================================

typedef struct {
   cellUsageInformation_t schedule[SCHEDULELENGTH];
   slotOffset_t           debugPrintRow;
} schedule_vars_t;

schedule_vars_t schedule_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

void schedule_init() {
   uint8_t slotCounter;
   // for debug print
   schedule_vars.debugPrintRow                             = 0;
   //all slots OFF
   for (slotCounter=0;slotCounter<SCHEDULELENGTH;slotCounter++){
      schedule_vars.schedule[slotCounter].type             = CELLTYPE_OFF;
      schedule_vars.schedule[slotCounter].channelOffset    = 0;
      schedule_vars.schedule[slotCounter].neighbor.type    = ADDR_NONE;
      schedule_vars.schedule[slotCounter].numUsed          = 0;
      schedule_vars.schedule[slotCounter].numTxACK         = 0;
      schedule_vars.schedule[slotCounter].timestamp        = 0;
   }
   //slot 0 is advertisement slot
   schedule_vars.schedule[0].type                          = CELLTYPE_ADV;
   //slot 1 TX@MASTER, RX@SLAVE
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[1].type                       = CELLTYPE_TX;
   } else {
      schedule_vars.schedule[1].type                       = CELLTYPE_RX;
   }
   schedule_vars.schedule[1].neighbor.type                 = ADDR_64B;
   schedule_vars.schedule[1].neighbor.addr_64b[0]          = 0x14;
   schedule_vars.schedule[1].neighbor.addr_64b[1]          = 0x15;
   schedule_vars.schedule[1].neighbor.addr_64b[2]          = 0x92;
   schedule_vars.schedule[1].neighbor.addr_64b[3]          = 0x09;
   schedule_vars.schedule[1].neighbor.addr_64b[4]          = 0x02;
   schedule_vars.schedule[1].neighbor.addr_64b[5]          = 0x2c;
   schedule_vars.schedule[1].neighbor.addr_64b[6]          = 0x00;
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[1].neighbor.addr_64b[7]       = DEBUG_MOTEID_SLAVE;
   } else {
      schedule_vars.schedule[1].neighbor.addr_64b[7]       = DEBUG_MOTEID_MASTER;
   }
   //slot 2 RX@MASTER, TX@SLAVE
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[2].type                       = CELLTYPE_RX;
   } else {
      schedule_vars.schedule[2].type                       = CELLTYPE_TX;
   }
   schedule_vars.schedule[2].neighbor.type                 = ADDR_64B;
   schedule_vars.schedule[2].neighbor.addr_64b[0]          = 0x14;
   schedule_vars.schedule[2].neighbor.addr_64b[1]          = 0x15;
   schedule_vars.schedule[2].neighbor.addr_64b[2]          = 0x92;
   schedule_vars.schedule[2].neighbor.addr_64b[3]          = 0x09;
   schedule_vars.schedule[2].neighbor.addr_64b[4]          = 0x02;
   schedule_vars.schedule[2].neighbor.addr_64b[5]          = 0x2c;
   schedule_vars.schedule[2].neighbor.addr_64b[6]          = 0x00;
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[2].neighbor.addr_64b[7]       = DEBUG_MOTEID_SLAVE;
   } else {
      schedule_vars.schedule[2].neighbor.addr_64b[7]       = DEBUG_MOTEID_MASTER;
   }
   //slot 3 TX@MASTER, OFF@SLAVE
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[3].type                       = CELLTYPE_TX;
      schedule_vars.schedule[3].neighbor.type              = ADDR_64B;
      schedule_vars.schedule[3].neighbor.addr_64b[0]       = 0x14;
      schedule_vars.schedule[3].neighbor.addr_64b[1]       = 0x15;
      schedule_vars.schedule[3].neighbor.addr_64b[2]       = 0x92;
      schedule_vars.schedule[3].neighbor.addr_64b[3]       = 0x09;
      schedule_vars.schedule[3].neighbor.addr_64b[4]       = 0x02;
      schedule_vars.schedule[3].neighbor.addr_64b[5]       = 0x2c;
      schedule_vars.schedule[3].neighbor.addr_64b[6]       = 0x00;
      schedule_vars.schedule[3].neighbor.addr_64b[7]       = DEBUG_MOTEID_SLAVE;
   } else {
      schedule_vars.schedule[3].type                       = CELLTYPE_OFF;
   }
   //slot 4 RX@MASTER, OFF@SLAVE
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[4].type                       = CELLTYPE_RX;
      schedule_vars.schedule[4].neighbor.type              = ADDR_64B;
      schedule_vars.schedule[4].neighbor.addr_64b[0]       = 0x14;
      schedule_vars.schedule[4].neighbor.addr_64b[1]       = 0x15;
      schedule_vars.schedule[4].neighbor.addr_64b[2]       = 0x92;
      schedule_vars.schedule[4].neighbor.addr_64b[3]       = 0x09;
      schedule_vars.schedule[4].neighbor.addr_64b[4]       = 0x02;
      schedule_vars.schedule[4].neighbor.addr_64b[5]       = 0x2c;
      schedule_vars.schedule[4].neighbor.addr_64b[6]       = 0x00;
      schedule_vars.schedule[4].neighbor.addr_64b[7]       = DEBUG_MOTEID_SLAVE;
   } else {
      schedule_vars.schedule[4].type                       = CELLTYPE_OFF;
   }
   //slot 5 is transmit slot to neighbor 0xffffffffffffffff
   if (idmanager_getMyID(ADDR_16B)->addr_16b[1]==DEBUG_MOTEID_MASTER) {
      schedule_vars.schedule[5].type                       = CELLTYPE_TX;
   } else {
      schedule_vars.schedule[5].type                       = CELLTYPE_OFF;//poipoi
   }
   schedule_vars.schedule[5].neighbor.type                 = ADDR_64B;
   schedule_vars.schedule[5].neighbor.addr_64b[0]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[1]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[2]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[3]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[4]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[5]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[6]          = 0xff;
   schedule_vars.schedule[5].neighbor.addr_64b[7]          = 0xff;
   //slot 6 is serialRx
   schedule_vars.schedule[6].type                          = CELLTYPE_SERIALRX;
}

__monitor cellType_t schedule_getType(asn_t asn_param) {
   uint16_t slotOffset;
   slotOffset = asn_param%SCHEDULELENGTH;
   return schedule_vars.schedule[slotOffset].type;
}

__monitor channelOffset_t schedule_getChannelOffset(asn_t asn_param) {
   uint16_t slotOffset;
   slotOffset = asn_param%SCHEDULELENGTH;
   return schedule_vars.schedule[slotOffset].channelOffset;
}

__monitor void schedule_getNeighbor(asn_t asn_param, open_addr_t* addrToWrite) {
   uint16_t slotOffset;
   slotOffset = asn_param%SCHEDULELENGTH;
   memcpy(addrToWrite,&schedule_vars.schedule[slotOffset].neighbor,sizeof(open_addr_t));
}

__monitor void schedule_indicateUse(asn_t asn, bool ack){
   uint16_t slotOffset;
   
   slotOffset = asn%SCHEDULELENGTH;
   if (schedule_vars.schedule[slotOffset].numUsed==0xFF) {
      schedule_vars.schedule[slotOffset].numUsed/=2;
      schedule_vars.schedule[slotOffset].numTxACK/=2;
   }
   schedule_vars.schedule[slotOffset].numUsed++;
   if (ack==TRUE) {
      schedule_vars.schedule[slotOffset].numTxACK++;
   }
   schedule_vars.schedule[slotOffset].timestamp=asn;
}

bool schedule_debugPrint() {
   debugCellUsageInformation_t temp;
   schedule_vars.debugPrintRow = (schedule_vars.debugPrintRow+1)%SCHEDULELENGTH;
   temp.row        = schedule_vars.debugPrintRow;
   temp.cellUsage  = schedule_vars.schedule[schedule_vars.debugPrintRow];
   openserial_printStatus(STATUS_SCHEDULE_SCHEDULE,
                          (uint8_t*)&temp,
                          sizeof(debugCellUsageInformation_t));
   return TRUE;
}

//=========================== private =========================================