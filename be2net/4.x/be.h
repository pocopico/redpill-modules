/*
 * This file is part of the Linux NIC driver for Emulex networking products.
 *
 * Copyright (C) 2005-2016 Broadcom. All rights reserved.
 *
 * EMULEX and SLI are trademarks of Emulex.
 * www.emulex.com
 * linux-drivers@emulex.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE
 * EXTENT THAT SUCH DISCLAIMERS ARE HELD TO BE LEGALLY INVALID.
 * See the GNU General Public License for more details, a copy of which
 * can be found in the file COPYING included with this package
 */

#ifndef BE_H
#define BE_H

#include <linux/pci.h>
#include <linux/etherdevice.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <net/tcp.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <linux/if_vlan.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/firmware.h>
#include <linux/jhash.h>
#include <linux/ethtool.h>
#include <linux/cpumask.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>

#ifdef NDO_GET_STATS64_defined
#include <linux/u64_stats_sync.h>
#endif

#include <linux/aer.h>
#include "be_compat.h"
#include "be_hw.h"
#include "be_roce.h"
#include "version.h"

#define	DRV_VER			STR_BE_MAJOR "." STR_BE_MINOR "."\
				STR_BE_BUILD "." STR_BE_BRANCH
#ifdef BE2NET_DEFINED
#define DRV_NAME		"be2net"
#endif
#ifdef LPNIC_DEFINED
#define DRV_NAME		"lpnic"
#endif
#define BE_NAME			"Emulex BladeEngine2"
#define BE3_NAME		"Emulex BladeEngine3"
#define OC_NAME			"Emulex OneConnect"
#define OC_NAME_BE		OC_NAME "(be3)"
#define OC_NAME_LANCER		OC_NAME "(Lancer)"
#define OC_NAME_SH		OC_NAME "(Skyhawk)"
#define OC_NAME_CORSAIR         OC_NAME "(Corsair)"
#define DRV_DESC		"Emulex OneConnect NIC Driver"

#define	BE_VENDOR_ID		0x19a2
#define EMULEX_VENDOR_ID	0x10df
#define BE_DEVICE_ID1		0x211
#define BE_DEVICE_ID2		0x221
#define OC_DEVICE_ID1		0x700	/* Device Id for BE2 cards */
#define OC_DEVICE_ID2		0x710	/* Device Id for BE3 cards */
#define OC_DEVICE_ID3		0xe220	/* Device id for Lancer cards */
#define OC_DEVICE_ID4		0xe228  /* Device id for VF in Lancer */
#define OC_DEVICE_ID5		0x720   /* Device id for SkyHawk cards */
#define OC_DEVICE_ID6		0x728   /* Device id for VF in SkyHawk */

#define OC_SUBSYS_DEVICE_ID1	0xE602
#define OC_SUBSYS_DEVICE_ID2	0xE642
#define OC_SUBSYS_DEVICE_ID3	0xE612
#define OC_SUBSYS_DEVICE_ID4	0xE652

/* Number of bytes of an RX frame that are copied to skb->data */
#define BE_HDR_LEN		((u16) 64)
/* allocate extra space to allow tunneling
 * decapsulation without head reallocation
 */
#define BE_RX_SKB_ALLOC_SIZE	256
#define BE_MAX_JUMBO_FRAME_SIZE	9018
#define BE_MIN_MTU		256
#define BE_MAX_MTU		(BE_MAX_JUMBO_FRAME_SIZE -	\
				 (ETH_HLEN + ETH_FCS_LEN))

/* Accommodate for QnQ configurations where VLAN insertion is enabled in HW */
#define BE_MAX_GSO_SIZE		(65535 - 2 * VLAN_HLEN)

#define BE_NUM_VLANS_SUPPORTED	64
#define BE_MAX_EQD		128u
#define	BE_MAX_TX_FRAG_COUNT	30

#define EVNT_Q_LEN		1024
#define TX_Q_LEN		2048
#define TX_CQ_LEN		1024
#define RX_Q_LEN		1024	/* Does not support any other value */
#define RX_CQ_LEN		1024
#define MCC_Q_LEN		128	/* total size not to exceed 8 pages */
#define MCC_CQ_LEN		256

#define BE2_MAX_RSS_QS		4
#define BE3_MAX_RSS_QS		16
#define BE3_MAX_TX_QS		16
#define BE3_MAX_EVT_QS		16
#define BE3_SRIOV_MAX_EVT_QS	8
#define	SH_VF_MAX_NIC_EQS	3	/* Skyhawk VFs can have a max of 4 EQs
					 * and at least 1 is granted to either
					 * SURF/DPDK
					 */

#define MAX_PORT_RSS_TABLES	15
#define MAX_NIC_FUNCS		16
#define MAX_RX_QS		32
#define MAX_EVT_QS		32

#ifdef MQ_TX
#define MAX_TX_QS		32
#else
#define MAX_TX_QS		1
#endif

#define MAX_MSIX_VECTORS	32
#define MIN_MSIX_VECTORS	1

#define BE_NAPI_WEIGHT		64
#define MAX_RX_POST		BE_NAPI_WEIGHT /* Frags posted at a time */
#define RX_FRAGS_REFILL_WM	(RX_Q_LEN - MAX_RX_POST)
#define MAX_NUM_POST_ERX_DB	255u

#define BE_MAX_LRO_DESCRIPTORS  16
#define BE_MAX_FRAGS_PER_FRAME  (min((u32) 16, (u32) MAX_SKB_FRAGS))

#define MAX_VFS			30

#define FW_VER_LEN		32

#define CNTL_SERIAL_NUM_WORDS	8  /* Controller serial number words */
#define CNTL_SERIAL_NUM_WORD_SZ	(sizeof(u16)) /* Byte-sz of serial num word */

#define	RSS_INDIR_TABLE_LEN	128
#define RSS_HASH_KEY_LEN	40

#define BE_UNKNOWN_PHY_STATE	0xFF

