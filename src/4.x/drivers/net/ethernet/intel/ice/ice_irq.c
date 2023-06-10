// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2018-2021, Intel Corporation. */

#include "ice.h"
#include "ice_lib.h"
#include "ice_irq.h"

#ifdef HAVE_PCI_ALLOC_IRQ
static int ice_alloc_and_fill_msix_entries(struct ice_pf *pf, int nvec)
{
	int i;

	pf->msix_entries = kcalloc(nvec, sizeof(*pf->msix_entries),
				   GFP_KERNEL);
	if (!pf->msix_entries)
		return -ENOMEM;

	for (i = 0; i < nvec; i++) {
		pf->msix_entries[i].entry = i;
		pf->msix_entries[i].vector = ice_get_irq_num(pf, i);
	}

	return 0;
}
#endif /* HAVE_PCI_ALLOC_IRQ */

#ifndef HAVE_PCI_ALLOC_IRQ
static int ice_alloc_msix_entries(struct ice_pf *pf, u16 num_entries)
{
	u16 i;

	pf->msix_entries = devm_kcalloc(ice_pf_to_dev(pf), num_entries,
					sizeof(*pf->msix_entries), GFP_KERNEL);
	if (!pf->msix_entries)
		return -ENOMEM;

	for (i = 0; i < num_entries; i++)
		pf->msix_entries[i].entry = i;

	return 0;
}

static void ice_free_msix_entries(struct ice_pf *pf)
{
	devm_kfree(ice_pf_to_dev(pf), pf->msix_entries);
	pf->msix_entries = NULL;
}
#endif /* HAVE_PCI_ALLOC_IRQ */

static void ice_dis_msix(struct ice_pf *pf)
{
#ifdef HAVE_PCI_ALLOC_IRQ
	pci_free_irq_vectors(pf->pdev);
#else
	ice_free_msix_entries(pf);
	pci_disable_msix(pf->pdev);
#endif /* HAVE_PCI_ALLOC_IRQ */
}

static int ice_ena_msix(struct ice_pf *pf, int nvec)
{
#ifdef HAVE_PCI_ALLOC_IRQ
	return pci_alloc_irq_vectors(pf->pdev, ICE_MIN_MSIX, nvec,
				     PCI_IRQ_MSIX);
#else
	int vectors;
	int err;

	err = ice_alloc_msix_entries(pf, nvec);
	if (err)
		return err;

	vectors = pci_enable_msix_range(pf->pdev, pf->msix_entries,
					ICE_MIN_MSIX, nvec);
	if (vectors < 0)
		ice_free_msix_entries(pf);

	return vectors;
#endif /* HAVE_PCI_ALLOC_IRQ */
}

static void ice_adj_vec_clear(int *src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		src[i] = 0;
}

static void ice_adj_vec_sum(int *dst, int *src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		dst[i] += src[i];
}

/*
 * Allow 256 queue pairs for ADQ only if the PF has at least
 * 1024 msix vectors (1 or 2 port NIC).
 */
static int ice_adq_max_qps(struct ice_pf *pf)
{
	if (pf->hw.func_caps.common_cap.num_msix_vectors >= 1024)
		return ICE_ADQ_MAX_QPS;

	return num_online_cpus();
}

/**
 * ice_ena_msix_range - request a range of MSI-X vectors from the OS
 * @pf: board private structure
 *
 * The driver tries to enable best-case scenario MSI-X vectors. If that doesn't
 * succeed than adjust to irqs number returned by kernel.
 *
 * The fall-back logic is described below with each [#] represented needed irqs
 * number for the step. If any of the steps is lower than received number, then
 * return the number of MSI-X. If any of the steps is greater, then check next
 * one. If received value is lower than irqs value in last step return error.
 *
 * Step [0]: Enable the best-case scenario MSI-X vectors.
 *
 * Step [1]: Enable MSI-X vectors with eswitch support disabled
 *
 * Step [2]: Enable MSI-X vectors with the number of vectors reserved for
 * MACVLAN and Scalable IOV support reduced by a factor of 2.
 *
 * Step [3]: Enable MSI-X vectors with the number of vectors reserved for
 * MACVLAN and Scalable IOV support reduced by a factor of 4.
 *
 * Step [4]: Enable MSI-X vectors with MACVLAN and Scalable IOV support
 * disabled.
 *
 * Step [5]: Enable MSI-X vectors with the number of pf->num_lan_msix reduced
 * by a factor of 2 from the previous step (i.e. num_online_cpus() / 2).
 * Also, with the number of pf->num_rdma_msix reduced by a factor of ~2 from the
 * previous step (i.e. num_online_cpus() / 2 + ICE_RDMA_NUM_AEQ_MSIX).
 *
 * Step [6]: Same as step [3], except reduce both by a factor of 4.
 *
 * Step [7]: Enable the bare-minimum MSI-X vectors.
 *
 * Each feature has separate table with needed irqs in each step. Sum of these
 * tables is tracked in adj_vec to show needed irqs in each step. Separate
 * tables are later use to set correct number of irqs for each feature based on
 * choosed step.
 */
