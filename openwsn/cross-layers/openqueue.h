/**
\brief The component which manages the pool of packet buffers.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, August 2010
*/

#ifndef __OPENQUEUE_H
#define __OPENQUEUE_H

#include "openwsn.h"
#include "IEEE802154.h"

//=========================== define ==========================================

#define QUEUELENGTH  10

//=========================== typedef =========================================

typedef struct {
   uint8_t  creator;
   uint8_t  owner;
} debugOpenQueueEntry_t;

//=========================== variables =======================================

//=========================== prototypes ======================================

// admin
          void               openqueue_init();
          bool               openqueue_debugPrint();
// called by any component
__monitor OpenQueueEntry_t*  openqueue_getFreePacketBuffer();
__monitor error_t            openqueue_freePacketBuffer(OpenQueueEntry_t* pkt);
__monitor void               openqueue_removeAllOwnedBy(uint8_t owner);
// called by res
__monitor OpenQueueEntry_t*  openqueue_resGetSentPacket();
__monitor OpenQueueEntry_t*  openqueue_resGetReceivedPacket();
// called by IEEE80215E
__monitor OpenQueueEntry_t*  openqueue_macGetDataPacket(open_addr_t* toNeighbor);
__monitor OpenQueueEntry_t*  openqueue_macGetAdvPacket();

#endif