struct be_dma_mem {
	void *va;
	dma_addr_t dma;
	u32 size;
};

struct be_queue_info {
	u32 len;
	u32 entry_size;	/* Size of an element in the queue */
	u32 tail, head;
	atomic_t used;	/* Number of valid elements in the queue */
	u32 id;
	struct be_dma_mem dma_mem;
	bool created;
};

static inline u32 MODULO(u32 val, u32 limit)
{
	BUG_ON(limit & (limit - 1));
	return val & (limit - 1);
}

static inline void index_adv(u32 *index, u32 val, u32 limit)
{
	*index = MODULO((*index + val), limit);
}

static inline void index_inc(u32 *index, u32 limit)
{
	*index = MODULO((*index + 1), limit);
}

static inline void index_dec(u32 *index, u32 limit)
{
	*index = MODULO((*index - 1), limit);
}

static inline void *queue_head_node(struct be_queue_info *q)
{
	return q->dma_mem.va + q->head * q->entry_size;
}

static inline void *queue_tail_node(struct be_queue_info *q)
{
	return q->dma_mem.va + q->tail * q->entry_size;
}

static inline void *queue_index_node(struct be_queue_info *q, u16 index)
{
	return q->dma_mem.va + index * q->entry_size;
}

static inline void queue_head_inc(struct be_queue_info *q)
{
	index_inc(&q->head, q->len);
}

static inline void queue_tail_inc(struct be_queue_info *q)
{
	index_inc(&q->tail, q->len);
}

struct be_eq_obj {
	struct be_queue_info q;
	char desc[32];
	u8 idx;			/* array index */
	u8 msix_idx;
	u16 spurious_intr;
	struct napi_struct napi;
	struct be_adapter *adapter;
	cpumask_var_t  affinity_mask;

#ifdef CONFIG_NET_RX_BUSY_POLL
#define BE_EQ_IDLE		0
#define BE_EQ_NAPI		1	/* napi owns this EQ */
#define BE_EQ_POLL		2	/* poll owns this EQ */
#define BE_EQ_LOCKED		(BE_EQ_NAPI | BE_EQ_POLL)
#define BE_EQ_NAPI_YIELD	4	/* napi yielded this EQ */
#define BE_EQ_POLL_YIELD	8	/* poll yielded this EQ */
#define BE_EQ_YIELD		(BE_EQ_NAPI_YIELD | BE_EQ_POLL_YIELD)
#define BE_EQ_USER_PEND		(BE_EQ_POLL | BE_EQ_POLL_YIELD)
	unsigned int state;
	spinlock_t lock;	/* lock to serialize napi and busy-poll */
#endif  /* CONFIG_NET_RX_BUSY_POLL */
} ____cacheline_aligned_in_smp;

struct be_aic_obj {		/* Adaptive interrupt coalescing (AIC) info */
	bool enable;
	u32 min_eqd;		/* in usecs */
	u32 max_eqd;		/* in usecs */
	u32 prev_eqd;		/* in usecs */
	u32 et_eqd;		/* configured val when aic is off */
	ulong jiffies;
	u64 rx_pkts_prev;	/* Used to calculate RX pps */
	u64 tx_reqs_prev;	/* Used to calculate TX pps */
};

enum {
	NAPI_POLLING,
	BUSY_POLLING
};

struct be_mcc_obj {
	struct be_queue_info q;
	struct be_queue_info cq;
	bool rearm_cq;
	ulong hb_sent_time;	/* Request time stamp in jiffies */
	bool hb_sent;		/* heart beat sent to fw */
};

struct be_tx_stats {
	u64 tx_bytes;
	u64 tx_pkts;
	u64 tx_vxlan_offload_pkts;
	u64 tx_reqs;
	u64 tx_compl;
	ulong tx_jiffies;
	u32 tx_stops;
	u32 tx_drv_drops;	/* pkts dropped by driver */
	/* the error counters are described in be_ethtool.c */
	u32 tx_hdr_parse_err;
	u32 tx_dma_err;
	u32 tx_tso_err;
	u32 tx_spoof_check_err;
	u32 tx_qinq_err;
	u32 tx_internal_parity_err;
	struct u64_stats_sync sync;
	struct u64_stats_sync sync_compl;
};

#define MAX_BATCH_SIZE			128
/* Number of used wrbs after which DB ringing is delayed */
#define START_BATCHING_WM		(32 + BE_MAX_TX_FRAG_COUNT)
/* Number of used wrbs after which pending xmits are flushed */
#define STOP_BATCHING_WM		(192 + MAX_BATCH_SIZE)

/* Structure to hold some data of interest obtained from a TX CQE */
struct be_tx_compl_info {
	u8 status;		/* Completion status */
	u16 end_index;		/* Completed TXQ Index */
};

struct be_tx_obj {
	u32 db_offset;
	struct be_queue_info q;
	struct be_queue_info cq;
	struct be_tx_compl_info txcp;
	/* Remember the skbs that were transmitted */
	struct sk_buff *sent_skb_list[TX_Q_LEN];
	struct be_tx_stats stats;
	spinlock_t tx_lock;
	u16 pend_wrb_cnt;	/* Number of WRBs yet to be given to HW */
	u16 last_req_wrb_cnt;	/* wrb cnt of the last req in the Q */
	u16 last_req_hdr;	/* index of the last req's hdr-wrb */
	u8 idx;
} ____cacheline_aligned_in_smp;

/* Struct to remember the pages posted for rx frags */
struct be_rx_page_info {
	struct page *page;
	/* set to page-addr for last frag of the page & frag-addr otherwise */
	u64 dma_addr;
	u16 page_offset;
	bool last_frag;		/* last frag of the page */
};

