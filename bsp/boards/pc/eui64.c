/**
\brief PC-specific definition of the "eui64" bsp module.

\author Thomas Watteyne <watteyne@eecs.berkeley.edu>, April 2012.
*/

#include "eui64.h"
#include "opensim_proto.h"

//=========================== defines =========================================

//=========================== variables =======================================

//=========================== prototypes ======================================

//=========================== public ==========================================

void eui64_get(uint8_t* addressToWrite) {
   opensim_repl_eui64_get_t replparams;
   
   // send request to server and get reply
   opensim_client_sendAndWaitForAck(OPENSIM_CMD_bsp_timer_reset,
                                    0,
                                    0,
                                    &replparams,
                                    sizeof(opensim_repl_eui64_get_t));
}

//=========================== private =========================================