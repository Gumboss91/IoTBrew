/*
 * Copyright (c) 2015, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define IEEE802154_CONF_PANID            0xABCD
#define IEEE802154_CONF_DEFAULT_CHANNEL      26
/*---------------------------------------------------------------------------*/
/* Enable the ROM bootloader */
#define CCXXWARE_CONF_ROM_BOOTLOADER_ENABLE   1
/*---------------------------------------------------------------------------*/
/* For very sleepy operation */
#define RF_BLE_CONF_ENABLED                   0
#define UIP_CONF_TCP                          0
#define RPL_CONF_LEAF_ONLY                    1

#define COAP_MAX_CHUNK_SIZE 64*8
#define COAP_MAX_OPEN_TRANSACTIONS 6

//#define LOG_CONF_LEVEL_COAP                        LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_VERYSLEEPY 1

/*
 * We'll fail without RPL probing, so turn it on explicitly even though it's
 * on by default
 */
#define RPL_CONF_WITH_PROBING                 1
// With only 2 chances in one minute and 30min downtime, this default logic is insane
#define RPL_CONF_WITH_DAO_ACK                 0

//the delay after which a node actually leaves a network, by default, 5 minutes.
#define RPL_CONF_DELAY_BEFORE_LEAVING (2 * 60 * CLOCK_SECOND)
/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