struct be_rx_stats {
	u64 rx_bytes;
	u64 rx_pkts;
	u64 rx_vxlan_offload_pkts;
	u32 rx_drops_no_skbs;	/* skb allocation errors */
	u32 rx_drops_no_frags;	/* HW has no fetched frags */
	u32 rx_post_fail;	/* page post alloc failures */
	u32 rx_compl;
	u32 rx_mcast_pkts;
	u32 rx_compl_err;	/* completions with err set */
	struct u64_stats_sync sync;
};

struct be_rx_compl_info {
	u32 rss_hash;
	u16 vlan_tag;
	u16 pkt_size;
	u16 port;
	u8 vlanf;
	u8 num_rcvd;
	u8 err;
	u8 ipf;
	u8 tcpf;
	u8 udpf;
	u8 ip_csum;
	u8 l4_csum;
	u8 ipv6;
	u8 qnq;
	u8 pkt_type;
	u8 ip_frag;
	u8 tunneled;
} __packed;

struct be_rx_obj {
	struct be_adapter *adapter;
	struct be_queue_info q;
	struct be_queue_info cq;
	struct be_rx_compl_info rxcp;
	struct be_rx_page_info *page_info_tbl;
	struct be_rx_stats stats;
	u8 rss_id;
	bool rx_post_starved;	/* Zero rx frags have been posted to BE */
	u32 prev_frag_idx;
} ____cacheline_aligned_in_smp;

struct be_drv_stats {
	u32 link_down_reason;
	u32 eth_red_drops;
	u32 dma_map_errors;
	u32 rx_drops_no_pbuf;
	u32 rx_drops_no_txpb;
	u32 rx_drops_no_erx_descr;
	u32 rx_drops_no_tpre_descr;
	u32 rx_drops_too_many_frags;
	u32 forwarded_packets;
	u32 rx_drops_mtu;
	u32 rx_crc_errors;
	u32 rx_alignment_symbol_errors;
	u32 rx_pause_frames;
	u32 rx_priority_pause_frames;
	u32 rx_control_frames;
	u32 rx_in_range_errors;
	u32 rx_out_range_errors;
	u32 rx_frame_too_long;
	u32 rx_address_filtered;
	u32 rx_dropped_too_small;
	u32 rx_dropped_too_short;
	u32 rx_dropped_header_too_small;
	u32 rx_dropped_tcp_length;
	u32 rx_dropped_runt;
	u32 rx_ip_checksum_errs;
	u32 rx_tcp_checksum_errs;
	u32 rx_udp_checksum_errs;
	u32 rx_switched_unicast_packets;
	u32 rx_switched_multicast_packets;
	u32 rx_switched_broadcast_packets;
	u32 tx_pauseframes;
	u32 tx_priority_pauseframes;
	u32 tx_controlframes;
	u32 rxpp_fifo_overflow_drop;
	u32 rx_input_fifo_overflow_drop;
	u32 pmem_fifo_overflow_drop;
	u32 jabber_events;
};

/* A vlan-id of 0xFFFF must be used to clear transparent vlan-tagging */
#define BE_RESET_VLAN_TAG_ID	0xFFFF

struct be_vf_cfg {
	unsigned char mac_addr[ETH_ALEN];
	int if_handle;
	int pmac_id;
	u16 vlan_tag;
	u32 tx_rate;
	u32 linkstate;
	u32 privileges;
	bool spoofchk;
};

enum {
	ENABLED = 0,
	ASSIGNED = 1
};

#define BE_INVALID_PMAC_ID		0xffffffff
#define BE_UC_PMAC_COUNT		30
#define BE_VF_UC_PMAC_COUNT		2

#define BE_FLAGS_LINK_STATUS_INITED		BIT(1)
#define BE_FLAGS_PG_PFC				BIT(2)
#define BE_FLAGS_WORKER_SCHEDULED		BIT(3)
#define BE_FLAGS_MCAST				BIT(7)
#define BE_FLAGS_NAPI_ENABLED			BIT(8)
#define BE_FLAGS_FW_DOWNLOAD_ABORTED		BIT(9)
#define BE_FLAGS_MAC_LEARNING_ENABLED		BIT(10)
#define BE_FLAGS_SETUP_DONE			BIT(11)
#define BE_FLAGS_MAC_LEARNING_INITIALIZED	BIT(12)
#define BE_FLAGS_TRY_RECOVERY			BIT(13)
#define BE_FLAGS_OS2BMC				BIT(14)
#define BE_FLAGS_QNQ_ASYNC_EVT_RCVD		BIT(15)
#define BE_FLAGS_VXLAN_OFFLOADS			BIT(16)
#define BE_FLAGS_SRIOV_ENABLED			BIT(17)
#define BE_FLAGS_MAC_FILTERS_EXHAUSTED		BIT(19)
#define BE_FLAGS_PHY_MISCONFIGURED		BIT(20)
#define BE_FLAGS_ERR_DETECTION_SCHEDULED	BIT(21)

/* Ethtool set_dump flags */
#define LANCER_INITIATE_FW_DUMP			0x1
#define LANCER_DELETE_FW_DUMP			0x2

struct phy_info {
/* From SFF-8472 spec */
#define SFP_VENDOR_NAME_LEN			17
	u8 transceiver;
	u8 autoneg;
	u8 fc_autoneg;
	u8 port_type;
	u16 phy_type;
	u16 interface_type;
	u32 misc_params;
	u16 auto_speeds_supported;
	u16 fixed_speeds_supported;
	int link_speed;
	u32 advertising;
	u32 supported;
	u8 cable_type;
	u8 vendor_name[SFP_VENDOR_NAME_LEN];
	u8 vendor_pn[SFP_VENDOR_NAME_LEN];
};

