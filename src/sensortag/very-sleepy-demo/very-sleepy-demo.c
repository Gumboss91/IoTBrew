/*
 * Copyright (c) 2014, Texas Instruments Incorporated - http://www.ti.com/
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
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/stimer.h"
#include "sys/process.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "dev/button-hal.h"
#include "batmon-sensor.h"
#include "board-peripherals.h"
#include "net/netstack.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/routing/routing.h"
#include "coap-engine.h"
#include "coap.h"

// Added
#include "net/ipv6/simple-udp.h"

#include "ti-lib.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/*---------------------------------------------------------------------------*/
/* Normal mode duration params in seconds */
#define OFF_TIME_DEFAULT 20
#define OFF_TIME_MIN     10
#define OFF_TIME_MAX     86400/* 1 day */
#define RESTART_INTERVAL 60*60
#define DO_RESTART 0
/*---------------------------------------------------------------------------*/
/* Observer notification period params in seconds */
#define ON_TIME_DEFAULT  20
#define ON_TIME_MIN      5
#define ON_TIME_MAX      60 
/*---------------------------------------------------------------------------*/
#define VERY_SLEEPY_MODE_OFF 0
#define VERY_SLEEPY_MODE_ON  1
/*---------------------------------------------------------------------------*/
#define MAC_CAN_BE_TURNED_OFF  0
#define MAC_MUST_STAY_ON       1

#define KEEP_MAC_ON_MIN_PERIOD 10 /* secs */
/*---------------------------------------------------------------------------*/
#define PERIODIC_INTERVAL_OFF         (10*CLOCK_SECOND)
#define PERIODIC_INTERVAL_ON         (1*CLOCK_SECOND)
/*---------------------------------------------------------------------------*/
#define POST_STATUS_BAD           0x80
#define POST_STATUS_HAS_MODE      0x40
#define POST_STATUS_HAS_OFFTIME   0x20
#define POST_STATUS_HAS_ONTIME    0x10
#define POST_STATUS_NONE          0x00
/*---------------------------------------------------------------------------*/
typedef struct sleepy_config_s {
  unsigned long offtime;
  unsigned long ontime;
  uint8_t mode;
} sleepy_config_t;

sleepy_config_t config;
/*---------------------------------------------------------------------------*/
#define STATE_NORMAL           0
#define STATE_NOTIFY_OBSERVERS 1
#define STATE_VERY_SLEEPY      2

#define MAX_MCAST_PAYLOAD_LEN 120
#define MCAST_SINK_UDP_PORT 3001 /* Host byte order */



/*---------------------------------------------------------------------------*/
#if DO_RESTART != 0
static struct stimer st_restart;
#endif
static struct stimer st_ontime;
static struct stimer st_offtime;
static struct stimer st_min_mac_on_duration;
static struct etimer et_periodic;
static process_event_t event_new_config;
static uint8_t state;

//static struct uip_udp_conn * mcast_conn;
static struct simple_udp_connection mcast_conn;
static char udp_mcast_buf[MAX_MCAST_PAYLOAD_LEN];

volatile uint8_t mac_status = 0;
/*---------------------------------------------------------------------------*/
int temp_press=-1; //BMP_280_SENSOR_TYPE_TEMP
int pressure=-1; //BMP_280_SENSOR_TYPE_PRESS

int temp_hum=-1; //HDC_1000_SENSOR_TYPE_TEMP
int humidity=-1; //HDC_1000_SENSOR_TYPE_HUMIDITY

int light=-1; 

int temp_obj=-1;
int temp_ambient=-1;
        
/*---------------------------------------------------------------------------*/
const char *not_supported_msg = "Supported:text/plain,application/json";
/*---------------------------------------------------------------------------*/
PROCESS(very_sleepy_demo_process, "CC13xx/CC26xx very sleepy process");
AUTOSTART_PROCESSES(&very_sleepy_demo_process);
/*---------------------------------------------------------------------------*/
static void
readings_batt_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                     uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;
  int temp=-1;
  int voltage=-1;

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }

  temp = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
  voltage = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);

  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"tmp\":{\"v\":%d,\"u\":\"C\"},"
             "\"volt\":{\"v\":%d,\"u\":\"mV\"}}",
             temp, (voltage * 125) >> 5);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "Temp=%dC, Voltage=%dmV",
             temp, (voltage * 125) >> 5);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
