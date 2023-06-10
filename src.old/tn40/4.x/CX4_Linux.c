#include "tn40.h"

int CX4_get_settings(struct net_device *netdev, struct ethtool_cmd *ecmd)
{
    ecmd->supported   = (SUPPORTED_10000baseT_Full  | SUPPORTED_AUI | SUPPORTED_Pause);
	ecmd->advertising = (ADVERTISED_10000baseT_Full | ADVERTISED_Pause);
	ecmd->speed       = SPEED_10000;
	ecmd->duplex      = DUPLEX_FULL;
	ecmd->port        = PORT_AUI;
	ecmd->transceiver = XCVR_INTERNAL;
	ecmd->autoneg     = AUTONEG_DISABLE;

	return 0;

} // CX4_get_settings()

//-------------------------------------------------------------------------------------------------

int CX4_set_settings(struct net_device *netdev, struct ethtool_cmd *ecmd)
{
	ERR("CX4_set_settings() not implemented\n");

	return -EPERM;

} // CX4_set_settings()

//-------------------------------------------------------------------------------------------------

__init void CX4_register_settings(struct bdx_priv *priv)
{
    priv->phy_ops.get_settings = CX4_get_settings;
    priv->phy_ops.set_settings = CX4_set_settings;

} // CX4_register_settings()

//-------------------------------------------------------------------------------------------------