struct be_resources {
	u16 max_vfs;		/* Total VFs "really" supported by FW/HW */
	u16 max_mcast_mac;
	u16 max_tx_qs;
	int max_prio_tx_qs;
	u16 max_rss_qs;
	u16 max_rx_qs;
	u16 max_cq_count;
	u16 max_uc_mac;		/* Max UC MACs programmable */
	u16 max_vlans;		/* Number of vlans supported */
	u16 max_iface_count;
	u16 max_mcc_count;
	u16 max_evt_qs;
	u16 max_nic_evt_qs;	/* NIC's share of evt qs */
	u32 if_cap_flags;
	u32 vf_if_cap_flags;	/* VF if capability flags */
	u32 flags;
	/* Calculated PF Pool's share of RSS Tables. This is not enforced by
	 * the FW, but is a self-imposed driver limitation.
	 */
	u16 max_rss_tables;

};

/* These are port-wide values */
struct be_port_resources {
	u16 max_vfs;
	u16 nic_pfs;
};

#define BE_MAC_AGE		300000 /* in msecs */
#define BE_MAC_HASH_TABLE_LEN	64
#define be_mac_matches(node, ethhdr)					\
		(node &&						\
		 ether_addr_equal_64bits(node->mac,			\
					 ethhdr->h_source))	\

#define be_mac_vlan_matches(node, ethhdr, vlan_id)			\
		(node &&						\
		 ether_addr_equal_64bits(node->mac,			\
					 ethhdr->h_source) &&	\
		 (node->vlan_id == vlan_id))

#define be_is_mac_learning_enabled(adapter)				\
	((adapter->flags & BE_FLAGS_MAC_LEARNING_INITIALIZED) &&	\
	 (adapter->flags & BE_FLAGS_MAC_LEARNING_ENABLED) &&		\
	 !(adapter->flags & BE_FLAGS_MAC_FILTERS_EXHAUSTED))

struct be_config_work {
	struct work_struct work;
	union {
		struct be_mac_node *mac;
	} data;
};

struct be_mac_node {
	struct rcu_head rcu;
	struct hlist_node list;
	struct be_adapter *adapter;
	u32 pmac_id;
	u8 mac[ETH_ALEN];
	u16 vlan_id;
	unsigned long jiffies;
	u8 hashidx;
	bool del;
};

#define is_be_os2bmc_enabled(adapter) (adapter->flags & BE_FLAGS_OS2BMC)

struct be_hwmon {
#ifdef HWMON_DEV_REGISTER_OLD
	struct class_device *hwmon_dev;
#else
	struct device *hwmon_dev;
#endif
	u8 be_on_die_temperature;
};

struct rss_info {
	u64 rss_flags;
	u8 rsstable[RSS_INDIR_TABLE_LEN];
	u8 rss_queue[RSS_INDIR_TABLE_LEN];
	u8 rss_hkey[RSS_HASH_KEY_LEN];
};

/* Macros to read/write the 'features' word of be_wrb_params structure. */
#define BE_WRB_F_BIT(name)		BE_WRB_F_##name##_BIT
#define BE_WRB_F_MASK(name)		BIT_MASK(BE_WRB_F_##name##_BIT)

#define BE_WRB_F_GET(word, name)	\
	(((word) & (BE_WRB_F_MASK(name))) >> BE_WRB_F_BIT(name))

#define BE_WRB_F_SET(word, name, val)	\
	((word) |= (((val) << BE_WRB_F_BIT(name)) & BE_WRB_F_MASK(name)))
/* Feature/offload bits */
enum {
	BE_WRB_F_CRC_BIT,		/* Ethernet CRC */
	BE_WRB_F_IPCS_BIT,		/* IP csum */
	BE_WRB_F_TCPCS_BIT,		/* TCP csum */
	BE_WRB_F_UDPCS_BIT,		/* UDP csum */
	BE_WRB_F_LSO_BIT,		/* LSO */
	BE_WRB_F_LSO6_BIT,		/* LSO6 */
	BE_WRB_F_VLAN_BIT,		/* VLAN */
	BE_WRB_F_VLAN_SKIP_HW_BIT,	/* Skip VLAN tag (workaround) */
	BE_WRB_F_OS2BMC_BIT		/* OS2BMC */
};

/* The structure below provides a HW-agnostic abstraction of WRB params
 * retrieved from a TX skb. This is in turn passed to chip specific routines
 * during transmit, to set the corresponding params in the WRB.
 */
struct be_wrb_params {
	u32 features;	/* Feature bits */
	u16 vlan_tag;	/* VLAN tag */
	u16 lso_mss;	/* MSS for LSO */
};

#define BE_SEC	1000			/* in msec */
#define BE_MIN	(60 * BE_SEC)		/* in msec */
#define BE_HOUR	(60 * BE_MIN)		/* in msec */

#define ERR_RECOVERY_MAX_RETRY_COUNT		3
#define ERR_RECOVERY_DETECTION_DELAY		BE_SEC
#define ERR_RECOVERY_RETRY_DELAY		(30 * BE_SEC)

/* UE-detection-duration in BEx/Skyhawk:
 * All PFs must wait for this duration after they detect UE before reading
 * SLIPORT_SEMAPHORE register. At the end of this duration, the Firmware
 * guarantees that the SLIPORT_SEMAPHORE register is updated to indicate
 * if the UE is recoverable.
 */
#define ERR_RECOVERY_UE_DETECT_DURATION			BE_SEC

/* Initial idle time (in msec) to elapse after driver load,
 * before UE recovery is allowed.
 */
#define ERR_IDLE_HR			24
#define ERR_RECOVERY_IDLE_TIME		(ERR_IDLE_HR * BE_HOUR)

/* Time interval (in msec) after which UE recovery can be repeated */
#define ERR_INTERVAL_HR			72
#define ERR_RECOVERY_INTERVAL		(ERR_INTERVAL_HR * BE_HOUR)

/* BEx/SH UE recovery state machine */
enum {
	ERR_RECOVERY_ST_NONE = 0,		/* No Recovery */
	ERR_RECOVERY_ST_DETECT = 1,		/* UE detection duration */
	ERR_RECOVERY_ST_RESET = 2,		/* Reset Phase (PF0 only) */
	ERR_RECOVERY_ST_PRE_POLL = 3,		/* Pre-Poll Phase (all PFs) */
	ERR_RECOVERY_ST_REINIT = 4		/* Re-initialize Phase */
};

