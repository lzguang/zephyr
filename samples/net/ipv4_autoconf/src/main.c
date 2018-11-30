/* Networking DHCPv4 client */

/*
 * Copyright (c) 2017 ARM Ltd.
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(net_ipv4_autoconf_sample, LOG_LEVEL_DBG);

#include <zephyr.h>
#include <linker/sections.h>
#include <errno.h>
#include <stdio.h>

#include <net/net_if.h>
#include <net/net_core.h>
#include <net/net_context.h>
#include <net/net_mgmt.h>

static struct net_mgmt_event_callback mgmt_cb;

static void handler(struct net_mgmt_event_callback *cb,
		    u32_t mgmt_event,
		    struct net_if *iface)
{
	int i = 0;
	struct net_if_config *cfg;

	cfg = net_if_get_config(iface);
	if (!cfg) {
		return;
	}

	if (mgmt_event != NET_EVENT_IPV4_ADDR_ADD) {
		return;
	}

	for (i = 0; i < NET_IF_MAX_IPV4_ADDR; i++) {
		char buf[NET_IPV4_ADDR_LEN];

		if (cfg->ip.ipv4->unicast[i].addr_type != NET_ADDR_AUTOCONF) {
			continue;
		}

		NET_INFO("Your address: %s",
			 log_strdup(net_addr_ntop(AF_INET,
				    &cfg->ip.ipv4->unicast[i].address.in_addr,
				    buf, sizeof(buf))));
	}
}

void main(void)
{
	NET_INFO("Run ipv4 autoconf client");

	net_mgmt_init_event_callback(&mgmt_cb, handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&mgmt_cb);
}
