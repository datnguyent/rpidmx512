/**
 * @file rdm_send.c
 *
 */
/* Copyright (C) 2015 by Arjan van Vught <pm @ http://www.raspberrypi.org/forum/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <string.h>

#include "../include/rdm_device_info.h"
#include "bcm2835.h"
#include "bcm2835_pl011.h"
#include "dmx.h"
#include "rdm.h"
#include "rdm_e120.h"

// TODO move for util.h
typedef enum {
	FALSE = 0,
	TRUE = 1
} _boolean;

extern uint8_t rdm_data[512];

static void rdm_send_no_break(const uint8_t *data, const uint16_t data_length)
{
	BCM2835_PL011->LCRH = PL011_LCRH_WLEN8 | PL011_LCRH_STP2;

	uint16_t i = 0;
	for (i = 0; i < data_length; i++)
	{
		while (1)
		{
			if ((BCM2835_PL011->FR & 0x20) == 0)
				break;
		}
		BCM2835_PL011->DR = data[i];
	}
	while (1)
	{
		if ((BCM2835_PL011->FR & 0x20) == 0)
			break;
	}
}

void rdm_send_discovery_msg(const uint8_t *data, const uint16_t data_length)
{
	uint64_t delay = bcm2835_st_read() - rdm_data_receive_end_get();

	if (delay < 180)
	{
		udelay(180 - delay);
	}

	dmx_port_direction_set(DMX_PORT_DIRECTION_OUTP, FALSE);
	rdm_send_no_break(data, data_length);
	dmx_port_direction_set(DMX_PORT_DIRECTION_INP, TRUE);
}

static void rdm_send_respond_message(uint8_t is_ack, uint16_t reason)
{
	uint16_t rdm_checksum = 0;

	struct _rdm_command *rdm_response = (struct _rdm_command *)rdm_data;

	if(is_ack == TRUE)
	{
		rdm_response->port_id = E120_RESPONSE_TYPE_ACK;
	} else
	{
		rdm_response->message_length = RDM_MESSAGE_MINIMUM_SIZE + 2;
		rdm_response->port_id = E120_RESPONSE_TYPE_NACK_REASON;
		rdm_response->param_data_length = 2;
		rdm_response->param_data[0] = (uint8_t)(reason >> 8);
		rdm_response->param_data[1] = (uint8_t)reason;
	}

	const uint8_t *uid_device = rdm_device_info_uuid_get();

	memcpy(rdm_response->destination_uid, rdm_response->source_uid, UID_SIZE);
	memcpy(rdm_response->source_uid, uid_device, UID_SIZE);

	rdm_response->command_class++;

	uint8_t i = 0;
	for (i = 0; i < rdm_response->message_length; i++)
	{
		rdm_checksum += rdm_data[i];
	}

	rdm_data[i++] = rdm_checksum >> 8;
	rdm_data[i] = rdm_checksum & 0XFF;;

	uint64_t delay = bcm2835_st_read() - rdm_data_receive_end_get();

	if (delay < 180)
	{
		udelay(180 - delay);
	}

	dmx_port_direction_set(DMX_PORT_DIRECTION_OUTP, FALSE);
	dmx_data_send(rdm_data, rdm_response->message_length + RDM_MESSAGE_CHECKSUM_SIZE);
	dmx_port_direction_set(DMX_PORT_DIRECTION_INP, TRUE);
}

void rdm_send_respond_message_ack()
{
	rdm_send_respond_message(TRUE, 0);
}

void rdm_send_respond_message_nack(const uint16_t reason)
{
	rdm_send_respond_message(FALSE, reason);
}