struct be_error_recovery {
	/* Lancer error recovery variables */
	u8 recovery_retries;

	/* BEx/Skyhawk error recovery variables */
	u8 recovery_state;
	u16 ue_to_reset_time;		/* Time after UE, to soft reset
					 * the chip - PF0 only
					 */
	u16 ue_to_poll_time;		/* Time after UE, to Restart Polling
					 * of SLIPORT_SEMAPHORE reg
					 */
	u16 last_err_code;
	bool recovery_supported;
	unsigned long probe_time;
	unsigned long last_recovery_time;

	/* Common to both Lancer & BEx/SH error recovery */
	u32 resched_delay;
	struct delayed_work err_detection_work;
};

/* Ethtool priv_flags */
#define	BE_DISABLE_TPE_RECOVERY	0x1

struct be_adapter {
	struct pci_dev *pdev;
	struct net_device *netdev;

	u8 __iomem *csr;	/* CSR BAR used only for BE2/3 */
	u8 __iomem *db;		/* Door Bell */
	u8 __iomem *pcicfg;	/* On SH,BEx only. Shadow of PCI config space */

	struct mutex mbox_lock;	/* For serializing mbox cmds to BE card */
	struct be_dma_mem mbox_mem;
	/* Mbox mem is adjusted to align to 16 bytes. The allocated addr
	 * is stored for freeing purpose */
	struct be_dma_mem mbox_mem_alloced;

	struct be_mcc_obj mcc_obj;
	struct mutex mcc_lock;  /* For serializing mcc cmds to BE card */
	spinlock_t mcc_cq_lock;

	u16 cfg_num_rx_irqs;		/* configured via set-channels */
	u16 cfg_num_tx_irqs;		/* configured via set-channels */
	u16 num_evt_qs;
	u16 num_msix_vec;
	struct be_eq_obj eq_obj[MAX_EVT_QS];
	struct msix_entry msix_entries[MAX_MSIX_VECTORS];
	bool isr_registered;

	/* TX Rings */
	u16 num_tx_qs;
	struct be_tx_obj tx_obj[MAX_TX_QS];
	u8 prio_tc_map[MAX_TX_QS];	/* prio_tc_map[prio] => tc-id */
	u8 tc_txq_map[MAX_TX_QS];	/* tc_txq_map[tc-id] => txq index */

	/* Rx rings */
	u16 num_rx_qs;
	u16 num_rss_qs;
	u16 need_def_rxq;
	struct be_rx_obj rx_obj[MAX_RX_QS];
	u32 big_page_size;	/* Compounded page size shared by rx wrbs */
	u16 rx_frag_size;

	struct be_dma_mem stats_cmd;
	struct net_device_stats net_stats;
	struct be_drv_stats drv_stats;

	struct be_aic_obj aic_obj[MAX_EVT_QS];
	struct vlan_group *vlan_grp;
	u8 vlan_prio_bmap;		/* Available priority BitMap */
	u16 recommended_prio_bits;/* Recommended Priority bits in vlan tag */
	struct be_dma_mem rx_filter; /* Cmd DMA mem for rx-filter */

	/* Work queue used to perform periodic tasks like getting statistics */
	struct delayed_work work;

	u8 err_flags;
	bool pcicfg_mapped;	/* pcicfg obtained via pci_iomap() */
	u32 flags;
	u32 cmd_privileges;
	bool rx_csum;		/* BE card must perform rx-checksumming */
	char fw_ver[FW_VER_LEN];
	char fw_on_flash[FW_VER_LEN];

	/* IFACE filtering fields */
	u32 if_handle;		/* Used to configure filtering */
	u32 if_flags;		/* Interface filtering flags */
	u32 *pmac_id;		/* MAC addr handle used by BE card */
	u8 *uc_list;		/* list of uc-addrs programmed (not perm) */
	u32 uc_macs;		/* Count of secondary UC MAC programmed */
	u8 *mc_list;		/* list of mcast addrs programmed */
	u32 mc_count;
	unsigned long vids[BITS_TO_LONGS(VLAN_GROUP_ARRAY_LEN)];
	u16 vlans_added;
	bool update_uc_list;
	bool update_mc_list;
	struct mutex rx_filter_lock;  /* For serializing VLAN_CONFIG cmds */

	u32 beacon_state;       /* for set_phys_id */

	u32 port_num;
	u32 hba_port_num;
	char port_name;
	u8 mc_type;		/* Multichannel type */
	u32 function_mode;
	u32 function_caps;
	u32 rx_fc;		/* Rx flow control */
	u32 tx_fc;		/* Tx flow control */
	bool stats_cmd_sent;
	struct {
		u32 size;
		u32 total_size;
		u64 io_addr;
	} roce_db;
	u32 num_msix_roce_vec;
	struct ocrdma_dev *ocrdma_dev;
	struct list_head entry;
	u32 flash_status;
	struct completion et_cmd_compl;

	struct be_resources pool_res;	/* resources available for the port */
	struct be_resources res;	/* resources available for the func */
	u16 num_vfs;			/* Number of VFs provisioned by PF */
	int drv_max_vfs;		/* Number of MAX VFs supported by chip*/
	u8 pf_num;			/* Numbering used by FW, starts at 0 */
	u8 vf_num;			/* Numbering used by FW, starts at 1 */
	u8 virtfn;
	struct be_vf_cfg *vf_cfg;
	bool be3_native;
	u16 pvid;
	__be16 vxlan_port;
	int vxlan_port_count;
	int vxlan_port_aliases;
	u32 sli_family;
	struct phy_info phy;
	char model_number[32];
	u8 wol_cap;
	bool wol_en;
	u16 asic_rev;
	u16 qnq_vid;
	struct be_hwmon hwmon_info;
	struct rss_info rss_info;
	u32 msg_enable;
	struct hlist_head *mac_table;
	spinlock_t mac_hash_lock; /* For serializing mac addr hash entries */
	struct workqueue_struct *config_workq;
	u32 bmc_filt_mask;
	u32 fat_dump_len;
	u16 serial_num[CNTL_SERIAL_NUM_WORDS];
	u8 phy_state; /* state of sfp optics (functional, faulted, etc.,) */
	u32 priv_flags; /* ethtool get/set_priv_flags() */
	struct be_error_recovery error_recovery;
	u8 dev_mac[ETH_ALEN];
};