RESOURCE(readings_batt_resource, "title=\"Sensor Batt Readings\";obs",
         readings_batt_get_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/
static void
readings_press_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                     uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }
  
  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"t_press\":{\"v\":%d.%02u,\"u\":\"C\"},"
             "\"press\":{\"v\":%d.%02u,\"u\":\"Pa\"}}",
             temp_press/100, abs(temp_press)%100, pressure/100, abs(pressure)%100);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "Temp_press=%d?, Pressure=%d?",
             temp_press/100, pressure);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
RESOURCE(readings_press_resource, "title=\"Sensor Press Readings\";obs",
         readings_press_get_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/
static void
readings_hum_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                     uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }
  
  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"t_hum\":{\"v\":%d.%02u,\"u\":\"C\"},"
             "\"hum\":{\"v\":%d.%02u,\"u\":\"%%RH\"}}",
             temp_hum/100, abs(temp_hum)%100, humidity/100, abs(humidity)%100);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "Temp_hum=%d, Humidity=%d?",
             temp_hum/100, humidity/100);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
RESOURCE(readings_hum_resource, "title=\"Sensor Hum Readings\";obs",
         readings_hum_get_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/
static void
readings_light_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                     uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }
  
  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"light\":{\"v\":%d.02%u,\"u\":\"Lux\"}}",
             light/100, abs(light)%100);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "Light=%d?",
             light/100);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
RESOURCE(readings_light_resource, "title=\"Sensor Light Readings\";obs",
         readings_light_get_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/
static void
readings_temp_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                     uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  PRINTF("Temp GET Handler\n");

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }

  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"t_obj\":{\"v\":%d.%03u,\"u\":\"C\"},"
             "\"t_amb\":{\"v\":%d.%03u,\"u\":\"C\"}}",
             temp_obj/1000, abs(temp_obj)%1000, temp_ambient/1000, abs(temp_ambient)%1000);

    coap_set_payload(response, buffer, strlen((char *)buffer));
    PRINTF("Temp GET Responded\n");
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "Temp_obj=%d?, Temp_ambient=%d?",
             temp_obj/1000, temp_ambient/1000);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
RESOURCE(readings_temp_resource, "title=\"Sensor Temp Readings\";obs",
         readings_temp_get_handler, NULL, NULL, NULL);