static int ice_ena_msix_range(struct ice_pf *pf)
{
#define ICE_ADJ_VEC_STEPS 8
#define ICE_ADJ_VEC_WORST_CASE 0
#define ICE_ADJ_VEC_BEST_CASE (ICE_ADJ_VEC_STEPS - 1)
	int num_cpus = num_online_cpus();
	int rdma_adj_vec[ICE_ADJ_VEC_STEPS] = {
		ICE_MIN_RDMA_MSIX,
		num_cpus / 4 > ICE_MIN_RDMA_MSIX ?
			num_cpus / 4 + ICE_RDMA_NUM_AEQ_MSIX :
			ICE_MIN_RDMA_MSIX,
		num_cpus / 2 > ICE_MIN_RDMA_MSIX ?
			num_cpus / 2 + ICE_RDMA_NUM_AEQ_MSIX :
			ICE_MIN_RDMA_MSIX,
		num_cpus > ICE_MIN_RDMA_MSIX ?
			num_cpus + ICE_RDMA_NUM_AEQ_MSIX : ICE_MIN_RDMA_MSIX,
		num_cpus > ICE_MIN_RDMA_MSIX ?
			num_cpus + ICE_RDMA_NUM_AEQ_MSIX : ICE_MIN_RDMA_MSIX,
		num_cpus > ICE_MIN_RDMA_MSIX ?
			num_cpus + ICE_RDMA_NUM_AEQ_MSIX : ICE_MIN_RDMA_MSIX,
		num_cpus > ICE_MIN_RDMA_MSIX ?
			num_cpus + ICE_RDMA_NUM_AEQ_MSIX : ICE_MIN_RDMA_MSIX,
		num_cpus > ICE_MIN_RDMA_MSIX ?
			num_cpus + ICE_RDMA_NUM_AEQ_MSIX : ICE_MIN_RDMA_MSIX,
	};
	int lan_adj_vec[ICE_ADJ_VEC_STEPS] = {
		ICE_MIN_LAN_MSIX,
		max_t(int, num_cpus / 4, ICE_MIN_LAN_MSIX),
		max_t(int, num_cpus / 2, ICE_MIN_LAN_MSIX),
		max_t(int, num_cpus, ICE_MIN_LAN_MSIX),
		max_t(int, num_cpus, ICE_MIN_LAN_MSIX),
		max_t(int, num_cpus, ICE_MIN_LAN_MSIX),
		max_t(int, num_cpus, ICE_MIN_LAN_MSIX),
		max_t(int, ice_adq_max_qps(pf), ICE_MIN_LAN_MSIX),
	};
	int fdir_adj_vec[ICE_ADJ_VEC_STEPS] = {
		ICE_FDIR_MSIX, ICE_FDIR_MSIX, ICE_FDIR_MSIX,
		ICE_FDIR_MSIX, ICE_FDIR_MSIX, ICE_FDIR_MSIX,
		ICE_FDIR_MSIX, ICE_FDIR_MSIX,
	};
	int adj_vec[ICE_ADJ_VEC_STEPS] = {
		ICE_OICR_MSIX, ICE_OICR_MSIX, ICE_OICR_MSIX,
		ICE_OICR_MSIX, ICE_OICR_MSIX, ICE_OICR_MSIX,
		ICE_OICR_MSIX, ICE_OICR_MSIX,
	};
#ifdef HAVE_NDO_DFWD_OPS
	int macvlan_adj_vec[ICE_ADJ_VEC_STEPS] = {
		0, 0, 0, 0,
		(ICE_MAX_MACVLANS * ICE_DFLT_VEC_VMDQ_VSI) / 4,
		(ICE_MAX_MACVLANS * ICE_DFLT_VEC_VMDQ_VSI) / 2,
		ICE_MAX_MACVLANS * ICE_DFLT_VEC_VMDQ_VSI,
		ICE_MAX_MACVLANS * ICE_DFLT_VEC_VMDQ_VSI,
	};
#endif /* OFFLOAD_MACVLAN_SUPPORT */
	int eswitch_adj_vec[ICE_ADJ_VEC_STEPS] = {
		0, 0, 0, 0, 0, 0, 0,
		ICE_ESWITCH_MSIX,
	};
	int scalable_adj_vec[ICE_ADJ_VEC_STEPS] = {
		0, 0, 0, 0,
		(ICE_MAX_SCALABLE * ICE_NUM_VF_MSIX_SMALL) / 4,
		(ICE_MAX_SCALABLE * ICE_NUM_VF_MSIX_SMALL) / 2,
		ICE_MAX_SCALABLE * ICE_NUM_VF_MSIX_SMALL,
		ICE_MAX_SCALABLE * ICE_NUM_VF_MSIX_SMALL,
	};
	struct device *dev = ice_pf_to_dev(pf);
	int adj_step = ICE_ADJ_VEC_BEST_CASE;
	int err = -ENOSPC;
	int v_actual, i;
	int needed = 0;

	needed += ICE_OICR_MSIX;

	needed += lan_adj_vec[ICE_ADJ_VEC_BEST_CASE];
	ice_adj_vec_sum(adj_vec, lan_adj_vec, ICE_ADJ_VEC_STEPS);

	if (test_bit(ICE_FLAG_ESWITCH_CAPABLE, pf->flags)) {
		needed += eswitch_adj_vec[ICE_ADJ_VEC_BEST_CASE];
		ice_adj_vec_sum(adj_vec, eswitch_adj_vec, ICE_ADJ_VEC_STEPS);
	} else {
		ice_adj_vec_clear(eswitch_adj_vec, ICE_ADJ_VEC_STEPS);
	}
#ifdef HAVE_NDO_DFWD_OPS

	if (test_bit(ICE_FLAG_VMDQ_ENA, pf->flags)) {
		needed += macvlan_adj_vec[ICE_ADJ_VEC_BEST_CASE];
		ice_adj_vec_sum(adj_vec, macvlan_adj_vec, ICE_ADJ_VEC_STEPS);
	} else {
		ice_adj_vec_clear(macvlan_adj_vec, ICE_ADJ_VEC_STEPS);
	}
#endif /* OFFLOAD_MACVLAN_SUPPORT */

	if (ice_chk_rdma_cap(pf)) {
		needed += rdma_adj_vec[ICE_ADJ_VEC_BEST_CASE];
		ice_adj_vec_sum(adj_vec, rdma_adj_vec, ICE_ADJ_VEC_STEPS);
	} else {
		ice_adj_vec_clear(rdma_adj_vec, ICE_ADJ_VEC_STEPS);
	}

	if (test_bit(ICE_FLAG_FD_ENA, pf->flags)) {
		needed += fdir_adj_vec[ICE_ADJ_VEC_BEST_CASE];
		ice_adj_vec_sum(adj_vec, fdir_adj_vec, ICE_ADJ_VEC_STEPS);
	} else {
		ice_adj_vec_clear(fdir_adj_vec, ICE_ADJ_VEC_STEPS);
	}

	if (test_bit(ICE_FLAG_SIOV_CAPABLE, pf->flags)) {
		needed += scalable_adj_vec[ICE_ADJ_VEC_BEST_CASE];
		ice_adj_vec_sum(adj_vec, scalable_adj_vec, ICE_ADJ_VEC_STEPS);
	} else {
		ice_adj_vec_clear(scalable_adj_vec, ICE_ADJ_VEC_STEPS);
	}

	v_actual = ice_ena_msix(pf, needed);
	if (v_actual < 0) {
		err = v_actual;
		goto err;
	} else if (v_actual < adj_vec[ICE_ADJ_VEC_WORST_CASE]) {
		ice_dis_msix(pf);
		goto err;
	}

	for (i = ICE_ADJ_VEC_WORST_CASE + 1; i < ICE_ADJ_VEC_STEPS; i++) {
		if (v_actual < adj_vec[i]) {
			adj_step = i - 1;
			break;
		}
	}

	pf->num_lan_msix = lan_adj_vec[adj_step];
	pf->num_rdma_msix = rdma_adj_vec[adj_step];
	if (test_bit(ICE_FLAG_ESWITCH_CAPABLE, pf->flags) &&
	    !eswitch_adj_vec[adj_step]) {
		dev_warn(dev, "Not enough MSI-X for eswitch support, disabling feature\n");
		clear_bit(ICE_FLAG_ESWITCH_CAPABLE, pf->flags);
	}
#ifdef HAVE_NDO_DFWD_OPS
	if (test_bit(ICE_FLAG_VMDQ_ENA, pf->flags) &&
	    !macvlan_adj_vec[adj_step]) {
		dev_warn(dev, "Not enough MSI-X for hardware MACVLAN support, disabling feature\n");
		clear_bit(ICE_FLAG_VMDQ_ENA, pf->flags);
	}
#endif /* OFFLOAD_MACVLAN_SUPPORT */
	pf->max_adq_qps = lan_adj_vec[adj_step];
	if (test_bit(ICE_FLAG_SIOV_CAPABLE, pf->flags) &&
	    !scalable_adj_vec[adj_step]) {
		dev_warn(dev, "Not enough MSI-X for Scalable IOV support, disabling feature\n");
		clear_bit(ICE_FLAG_SIOV_CAPABLE, pf->flags);
	}
	return v_actual;

err:
	dev_err(dev, "Failed to enable MSI-X vectors\n");
	return  err;
}