/* Used for defered FW config cmds. Add fields to this struct as reqd */
struct be_cmd_work {
	struct work_struct work;
	struct be_adapter *adapter;
	union {
		__be16 vxlan_port;
	} info;
};

#define be_physfn(adapter)		(!adapter->virtfn)
#define be_virtfn(adapter)		(adapter->virtfn)
#define sriov_enabled(adapter)		(adapter->flags &	\
					 BE_FLAGS_SRIOV_ENABLED)

#define for_all_vfs(adapter, vf_cfg, i)				\
		for (i = 0, vf_cfg = &adapter->vf_cfg[i];	\
					i < adapter->num_vfs; i++, vf_cfg++)

#define ON				1
#define OFF				0

#define be_max_vlans(adapter)		(adapter->res.max_vlans)
#define be_max_uc(adapter)		(adapter->res.max_uc_mac)
#define be_max_mc(adapter)		(adapter->res.max_mcast_mac)
#define be_max_vfs(adapter)		(adapter->pool_res.max_vfs)
#define be_max_rss(adapter)		(adapter->res.max_rss_qs)
#define be_max_txqs(adapter)		(adapter->res.max_tx_qs)
#define be_max_prio_txqs(adapter)	(adapter->res.max_prio_tx_qs)
#define be_max_rxqs(adapter)		(adapter->res.max_rx_qs)
/* Max number of EQs available for the function (NIC + RoCE (if enabled)) */
#define be_max_func_eqs(adapter)	(adapter->res.max_evt_qs)
/* Max number of EQs available avaialble only for NIC */
#define be_max_nic_eqs(adapter)		(adapter->res.max_nic_evt_qs)
#define be_if_cap_flags(adapter)	(adapter->res.if_cap_flags)
#define be_max_pf_pool_rss_tables(adapter)	\
				(adapter->pool_res.max_rss_tables)
/* Max irqs available for the nic function */
#define be_max_irqs(adapter)		\
			(min_t(u16, be_max_nic_eqs(adapter), num_online_cpus()))

/* Is BE in pvid_tagging mode */
#define be_pvid_tagging_enabled(adapter)	(adapter->pvid)

/* Is BE in QNQ multi-channel mode */
#define be_is_qnq_mode(adapter)		(adapter->function_mode & QNQ_MODE)

/* Max irqs *needed* for RX queues */
static inline u16 be_max_rx_irqs(struct be_adapter *adapter)
{
	/* If no RSS, need atleast one irq for def-RXQ */
	u16 num = max_t(u16, be_max_rss(adapter), 1);

	return min_t(u16, num, be_max_irqs(adapter));
}

/* Max irqs *needed* for TX queues */
static inline u16 be_max_tx_irqs(struct be_adapter *adapter)
{
	return min_t(u16, be_max_txqs(adapter), be_max_irqs(adapter));
}

/* Max irqs *needed* for combined queues */
static inline u16 be_max_qp_irqs(struct be_adapter *adapter)
{
	return min(be_max_tx_irqs(adapter), be_max_rx_irqs(adapter));
}

/* Max irqs *needed* for RX and TX queues together */
static inline u16 be_max_any_irqs(struct be_adapter *adapter)
{
	return max(be_max_tx_irqs(adapter), be_max_rx_irqs(adapter));
}

#define lancer_chip(adapter)	(adapter->pdev->device == OC_DEVICE_ID3 || \
				 adapter->pdev->device == OC_DEVICE_ID4)

#define LANCER_TSO_MIN_MSS		256

#define BE3_chip(adapter)	(adapter->pdev->device == BE_DEVICE_ID2 || \
				 adapter->pdev->device == OC_DEVICE_ID2)

#define BE2_chip(adapter)	(adapter->pdev->device == BE_DEVICE_ID1 || \
				 adapter->pdev->device == OC_DEVICE_ID1)

#define BEx_chip(adapter)	(BE3_chip(adapter) || BE2_chip(adapter))

#define skyhawk_chip(adapter)	(adapter->pdev->device == OC_DEVICE_ID5 || \
				 adapter->pdev->device == OC_DEVICE_ID6)
#define be_roce_supported(adapter)	(skyhawk_chip(adapter) && \
				((adapter->function_mode & RDMA_ENABLED) || \
				 (adapter->function_mode & SURF_ENABLED)))

extern struct ethtool_ops be_ethtool_ops;
#ifdef ETHTOOL_OPS_EXT_defined
extern struct ethtool_ops_ext be_ethtool_ops_ext;
#endif

#define msix_enabled(adapter)		(adapter->num_msix_vec > 0)
#define num_irqs(adapter)		(msix_enabled(adapter) ?	\
						adapter->num_msix_vec : 1)
#define tx_stats(txo)			(&(txo)->stats)
#define rx_stats(rxo)			(&(rxo)->stats)

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 29)
#define BE_SET_NETDEV_OPS(netdev, ops)	be_netdev_ops_init(netdev, ops)
#else
#define BE_SET_NETDEV_OPS(netdev, ops)	(netdev->netdev_ops = ops)
#endif

#ifdef NETDEV_OPS_EXT_defined
#define BE_NETDEV_HW_FEATURES(netdev)	(netdev_extended(netdev)->hw_features)
#else
#define BE_NETDEV_HW_FEATURES(netdev)	(netdev->hw_features)
#endif