/*---------------------------------------------------------------------------*/
static void
conf_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                 uint16_t preferred_size, int32_t *offset)
{
  unsigned int accept = -1;

  if(request != NULL) {
    coap_get_header_accept(request, &accept);
  }

  if(accept == -1 || accept == APPLICATION_JSON) {
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "{\"config\":{\"mode\":%u,\"ontime\":%lu,\"offtime\":%lu}}",
             config.mode, config.ontime, config.offtime);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else if(accept == TEXT_PLAIN) {
    coap_set_header_content_format(response, TEXT_PLAIN);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "Mode=%u, Ontime=%lusecs, Offtime=%lusecs",
             config.mode, config.ontime, config.offtime);

    coap_set_payload(response, buffer, strlen((char *)buffer));
  } else {
    coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
    coap_set_payload(response, not_supported_msg,
                              strlen(not_supported_msg));
  }
}
/*---------------------------------------------------------------------------*/
static void
conf_post_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer,
                  uint16_t preferred_size, int32_t *offset)
{
  const char *ptr = NULL;
  char tmp_buf[16];
  unsigned long ontime = 0;
  unsigned long offtime = 0;
  uint8_t mode = VERY_SLEEPY_MODE_OFF;
  uint8_t post_status = POST_STATUS_NONE;
  int rv;

  PRINTF("Conf handler\n");

  rv = coap_get_post_variable(request, "mode", &ptr);
  if(rv && rv < 16) {
    memset(tmp_buf, 0, sizeof(tmp_buf));
    memcpy(tmp_buf, ptr, rv);
    rv = atoi(tmp_buf);

    if(rv == 1) {
      mode = VERY_SLEEPY_MODE_ON;
      post_status |= POST_STATUS_HAS_MODE;
    } else if(rv == 0) {
      mode = VERY_SLEEPY_MODE_OFF;
      post_status |= POST_STATUS_HAS_MODE;
    } else {
      post_status = POST_STATUS_BAD;
    }
  }

  rv = coap_get_post_variable(request, "offtime", &ptr);
  if(rv && rv < 16) {
    memset(tmp_buf, 0, sizeof(tmp_buf));
    memcpy(tmp_buf, ptr, rv);
    rv = atoi(tmp_buf);

    offtime = (unsigned long)rv;
    if(offtime < OFF_TIME_MIN || offtime > OFF_TIME_MAX) {
      post_status = POST_STATUS_BAD;
    } else {
      post_status |= POST_STATUS_HAS_OFFTIME;
    }
  }

  rv = coap_get_post_variable(request, "ontime", &ptr);
  PRINTF("Interval rv %d\n", rv);
  if(rv && rv < 16) {
    memset(tmp_buf, 0, sizeof(tmp_buf));
    memcpy(tmp_buf, ptr, rv);
    rv = atoi(tmp_buf);
    ontime = (unsigned long)rv;
    if(ontime < ON_TIME_MIN || ontime > ON_TIME_MAX) {
      post_status = POST_STATUS_BAD;
    } else {
      post_status |= POST_STATUS_HAS_ONTIME;
    }
  }

  if((post_status & POST_STATUS_BAD) == POST_STATUS_BAD ||
     post_status == POST_STATUS_NONE) {
    PRINTF("Unkown Post %d\n", post_status);
    coap_set_status_code(response, BAD_REQUEST_4_00);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE,
             "mode=0|1&offtime=[%u,%u]&ontime=[%u,%u]",
             OFF_TIME_MIN, OFF_TIME_MAX,
             ON_TIME_MIN, ON_TIME_MAX);

    coap_set_payload(response, buffer, strlen((char *)buffer));
    return;
  }

  /* Values are sane. Update the config and notify the process */
  if(post_status & POST_STATUS_HAS_MODE) {
    config.mode = mode;
  }

  if(post_status & POST_STATUS_HAS_OFFTIME) {
    config.offtime = offtime;
  }

  if(post_status & POST_STATUS_HAS_ONTIME) {
    config.ontime = ontime;
  }

  process_post(&very_sleepy_demo_process, event_new_config, NULL);
}
/*---------------------------------------------------------------------------*/
RESOURCE(very_sleepy_conf,
         "title=\"Very sleepy conf: "
         "GET|POST mode=0|1&offtime=<secs>&ontime=<secs>\";rt=\"Control\"",
         conf_get_handler, conf_post_handler, NULL, NULL);
/*---------------------------------------------------------------------------*/
/*
 * If our preferred parent is not NBR_REACHABLE in the ND cache, NUD will send
 * a unicast NS and wait for NA. If NA fails then the neighbour will be removed
 * from the ND cache and the default route will be deleted. To prevent this,
 * keep the MAC on until the parent becomes NBR_REACHABLE. We also keep the MAC
 * on if we are about to do RPL probing.
 *
 * In all cases, the radio will be locked on for KEEP_MAC_ON_MIN_PERIOD secs
 */
