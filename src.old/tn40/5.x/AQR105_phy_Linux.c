#include "tn40.h"

int AQR105_set_speed(struct bdx_priv *priv, s32 speed);

int AQR105_get_settings(struct net_device *netdev, struct ethtool_cmd *ecmd)
{
    struct bdx_priv *priv = netdev_priv(netdev);


	ecmd->supported   = (SUPPORTED_10000baseT_Full  | SUPPORTED_1000baseT_Full  | SUPPORTED_100baseT_Full  | SUPPORTED_Autoneg  | SUPPORTED_TP | SUPPORTED_Pause);
	if(!priv->advertising)
	{
		priv->advertising   = ecmd->supported;
		priv->autoneg     = AUTONEG_ENABLE;
	}
    ecmd->advertising = priv->advertising;
	ecmd->speed       =  priv->link_speed;
	ecmd->duplex      = DUPLEX_FULL;
	ecmd->port        = PORT_TP;
	ecmd->transceiver = XCVR_INTERNAL;
	ecmd->autoneg     = priv->autoneg;
#if defined(ETH_TP_MDI_AUTO)
	ecmd->eth_tp_mdix = ETH_TP_MDI_AUTO;
#else
#if (!defined VM_KLNX)
	ecmd->eth_tp_mdix = ETH_TP_MDI | ETH_TP_MDI_X;
#endif
#endif
	return 0;

} // AQR105_get_settings()

//-------------------------------------------------------------------------------------------------

int AQR105_set_settings(struct net_device *netdev, struct ethtool_cmd *ecmd)
{
	struct bdx_priv *priv 	= netdev_priv(netdev);
	s32 			speed 	= ethtool_cmd_speed(ecmd);
	int				rVal 	= 0;

	DBG("AQR105 ecmd->cmd=%x\n", ecmd->cmd);
	DBG("AQR105 speed=%u\n",speed);
	DBG("AQR105 ecmd->autoneg=%u\n",ecmd->autoneg);

	if (ecmd->autoneg == AUTONEG_ENABLE)
	{
		DBG("AQR105 speed %d Autoneg\n", speed);
		priv->advertising = (ADVERTISED_10000baseT_Full | ADVERTISED_1000baseT_Full |
						     ADVERTISED_100baseT_Full   | ADVERTISED_Autoneg | ADVERTISED_Pause);
		priv->autoneg = AUTONEG_ENABLE;
	}
	else
	{
		priv->autoneg = AUTONEG_DISABLE;
		switch (speed)
		{
			case 10000: //10G
				priv->advertising = (ADVERTISED_10000baseT_Full | ADVERTISED_Pause);
				break;

			case 5000: //5G
				priv->advertising = (/*ADVERTISED_5000baseT_Full | */ADVERTISED_Pause);
				break;

			case 2500: //2.5G
				priv->advertising = (/*ADVERTISED_2500baseT_Full |*/ADVERTISED_Pause);
				break;

			case 1000:  //1G
				priv->advertising = (ADVERTISED_1000baseT_Full | ADVERTISED_Pause);
				break;

			case 100:   //100m
				priv->advertising = (ADVERTISED_100baseT_Full | ADVERTISED_Pause);
				break;

			default:
				ERR("%s does not support speed %u\n", priv->ndev->name, speed);
				rVal = -EINVAL;
		}
	}
	if (rVal == 0)
	{
		rVal = AQR105_set_speed(priv, speed);
	}

	return rVal;

} // AQR105_set_settings()

//-------------------------------------------------------------------------------------------------

#ifdef ETHTOOL_GLINKSETTINGS
#define AQR105_ALL_SPEEDS	(__ETHTOOL_LINK_MODE_LAST+1)

static void AQR105_set_link_mode(unsigned long *bits, u32 speed)
{
	bitmap_zero(bits, __ETHTOOL_LINK_MODE_MASK_NBITS);
	__set_bit(ETHTOOL_LINK_MODE_Pause_BIT,   	  		bits);
	__set_bit(ETHTOOL_LINK_MODE_TP_BIT,   	  			bits);;
	if (speed == AQR105_ALL_SPEEDS)
	{
		__set_bit(ETHTOOL_LINK_MODE_10000baseT_Full_BIT, 	bits);
//		__set_bit(ETHTOOL_LINK_MODE_5000baseT_Full_BIT, 	bits);
//		__set_bit(ETHTOOL_LINK_MODE_2500baseT_Full_BIT, 	bits);
		__set_bit(ETHTOOL_LINK_MODE_1000baseT_Full_BIT,  	bits);
		__set_bit(ETHTOOL_LINK_MODE_100baseT_Full_BIT,   	bits);
		__set_bit(ETHTOOL_LINK_MODE_Autoneg_BIT,   	  		bits);
	}
	else
	{
		__set_bit(speed,   	bits);
	}

} // AQR105_set_link_mode()
//-------------------------------------------------------------------------------------------------