/* The default RXQ is the last RXQ */
#define default_rxo(adpt)		(&adpt->rx_obj[adpt->num_rx_qs - 1])

#define for_all_rx_queues(adapter, rxo, i)				\
	for (i = 0, rxo = &adapter->rx_obj[i]; i < adapter->num_rx_qs;	\
		i++, rxo++)

#define for_all_rss_queues(adapter, rxo, i)				\
	for (i = 0, rxo = &adapter->rx_obj[i]; i < (adapter->num_rss_qs);\
		i++, rxo++)

#define for_all_tx_queues(adapter, txo, i)				\
	for (i = 0, txo = &adapter->tx_obj[i]; i < adapter->num_tx_qs;	\
		i++, txo++)

#define for_all_evt_queues(adapter, eqo, i)				\
	for (i = 0, eqo = &adapter->eq_obj[i]; i < adapter->num_evt_qs; \
		i++, eqo++)

#define for_all_rx_queues_on_eq(adapter, eqo, rxo, i)			\
	for (i = eqo->idx, rxo = &adapter->rx_obj[i]; i < adapter->num_rx_qs;\
		i += adapter->num_evt_qs, rxo += adapter->num_evt_qs)

#define for_all_tx_queues_on_eq(adapter, eqo, txo, i)			\
	for (i = eqo->idx, txo = &adapter->tx_obj[i]; i < adapter->num_tx_qs;\
		i += adapter->num_evt_qs, txo += adapter->num_evt_qs)

#define is_mcc_eqo(eqo) 		(eqo->idx == 0)
#define mcc_eqo(adapter)		(&adapter->eq_obj[0])

#define PAGE_SHIFT_4K		12
#define PAGE_SIZE_4K		(1 << PAGE_SHIFT_4K)

/* Returns number of pages spanned by the data starting at the given addr */
#define PAGES_4K_SPANNED(_address, size)				\
	((u32)((((size_t)(_address) & (PAGE_SIZE_4K - 1)) +	\
			(size) + (PAGE_SIZE_4K - 1)) >> PAGE_SHIFT_4K))

/* Returns bit offset within a DWORD of a bitfield */
#define AMAP_BIT_OFFSET(_struct, field)					\
		(((size_t)&(((_struct *)0)->field))%32)

/* Returns the bit mask of the field that is NOT shifted into location. */
static inline u32 amap_mask(u32 bitsize)
{
	return (bitsize == 32 ? 0xFFFFFFFF : (1 << bitsize) - 1);
}

static inline void
amap_set(void *ptr, u32 dw_offset, u32 mask, u32 offset, u32 value)
{
	u32 *dw = (u32 *) ptr + dw_offset;
	*dw &= ~(mask << offset);
	*dw |= (mask & value) << offset;
}

#define AMAP_SET_BITS(_struct, field, ptr, val)				\
		amap_set(ptr,						\
			offsetof(_struct, field)/32,			\
			amap_mask(sizeof(((_struct *)0)->field)),	\
			AMAP_BIT_OFFSET(_struct, field),		\
			val)

static inline u32 amap_get(void *ptr, u32 dw_offset, u32 mask, u32 offset)
{
	u32 *dw = (u32 *) ptr;
	return mask & (*(dw + dw_offset) >> offset);
}

#define AMAP_GET_BITS(_struct, field, ptr)				\
		amap_get(ptr,						\
			offsetof(_struct, field)/32,			\
			amap_mask(sizeof(((_struct *)0)->field)),	\
			AMAP_BIT_OFFSET(_struct, field))

#define GET_RX_COMPL_V0_BITS(field, ptr)				\
		AMAP_GET_BITS(struct amap_eth_rx_compl_v0, field, ptr)

#define GET_RX_COMPL_V1_BITS(field, ptr)				\
		AMAP_GET_BITS(struct amap_eth_rx_compl_v1, field, ptr)

#define GET_TX_COMPL_BITS(field, ptr)					\
		AMAP_GET_BITS(struct amap_eth_tx_compl, field, ptr)

#define SET_TX_WRB_HDR_BITS(field, ptr, val)				\
		AMAP_SET_BITS(struct amap_eth_hdr_wrb, field, ptr, val)

#define be_dws_cpu_to_le(wrb, len)	swap_dws(wrb, len)
#define be_dws_le_to_cpu(wrb, len)	swap_dws(wrb, len)
static inline void swap_dws(void *wrb, int len)
{
#ifdef __BIG_ENDIAN
	u32 *dw = wrb;
	BUG_ON(len % 4);
	do {
		*dw = cpu_to_le32(*dw);
		dw++;
		len -= 4;
	} while (len);
#endif				/* __BIG_ENDIAN */
}

#define be_cmd_status(status)		(status > 0 ? -EIO : status)

static inline bool is_tcp_pkt(struct sk_buff *skb)
{
	u8 val = 0;

	if (ip_hdr(skb)->version == 4)
		val = (ip_hdr(skb)->protocol == IPPROTO_TCP);
	else if (ip_hdr(skb)->version == 6)
		val = (ipv6_hdr(skb)->nexthdr == NEXTHDR_TCP);

	return val;
}

static inline bool is_udp_pkt(struct sk_buff *skb)
{
	u8 val = 0;

	if (ip_hdr(skb)->version == 4)
		val = (ip_hdr(skb)->protocol == IPPROTO_UDP);
	else if (ip_hdr(skb)->version == 6)
		val = (ipv6_hdr(skb)->nexthdr == NEXTHDR_UDP);

	return val;
}

static inline bool is_ipv4_pkt(struct sk_buff *skb)
{
	return skb->protocol == htons(ETH_P_IP) &&
		ip_hdr(skb)->version == 4;
}

static inline bool is_ipv6_ext_hdr(struct sk_buff *skb)
{
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	if (ip_hdr(skb)->version == 6)
		return ipv6_ext_hdr(ipv6_hdr(skb)->nexthdr);
	else
#endif
		return false;
}