static uint8_t
keep_mac_on(void)
{
  uip_ds6_nbr_t *nbr;
  uint8_t rv = MAC_CAN_BE_TURNED_OFF;

  if(!stimer_expired(&st_min_mac_on_duration)) {
    return MAC_MUST_STAY_ON;
  }

#if RPL_WITH_PROBING
  /* Determine if we are about to send a RPL probe */
  if(CLOCK_LT(etimer_expiration_time(
                &rpl_get_default_instance()->dag.probing_timer.etimer),
              (clock_time() + PERIODIC_INTERVAL))) {
    rv = MAC_MUST_STAY_ON;
  }
#endif

  /* It's OK to pass a NULL pointer, the callee checks and returns NULL */
  nbr = uip_ds6_nbr_lookup(uip_ds6_defrt_choose());

  if(nbr == NULL) {
    /* We don't have a default route, or it's not reachable (NUD likely). */
    PRINTF("No default route\n");
    rv = MAC_MUST_STAY_ON;
  } else {
    if(nbr->state != NBR_REACHABLE) {
      PRINTF("NBR not rechable\n");
      rv = MAC_MUST_STAY_ON;
    }
  }

  if(rv == MAC_MUST_STAY_ON && stimer_expired(&st_min_mac_on_duration)) {
    stimer_set(&st_min_mac_on_duration, KEEP_MAC_ON_MIN_PERIOD);
  }

  return rv;
}
/*---------------------------------------------------------------------------*/
static void
get_bmp_reading()
{
  pressure = bmp_280_sensor.value(BMP_280_SENSOR_TYPE_PRESS);
  temp_press = bmp_280_sensor.value(BMP_280_SENSOR_TYPE_TEMP);
  SENSORS_DEACTIVATE(bmp_280_sensor);
}
/*---------------------------------------------------------------------------*/
static void
get_tmp_reading()
{
  tmp_007_sensor.value(TMP_007_SENSOR_TYPE_ALL);
  temp_ambient = tmp_007_sensor.value(TMP_007_SENSOR_TYPE_AMBIENT);
  temp_obj = tmp_007_sensor.value(TMP_007_SENSOR_TYPE_OBJECT);
  
  SENSORS_DEACTIVATE(tmp_007_sensor);
}
/*---------------------------------------------------------------------------*/
static void
get_hdc_reading()
{
  temp_hum = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_TEMP);
  humidity = hdc_1000_sensor.value(HDC_1000_SENSOR_TYPE_HUMID);
}
/*---------------------------------------------------------------------------*/
static void
get_light_reading()
{
  light = opt_3001_sensor.value(0);
  /* The OPT will turn itself off, so we don't need to call its DEACTIVATE */
}
/*---------------------------------------------------------------------------*/