int AQR105_get_link_ksettings(struct net_device *netdev, struct ethtool_link_ksettings *cmd)
{
	struct bdx_priv *priv = netdev_priv(netdev);

	ENTER;

	cmd->base.port 				= PORT_TP;
	cmd->base.autoneg 			= AUTONEG_ENABLE;
#if defined(ETH_TP_MDI_AUTO)
	cmd->base.eth_tp_mdix		= ETH_TP_MDI_AUTO;
	cmd->base.eth_tp_mdix_ctrl	= ETH_TP_MDI_AUTO;
#else
#if (!defined VM_KLNX)
	cmd->base.eth_tp_mdix 		= ETH_TP_MDI | ETH_TP_MDI_X;
	cmd->base.eth_tp_mdix_ctrl	= ETH_TP_MDI | ETH_TP_MDI_X;
#endif
#endif
	AQR105_set_link_mode(cmd->link_modes.supported, AQR105_ALL_SPEEDS);

	if (priv->autoneg == AUTONEG_ENABLE)
	{
		memcpy(priv->link_advertising, cmd->link_modes.supported, sizeof(priv->link_advertising));
	}
	memcpy(cmd->link_modes.advertising, priv->link_advertising, sizeof(cmd->link_modes.advertising));

	RET(0);

} // AQR105_get_link_ksettings()

#endif
//-------------------------------------------------------------------------------------------------
#ifdef ETHTOOL_SLINKSETTINGS

int AQR105_set_link_ksettings(struct net_device *netdev, const struct ethtool_link_ksettings *cmd)
{
	struct bdx_priv *priv = netdev_priv(netdev);
	int				rVal  = 0;

	ENTER;

	priv->autoneg = cmd->base.autoneg;
	if (priv->autoneg == AUTONEG_ENABLE)
	{
		AQR105_set_link_mode(priv->link_advertising, AQR105_ALL_SPEEDS);
	}
	else
	{
		switch (cmd->base.speed)
		{
			case 10000: //10G
				AQR105_set_link_mode(priv->link_advertising, ETHTOOL_LINK_MODE_10000baseT_Full_BIT);
				break;

			case 5000: //5G
//				AQR105_set_link_mode(priv->link_advertising, ETHTOOL_LINK_MODE_5000baseT_Full_BIT);
				break;

			case 2500: //2.5G
//				AQR105_set_link_mode(priv->link_advertising, ETHTOOL_LINK_MODE_2500baseT_Full_BIT);
				break;

			case 1000:  //1G
				AQR105_set_link_mode(priv->link_advertising, ETHTOOL_LINK_MODE_1000baseT_Full_BIT);
				break;

			case 100:   //100m
				AQR105_set_link_mode(priv->link_advertising, ETHTOOL_LINK_MODE_100baseT_Full_BIT);
				break;

			default :
				ERR("does not support speed %u\n", cmd->base.speed);
				rVal = -EINVAL;
				break;
		}
	}
	if (rVal == 0)
	{
		rVal = AQR105_set_speed(priv, cmd->base.speed);
	}

	RET(0);

} // AQR105_set_link_ksettings()

#endif
//-------------------------------------------------------------------------------------------------

__init void AQR105_register_settings(struct bdx_priv *priv)
{
    priv->phy_ops.get_settings 		 = AQR105_get_settings;
    priv->phy_ops.set_settings 		 = AQR105_set_settings;
#ifdef ETHTOOL_GLINKSETTINGS
    priv->phy_ops.get_link_ksettings = AQR105_get_link_ksettings;
#endif
#ifdef ETHTOOL_SLINKSETTINGS
    priv->phy_ops.set_link_ksettings = AQR105_set_link_ksettings;
#endif
    priv->autoneg     				 = AUTONEG_ENABLE;

} // MV88X3120_register_settings()

//-------------------------------------------------------------------------------------------------