/**
 * ice_init_interrupt_scheme - Determine proper interrupt scheme
 * @pf: board private structure to initialize
 */
int ice_init_interrupt_scheme(struct ice_pf *pf)
{
	int vectors = ice_ena_msix_range(pf);

	if (vectors < 0)
		return vectors;

	/* pf->msix_entries is used in idc and needs to be filled on kernel
	 * with new irq alloc API
	 */
#ifdef HAVE_PCI_ALLOC_IRQ
	if (ice_alloc_and_fill_msix_entries(pf, vectors)) {
		ice_dis_msix(pf);
		return -ENOMEM;
	}
#endif /* HAVE_PCI_ALLOC_IRQ */
	/* set up vector assignment tracking */
	pf->irq_tracker =
		devm_kzalloc(ice_pf_to_dev(pf),
			     struct_size(pf->irq_tracker, list, vectors),
			     GFP_KERNEL);
	if (!pf->irq_tracker) {
		ice_dis_msix(pf);
		return -ENOMEM;
	}

	/* populate SW interrupts pool with number of OS granted IRQs. */
	pf->num_avail_sw_msix = (u16)vectors;
	pf->irq_tracker->num_entries = (u16)vectors;
	pf->irq_tracker->end = pf->irq_tracker->num_entries;

	return 0;
}

/**
 * ice_clear_interrupt_scheme - Undo things done by ice_init_interrupt_scheme
 * @pf: board private structure
 */
void ice_clear_interrupt_scheme(struct ice_pf *pf)
{
#ifdef HAVE_PCI_ALLOC_IRQ
	kfree(pf->msix_entries);
	pf->msix_entries = NULL;

#endif /* PEER_SUPPORT */
	ice_dis_msix(pf);

	if (pf->irq_tracker) {
		devm_kfree(ice_pf_to_dev(pf), pf->irq_tracker);
		pf->irq_tracker = NULL;
	}
}

/**
 * ice_get_irq_num - get system irq number based on index from driver
 * @pf: board private structure
 * @idx: driver irq index
 */
int ice_get_irq_num(struct ice_pf *pf, int idx)
{
#ifdef HAVE_PCI_ALLOC_IRQ
	return pci_irq_vector(pf->pdev, idx);
#else
	if (!pf->msix_entries)
		return -EINVAL;

	return pf->msix_entries[idx].vector;
#endif /* HAVE_PCI_ALLOC_IRQ */
}