static inline void sendUdp(uint8_t status) {
  uip_ipaddr_t ipaddr;
  
  uip_ip6addr(&ipaddr, 0xBBBB,0,0,0,0x6d26,0xb1b6,0x5142,0x21be);
  SENSORS_ACTIVATE(bmp_280_sensor);
  SENSORS_ACTIVATE(hdc_1000_sensor);
  SENSORS_ACTIVATE(opt_3001_sensor);
  SENSORS_ACTIVATE(tmp_007_sensor);
  //uip_udp_packet_send(mcast_conn, udp_mcast_buf, sizeof(udp_mcast_buf));
  simple_udp_sendto(&mcast_conn, udp_mcast_buf, sizeof(udp_mcast_buf), &ipaddr);
  PRINTF("UDP send %d\n", status);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(very_sleepy_demo_process, ev, data)
{
  uint8_t mac_keep_on;
  uint8_t timer_status;
  #define TIMER_ONTIME_FIRED 0x01
  #define TIMER_OFFTIME_FIRED 0x02

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(batmon_sensor);

  config.mode = VERY_SLEEPY_MODE_ON;
  config.ontime = ON_TIME_DEFAULT;
  config.offtime = OFF_TIME_DEFAULT;
  
  bmp_280_sensor.configure(SENSORS_HW_INIT, 0);
  hdc_1000_sensor.configure(SENSORS_HW_INIT, 0);
  opt_3001_sensor.configure(SENSORS_HW_INIT, 0);
  tmp_007_sensor.configure(SENSORS_HW_INIT, 0);

  state = STATE_NORMAL;

  event_new_config = process_alloc_event();

  SENSORS_ACTIVATE(bmp_280_sensor);
  SENSORS_ACTIVATE(hdc_1000_sensor);
  SENSORS_ACTIVATE(opt_3001_sensor);
  SENSORS_ACTIVATE(tmp_007_sensor);

  /*readings_batt_resource.flags += IS_OBSERVABLE;
  readings_press_resource.flags += IS_OBSERVABLE;
  readings_hum_resource.flags += IS_OBSERVABLE;
  readings_light_resource.flags += IS_OBSERVABLE;
  readings_temp_resource.flags += IS_OBSERVABLE;*/
  
  coap_activate_resource(&readings_hum_resource, "sen/readings/hum");
  coap_activate_resource(&readings_press_resource, "sen/readings/press");
  coap_activate_resource(&readings_light_resource, "sen/readings/light");
  coap_activate_resource(&readings_batt_resource, "sen/readings/batt");
  coap_activate_resource(&readings_temp_resource, "sen/readings/temp");
  coap_activate_resource(&very_sleepy_conf, "very_sleepy_config");

  PRINTF("Very Sleepy Demo Process\n");

  state = STATE_NORMAL;

  etimer_set(&et_periodic, PERIODIC_INTERVAL_OFF);
  
  memset(udp_mcast_buf, 0x32, MAX_MCAST_PAYLOAD_LEN/2);
  
  //mcast_conn = udp_new(&ipaddr, UIP_HTONS(MCAST_SINK_UDP_PORT), NULL);  
  simple_udp_register(&mcast_conn, MCAST_SINK_UDP_PORT, NULL,
                      MCAST_SINK_UDP_PORT, NULL);
  //udp_bind(mcast_conn, UIP_HTONS(MCAST_SINK_UDP_PORT));

  PRINTF("Pre-While\n");
  stimer_set(&st_offtime, config.offtime);
  #if DO_RESTART != 0
  stimer_set(&st_restart, RESTART_INTERVAL);
  #endif
  mac_status = 0;

  while(1) {

    //PRINTF("sleep\n");
    PROCESS_YIELD();
    //PRINTF("unsleep %d, %d, %d, %d\n", ev, sensors_event, PROCESS_EVENT_TIMER, event_new_config);

    #if DO_RESTART != 0
    if(stimer_expired(&st_restart)) {
      watchdog_reboot();
    }
    #endif

    if(ev == event_new_config) {
      stimer_set(&st_offtime, config.offtime);
    }
    
    if(ev == sensors_event && data == &bmp_280_sensor) {
        get_bmp_reading();
    } else if(ev == sensors_event && data == &opt_3001_sensor) {
        get_light_reading();
    } else if(ev == sensors_event && data == &hdc_1000_sensor) {
        get_hdc_reading();
    } else if(ev == sensors_event && data == &tmp_007_sensor) {
        get_tmp_reading();
    }

    if((ev == PROCESS_EVENT_TIMER && data == &et_periodic) ||
       (ev == event_new_config)) {
      /*
       * Determine if the stack is about to do essential network maintenance
       * and, if so, keep the MAC layer on
       */
      mac_keep_on = keep_mac_on();   

      /*
       * Next, switch between normal and very sleepy mode depending on config,
       * send notifications to observers as required.
       */
      if(state == STATE_NOTIFY_OBSERVERS) {
        /*coap_notify_observers(&readings_batt_resource);
        coap_notify_observers(&readings_hum_resource);
        coap_notify_observers(&readings_press_resource);
        coap_notify_observers(&readings_light_resource);
        coap_notify_observers(&readings_temp_resource);*/

        sendUdp(0);
        
        state = STATE_NORMAL;
        
      }

      // Keep timer looping
      timer_status = 0;
      if(stimer_expired(&st_offtime)) {
        stimer_set(&st_ontime, config.ontime);
        stimer_set(&st_offtime, 9999999); // never expire
        timer_status |= TIMER_OFFTIME_FIRED;
        PRINTF("OFFTIME fired\n");
      }

      if(stimer_expired(&st_ontime)) {
        stimer_set(&st_ontime, 9999999); // never expire
        stimer_set(&st_offtime, config.offtime);
        timer_status |= TIMER_ONTIME_FIRED;
        PRINTF("ONTIME fired\n");
      }

      //ONTime passed
      if(timer_status&TIMER_ONTIME_FIRED && config.mode == 1) {
        // Switch to sleepy
        PRINTF("Send to sleep\n");
        state = STATE_VERY_SLEEPY;
      }

      //OFFTime passed
      if(timer_status&TIMER_OFFTIME_FIRED) {
        // Switch to normal
        PRINTF("Wakeup\n");
        state = STATE_NOTIFY_OBSERVERS;
      }

      if(mac_keep_on == MAC_CAN_BE_TURNED_OFF && state == STATE_VERY_SLEEPY) {
        PRINTF("Try OFF\n");
        if(mac_status) {
          leds_off(LEDS_GREEN);
          NETSTACK_MAC.off();
          mac_status = 0;
          PRINTF("MAC off %d\n", mac_keep_on);
        }
      } else {
        PRINTF("Try ON\n");
        if(!mac_status) {
          leds_on(LEDS_GREEN);
          NETSTACK_MAC.on();
          PRINTF("MAC on - SEND %d\n", mac_keep_on);
          mac_status = 1;
        }
      }

      /* Schedule next pass */
      if(mac_status) {
        etimer_set(&et_periodic, PERIODIC_INTERVAL_ON);
      } else {
        etimer_set(&et_periodic, PERIODIC_INTERVAL_OFF);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