static inline u16 be_get_tx_vlan_tag(struct be_adapter *adapter,
				     struct sk_buff *skb)
{
	u8 vlan_prio = 0;
	u16 vlan_tag = 0;

	vlan_tag = skb_vlan_tag_get(skb);
	vlan_prio = (vlan_tag & VLAN_PRIO_MASK) >> VLAN_PRIO_SHIFT;
	/* If vlan priority provided by OS is NOT in available bmap */
	if (!(adapter->vlan_prio_bmap & (1 << vlan_prio)))
		vlan_tag = (vlan_tag & ~VLAN_PRIO_MASK) |
				adapter->recommended_prio_bits;

	return vlan_tag;
}

#define be_error_recovering(adapter)		\
		(adapter->flags & BE_FLAGS_TRY_RECOVERY)

#define BE_ERROR_EEH		1
#define BE_ERROR_UE		BIT(1)
#define BE_ERROR_FW		BIT(2)
#define BE_ERROR_HW		(BE_ERROR_EEH | BE_ERROR_UE)
#define BE_ERROR_ANY		(BE_ERROR_EEH | BE_ERROR_UE | BE_ERROR_FW)
#define BE_CLEAR_ALL		0xFF

static inline u8 be_check_error(struct be_adapter *adapter, u32 err_type)
{
	return (adapter->err_flags & err_type);
}

static inline void be_set_error(struct be_adapter *adapter, int err_type)
{
	struct net_device *netdev = adapter->netdev;

	adapter->err_flags |= err_type;
	netif_carrier_off(netdev);

	dev_info(&adapter->pdev->dev, "%s: Link down\n", netdev->name);
}

static inline void  be_clear_error(struct be_adapter *adapter, int err_type)
{
	adapter->err_flags &= ~err_type;
}

static inline bool be_multi_rxq(struct be_adapter *adapter)
{
	return (adapter->num_rx_qs > 1);
}

void be_cq_notify(struct be_adapter *adapter, u16 qid, bool arm,
		  u16 num_popped);
void be_link_status_update(struct be_adapter *adapter, u8 link_status);
void be_parse_stats(struct be_adapter *adapter);
int be_load_fw(struct be_adapter *adapter, u8 *func);
void be_sysfs_create_group(struct be_adapter *adapter);
void be_sysfs_remove_group(struct be_adapter *adapter);
bool be_is_wol_supported(struct be_adapter *adapter);
bool be_pause_autoneg_supported(struct be_adapter *adapter);
u32 be_get_fw_log_level(struct be_adapter *adapter);
int be_update_queues(struct be_adapter *adapter);
int be_poll(struct napi_struct *napi, int budget);
void be_eqd_update(struct be_adapter *adapter, bool force_update);
int be_pci_sriov_configure(struct pci_dev *pdev, int num_vfs);

/*
 * internal function to initialize-cleanup roce device.
 */
void be_roce_dev_add(struct be_adapter *);
void be_roce_dev_remove(struct be_adapter *);

/*
 * internal function to open-close roce device during ifup-ifdown.
 */
void be_roce_dev_shutdown(struct be_adapter *);

/* OS2BMC related */

#define DHCP_CLIENT_PORT	68
#define DHCP_SERVER_PORT	67
#define NET_BIOS_PORT1		137
#define NET_BIOS_PORT2		138
#define DHCPV6_RAS_PORT		547

#define BMC_FILT_BROADCAST_ARP				((u32)(1))
#define BMC_FILT_BROADCAST_DHCP_CLIENT			((u32)(1 << 1))
#define BMC_FILT_BROADCAST_DHCP_SERVER			((u32)(1 << 2))
#define BMC_FILT_BROADCAST_NET_BIOS			((u32)(1 << 3))
#define BMC_FILT_BROADCAST				((u32)(1 << 4))
#define BMC_FILT_MULTICAST_IPV6_NEIGH_ADVER		((u32)(1 << 5))
#define BMC_FILT_MULTICAST_IPV6_RA			((u32)(1 << 6))
#define BMC_FILT_MULTICAST_IPV6_RAS			((u32)(1 << 7))
#define BMC_FILT_MULTICAST				((u32)(1 << 8))


#define is_mc_allowed_on_bmc(adapter, eh)	\
	(!is_multicast_filt_enabled(adapter) &&	\
	 is_multicast_ether_addr(eh->h_dest) &&	\
	 !is_broadcast_ether_addr(eh->h_dest))

#define is_bc_allowed_on_bmc(adapter, eh)	\
	(!is_broadcast_filt_enabled(adapter) &&	\
	 is_broadcast_ether_addr(eh->h_dest))

#define is_arp_allowed_on_bmc(adapter, skb)	\
	(is_arp(skb) && is_arp_filt_enabled(adapter))

#define is_broadcast_packet(eh, adapter)	\
		(is_multicast_ether_addr(eh->h_dest) && \
		!compare_ether_addr(eh->h_dest, adapter->netdev->broadcast))

#define is_arp(skb)	(skb->protocol == htons(ETH_P_ARP))

#define is_arp_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & (BMC_FILT_BROADCAST_ARP))

#define is_dhcp_client_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_BROADCAST_DHCP_CLIENT)

#define is_dhcp_srvr_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_BROADCAST_DHCP_SERVER)

#define is_nbios_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_BROADCAST_NET_BIOS)

#define is_ipv6_na_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask &	\
			BMC_FILT_MULTICAST_IPV6_NEIGH_ADVER)

#define is_ipv6_ra_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_MULTICAST_IPV6_RA)

#define is_ipv6_ras_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_MULTICAST_IPV6_RAS)

#define is_broadcast_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_BROADCAST)

#define is_multicast_filt_enabled(adapter)	\
		(adapter->bmc_filt_mask & BMC_FILT_MULTICAST)

#endif				/* BE_H */
