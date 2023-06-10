/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2018-2021, Intel Corporation. */

#ifndef _ICE_GNSS_H_
#define _ICE_GNSS_H_

#include <linux/tty.h>
#include <linux/tty_flip.h>

#define ICE_E810T_GNSS_I2C_BUS		0x2
#define ICE_GNSS_TIMER_DELAY_TIME	(HZ / 10) /* 0.1 second per message */
#define ICE_GNSS_TTY_MINOR_DEVICES	2
#define ICE_GNSS_TTY_WRITE_BUF		250
#define ICE_MAX_I2C_DATA_SIZE		(ICE_AQC_I2C_DATA_SIZE_M >> \
					 ICE_AQC_I2C_DATA_SIZE_S)
#define ICE_MAX_I2C_WRITE_BYTES		4

/* ublox specific deifinitions */
#define ICE_GNSS_UBX_I2C_BUS_ADDR	0x42
/* Data length register is big endian */
#define ICE_GNSS_UBX_DATA_LEN_H		0xFD
#define ICE_GNSS_UBX_DATA_LEN_WIDTH	2
#define ICE_GNSS_UBX_EMPTY_DATA		0xFF
/* For ublox writes are performed without address so the first byte to write is
 * passed as I2C addr parameter.
 */
#define ICE_GNSS_UBX_WRITE_BYTES	(ICE_MAX_I2C_WRITE_BYTES + 1)
#define ICE_MAX_UBX_READ_TRIES		255
#define ICE_MAX_UBX_ACK_READ_TRIES	4095

struct gnss_write_buf {
	struct list_head queue;
	unsigned int size;
	unsigned char *buf;
};

/**
 * struct gnss_serial - data used to initialize GNSS TTY port
 * @back: back pointer to PF
 * @tty: pointer to the tty for this device
 * @open_count: number of times this port has been opened
 * @gnss_mutex: gnss_mutex used to protect GNSS serial operations
 * @kworker: kwork thread for handling periodic work
 * @read_work: read_work function for handling GNSS reads
 * @write_work: write_work function for handling GNSS writes
 * @queue: write buffers queue
 * @buf: write buffer for a single u8, negative if empty
 */
struct gnss_serial {
	struct ice_pf *back;
	struct tty_struct *tty;
	int open_count;
	struct mutex gnss_mutex; /* protects GNSS serial structure */
	struct kthread_worker *kworker;
	struct kthread_delayed_work read_work;
	struct kthread_work write_work;
	struct list_head queue;
};

void ice_gnss_init(struct ice_pf *pf);
void ice_gnss_exit(struct ice_pf *pf);
bool ice_gnss_is_gps_present(struct ice_hw *hw);
#endif /* _ICE_GNSS_H_ */
