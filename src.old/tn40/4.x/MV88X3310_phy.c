#include "tn40.h"
#include "MV88X3310_phy.h"
#include "MV88E2010_phy.h"
#define MACTYPE (0x1)
#define	MV88X3310_FILE_HEADER_SIZE	(16)
#define MANUF_MODEL_NUM_MASK        (0x3F)
#define MANUF_MODEL_NUM_BIT_POS     (4)
#define MV88X3120_FILE_HEADER_SIZE  (16)
//#define SPEED_RES_10M				(0x0000)		// Not supported
#define SPEED_RES_100M 				(0x0001)
#define SPEED_RES_1GIG 				(0x0002)
#define SPEED_RES_NBASE   			(0x0003)
#define SPEED_RES_10GIG 			(0x0000)
#define SPEED_RES_5GIG 				(0x0002)
#define SPEED_RES_2P5GIG 			(0x0001)
#define	MV88X3310_10M_MASK			((1 << 5) | (1 << 6))		// reg7_0010
#define	MV88X3310_100M_MASK			(1 << 8)					// 		"
#define	MV88X3310_1G_MASK			(1 << 9)					// reg7_8000
#define	MV88X3310_2_5G_MASK			((1 << 5) | (1 << 7))		// reg7_0020
#define	MV88X3310_5G_MASK			((1 << 6) | (1 << 8))		// 		"
#define	MV88X3310_10G_MASK			(1 << 12)					// 		"
#define MV88X3310_LED_0				(0xf020)
#define MV88X3310_LED_1				(0xf021)
#define MV88X3310_LED_2				(0xf022)
#define MV88X3310_LED_3				(0xf023)

#define SW_RESET_COUNT				(100)
#define LINK_LOOP_MAX   			(25)
#define PHY_NAME					((priv->deviceId == 0x4027) ? "MV88X3310" : "MV88E2010")
enum PHY_LED_COLOR
{
	PHY_LED_OFF,
    PHY_LED_GREEN,
    PHY_LED_AMBER,
};
enum PHY_SPEED
{
	PHY_SPEED_LOW,
	PHY_SPEED_HIGH,
};

void MV88X3310_register_settings(struct bdx_priv *priv);
int  MV88X3310_mdio_reset(struct bdx_priv *priv, int port,  unsigned short phy);
u32  MV88X3310_link_changed(struct bdx_priv *priv);
void MV88X3310_leds(struct bdx_priv *priv, enum PHY_LEDS_OP op);

//-------------------------------------------------------------------------------------------------

int MV88X3310_set_speed(struct bdx_priv *priv, signed int speed)
{

	u16 val, port=priv->phy_mdio_port;
	u16 reg7_0010 = PHY_MDIO_READ(priv,7,0x0010);
	u16 reg7_0020 = PHY_MDIO_READ(priv,7,0x0020);
	u16 reg7_8000 = PHY_MDIO_READ(priv,7,0x8000);
	int rVal = 0;

	if(priv->autoneg == AUTONEG_ENABLE)
	{
		DBG("MV88X3310 speed 10G/1G/100m Autoneg\n");
        reg7_0010 = (reg7_0010 & ~MV88X3310_10M_MASK) | MV88X3310_100M_MASK;
        reg7_0020 = reg7_0020 | MV88X3310_2_5G_MASK | MV88X3310_5G_MASK | MV88X3310_10G_MASK;
        reg7_8000 = reg7_8000 | MV88X3310_1G_MASK;

	}
	else
	{
		switch(speed)
		{
			case 10000: //10G
				DBG("MV88X3310 speed 10G\n");
		        reg7_0010 = reg7_0010 & ~MV88X3310_10M_MASK & ~MV88X3310_100M_MASK;
		        reg7_0020 = (reg7_0020 & ~MV88X3310_2_5G_MASK & ~MV88X3310_5G_MASK) | MV88X3310_10G_MASK;
		        reg7_8000 = reg7_8000 & ~MV88X3310_1G_MASK;
				break;

			case 5000: //5G
				DBG("MV88X3310 speed 5G\n");
		        reg7_0010 = reg7_0010 & ~MV88X3310_10M_MASK & ~MV88X3310_100M_MASK;
		        reg7_0020 = (reg7_0020 & ~MV88X3310_2_5G_MASK & ~MV88X3310_10G_MASK) | MV88X3310_5G_MASK;
		        reg7_8000 = reg7_8000 & ~MV88X3310_1G_MASK;
				break;

			case 2500: //2.5G
				DBG("MV88X3310 speed 2.5G\n");
		        reg7_0010 = reg7_0010 & ~MV88X3310_10M_MASK & ~MV88X3310_100M_MASK;
		        reg7_0020 = (reg7_0020 & ~MV88X3310_5G_MASK & ~MV88X3310_10G_MASK) | MV88X3310_2_5G_MASK;
		        reg7_8000 = reg7_0020 & ~MV88X3310_1G_MASK;
				break;

			case 1000:  //1G
				DBG("MV88X3310 speed 1G\n");
		        reg7_0010 = reg7_0010 & ~MV88X3310_10M_MASK & ~MV88X3310_100M_MASK;
		        reg7_0020 = reg7_0020 & ~MV88X3310_2_5G_MASK & ~MV88X3310_10G_MASK & ~MV88X3310_5G_MASK;
		        reg7_8000 = reg7_8000 | MV88X3310_1G_MASK;
				break;

			case 100:   //100m
				DBG("MV88X3310 speed 100m\n");
		        reg7_0010 = (reg7_0010 & ~MV88X3310_10M_MASK) | MV88X3310_100M_MASK;
		        reg7_0020 = reg7_0020 & ~MV88X3310_2_5G_MASK & ~MV88X3310_10G_MASK & ~MV88X3310_5G_MASK;
		        reg7_8000 = reg7_8000 & ~MV88X3310_1G_MASK;
				break;

			default :
				ERR("does not support speed %u\n", speed);
				rVal= -1;
				break;
		}
	}
	if (rVal == 0)
	{
		// set speed
		DBG("writing 7,0x0010 0x%04x 7,0x0020 0x%04x 7,0x8000 0x%04x\n", (u32)reg7_0010, (u32)reg7_0020, (u32)reg7_8000);
		BDX_MDIO_WRITE(priv, 7, 0x0010, reg7_0010);
		BDX_MDIO_WRITE(priv, 7, 0x0020, reg7_0020);
		BDX_MDIO_WRITE(priv, 7, 0x8000, reg7_8000);
		// restart autoneg
		val = PHY_MDIO_READ(priv,7,0);
		val = val | 0x1200;
		BDX_MDIO_WRITE(priv, 0x07,0x0,val);
	}

	return rVal;

} // MV88X3310_set_speed()

//-------------------------------------------------------------------------------------------------

static int  MV88X3310_set_Reset_values(struct bdx_priv *priv)
{
	u16 val;
	u16 port=priv->phy_mdio_port;
	u16 port_ctrl = PHY_MDIO_READ(priv,31,0xF001);
	u16 reg7_0010 = PHY_MDIO_READ(priv, 7,0x0010);
	u16 reg7_0020 = PHY_MDIO_READ(priv, 7,0x0020);
	u16 reg7_8000 = PHY_MDIO_READ(priv, 7,0x8000);

    reg7_0010 = (reg7_0010 & ~MV88X3310_10M_MASK) | MV88X3310_100M_MASK;
    reg7_0020 = reg7_0020 | MV88X3310_2_5G_MASK | MV88X3310_5G_MASK | MV88X3310_10G_MASK;
    reg7_8000 = reg7_8000 | MV88X3310_1G_MASK;

	DBG("writing 7,0x0010 0x%04x 7,0x0020 0x%04x 7,0x8000 0x%04x\n", (u32)reg7_0010, (u32)reg7_0020, (u32)reg7_8000);
	BDX_MDIO_WRITE(priv, 7, 0x0010, reg7_0010);
	BDX_MDIO_WRITE(priv, 7, 0x0020, reg7_0020);
	BDX_MDIO_WRITE(priv, 7, 0x8000, reg7_8000);

	port_ctrl |= MACTYPE ;					// XAUI
	port_ctrl |= (1 << 15);			// SW reset
	BDX_MDIO_WRITE(priv, 31, 0xF001, port_ctrl);

	val = PHY_MDIO_READ(priv,7,0);
	val |= ((1 << 9) | (1 << 12));	// Enable and restart AN
	BDX_MDIO_WRITE(priv, 7, 0, val);
	DBG("MACTYPE = 0x%x\n",bdx_mdio_read(priv, 31, port, 0xF001) & 0x0007);

	return 0;

} // MV88X3310_set_Reset_values()

//-------------------------------------------------------------------------------------------------

static int  MV88X3310_set_rate_adapt(struct bdx_priv *priv, enum PHY_SPEED phy_speed)
{
	u16 port = priv->phy_mdio_port;

	// X3310P/E2010P Rate Adaptation configuration
	BDX_MDIO_WRITE(priv, 31, 0xF000, 0x30C8);
	bdx_mdio_read(priv, 31, port, 0xF001);
	bdx_mdio_read(priv, 31, port, 0xF001);
	BDX_MDIO_WRITE(priv, 31, 0x8000, 0x0F3F);
	BDX_MDIO_WRITE(priv, 31, 0x8001, 0x7DD0);
	bdx_mdio_read(priv, 31, port, 0xF001);
	bdx_mdio_read(priv, 31, port, 0xF001);
	bdx_mdio_read(priv, 31, port, 0xF001);
	bdx_mdio_read(priv, 31, port, 0xF001);
	bdx_mdio_read(priv, 31, port, 0xF001);
	BDX_MDIO_WRITE(priv, 31, 0x8C02, 0x1FFF);
	BDX_MDIO_WRITE(priv, 31, 0x8C03, 0x0000);
	if (phy_speed == PHY_SPEED_HIGH)
	{
		DBG("high_speed\n");
		BDX_MDIO_WRITE(priv, 31, 0x8E02, 0x1FFF);
		BDX_MDIO_WRITE(priv, 31, 0x8E03, 0x0000);
	}
	else
	{
		DBG("lower Speeds\n");
		BDX_MDIO_WRITE(priv, 31, 0x8E00, 0x7FFF);
		BDX_MDIO_WRITE(priv, 31, 0x8E01, 0x0000);
	}
	BDX_MDIO_WRITE(priv, 31, 0x8C00, 0x1383);
	BDX_MDIO_WRITE(priv, 31, 0x8C01, 0x0000);
	BDX_MDIO_WRITE(priv, 31, 0x8C04, 0x1C00);
	BDX_MDIO_WRITE(priv, 31, 0x8C05, 0x0000);
	BDX_MDIO_WRITE(priv, 31, 0x800E, 0x1B0A);
	BDX_MDIO_WRITE(priv, 31, 0x800F, 0x0000);
	BDX_MDIO_WRITE(priv, 31, 0x80A2, 0x0001);
	BDX_MDIO_WRITE(priv, 31, 0x80A3, 0x0000);

	return 0;

} // MV88X3310_set_rate_adapt()

//-------------------------------------------------------------------------------------------------

#ifdef _
#ifdef ETHTOOL_SEEE_
int MV88X3310_set_eee(struct bdx_priv *priv/*, struct ethtool_eee *edata*/);
#endif
#endif
__init int MV88X3310_mdio_reset(struct bdx_priv *priv, int port,  unsigned short phy)
{
    unsigned short  val, val1;
    int             rVal = 0;
    u32             phy_firmware_size, j;
    u16				*phy_firmware;
    u16				expected_checksum = 0;

    ENTER;
    if (priv->deviceId == 0x4027)
    {
    	phy_firmware 		= MV88X3310_phy_firmware;
    	phy_firmware_size 	= sizeof(MV88X3310_phy_firmware)/sizeof(u16);
    }
    else
    {
    	phy_firmware 		= MV88E2010_phy_firmware;
    	phy_firmware_size 	= sizeof(MV88E2010_phy_firmware)/sizeof(u16);

    }
    val  = bdx_mdio_read(priv, 1, port, 3);
    val1 = val & 0x000F;                                           // revNo
    val  = (val & (MANUF_MODEL_NUM_MASK << MANUF_MODEL_NUM_BIT_POS)) >> MANUF_MODEL_NUM_BIT_POS; // modelNo
    DBG("%s modelNo = %d revNo %d\n", PHY_NAME, val, val1);
    do
    {
        val = bdx_mdio_read(priv, 31, port, 0xF008);
        BDX_MDIO_WRITE(priv, 31,0xF008, (val | 1<<5));
        val = bdx_mdio_read(priv, 31, port, 0xF001);
        BDX_MDIO_WRITE(priv, 31,0xF001, (val | 1<<12));                         // Reset
        msleep(250);                                                    		// Allow reset to complete
        // Make sure we can access the DSP
        // and it's in the correct mode (waiting for download)
        if ((val = bdx_mdio_read(priv, 1, port, 0xC050)) != 0x000A)
        {
            ERR("%s DSP is not in waiting on download mode. Expected 0x000A, read 0x%04X\n",PHY_NAME, (unsigned)val);
            rVal = -1;
            break;
        }
        else
        {
            DBG("%s Downloading code...\n", PHY_NAME);
        }
        val = bdx_mdio_read(priv, 3, port, 0xD0F3);							// Clear the checksum
        BDX_MDIO_WRITE(priv, 3,0xD0F0,0);                                     	// Set starting address in RAM to 0x00100000 */
        BDX_MDIO_WRITE(priv, 3,0xD0F1,0x0010);
        for (j = MV88X3310_FILE_HEADER_SIZE; j < phy_firmware_size; j++)
        {
            val = swab16(phy_firmware[j]);
            expected_checksum += (val & 0x00ff) + ((val & 0xff00) >> 8);
            BDX_MDIO_WRITE(priv, 3, 0xD0F2, val);
        }
        DBG("%s loaded %d 16bit words\n", PHY_NAME, phy_firmware_size);
        val = bdx_mdio_read(priv, 1, port, 0xC050);
        BDX_MDIO_WRITE(priv, 1, 0xC050,(val | (1<<6) ));                       	// Code uploaded
        msleep(500);                                                    		// Give firmware code time to start
        // make sure the firmware code started
        if (!((val = bdx_mdio_read(priv,1, port, 0xC050)) & (1<<4)))    		// Application started
        {
            ERR("%s firmware code did not start. Expected bit 4 to be 1, read 0x%04X\n",PHY_NAME, (unsigned)val);
            rVal = -1;
            break;
        }
        else
        {
            MSG("%s firmware code is running\n", PHY_NAME);
        }
        val = bdx_mdio_read(priv, 3, port, 0xD0F3);
        if ((phy_firmware[4] != swab16(~expected_checksum)) || (swab16(~val) != swab16(~expected_checksum)))
        {
			ERR("F/W checksum error ! file checksum is 0x%04x F/W checksum is 0x%04x expected checksum is 0x%04x\n",
					phy_firmware[4], swab16(~val), swab16(~expected_checksum));
            rVal = -1;
            break;
        }
        val  = bdx_mdio_read(priv, 1, port, 0xc011);
        val1 = bdx_mdio_read(priv, 1, port, 0xc012);
        MSG("%s FW version is %d.%d.%d.%d\n", PHY_NAME,
        	((val & 0xff00) >> 8), (val & 0x00ff), ((val1 & 0xff00) >> 8), (val1 & 0x00ff));
        val = bdx_mdio_read(priv,1, port, 0);
        BDX_MDIO_WRITE(priv,1, 0, val | (1 << 15));
        // verify reset complete
        for (j = 0; j < SW_RESET_COUNT; j++)
        {
            if (!((val = bdx_mdio_read(priv,1, port, 0)) & (1<<15)))
            {
                break;
            }
            DBG("%s waiting for reset complete (1.0.15) 0x%x\n", PHY_NAME, val);
            msleep(10);
        }
        if (j == SW_RESET_COUNT)
        {
        	ERR("%s SW reset was not completed (1.0.15) 0x%x\n", PHY_NAME, val);
        }
        MV88X3310_set_Reset_values(priv);
        BDX_MDIO_WRITE(priv, 3,0x8010, (1 << 10) | (1 << 11));             	// Enable LASI auto negotiation complete

    } while (0);

    RET(rVal);

} // MV88X3310_mdio_reset

//-------------------------------------------------------------------------------------------------

static void MV88X3310_led(struct bdx_priv *priv, enum PHY_LED_COLOR color)
{
	u16	led1val, led2val;

	if (color == PHY_LED_GREEN)
	{
		led1val = 0;
		led2val = 0xb8;
	}
	else if (color == PHY_LED_AMBER)
	{
		led1val = 0xb8;
		led2val = 0;
	}
	else
	{
		led1val = led2val = 0;
	}
	bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_1, led1val);
	bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_2, led2val);

} // MV88X3310_led()

//-------------------------------------------------------------------------------------------------

static int  MV88X3310_get_link_speed(struct bdx_priv *priv)
{
    unsigned short  status;
    int             resolved, duplex, speed, link = 0;

    ENTER;
    status = bdx_mdio_read(priv, 7, priv->phy_mdio_port, 1);
    DBG("%s 7.1 = 0x%x\n", PHY_NAME, status);

    // Clear C Unit interrupts
    PHY_MDIO_READ(priv,3,0x8011);
    PHY_MDIO_READ(priv,3,0x8011);

    if (status & (1 << 2))                                         // Link up
    {
        status  	= bdx_mdio_read(priv, 3, priv->phy_mdio_port, 0x8008);
        resolved	= status & (1 << 11);
        duplex 		= status & (1 << 13);
        speed		= (status & 0xc000) >> 14;
        if (resolved)
        {
        	DBG("%s speed %d %s duplex\n", PHY_NAME, speed, duplex ? "full" : "half");
			switch(speed)
			{
				case SPEED_RES_NBASE    :
					MV88X3310_set_rate_adapt(priv, PHY_SPEED_HIGH);
					speed = (status & 0x000c) >> 2;
					switch (speed)
					{
						case SPEED_RES_10GIG:
						link = SPEED_10000;
						MV88X3310_led(priv, PHY_LED_AMBER);
						DBG("%s 10G link detected\n", PHY_NAME);
						break;

						case SPEED_RES_5GIG:
						link = SPEED_5000;
						MV88X3310_led(priv, PHY_LED_GREEN);
						DBG("%s 5G link detected\n", PHY_NAME);
						break;

						case SPEED_RES_2P5GIG:
						link = SPEED_2500;
						MV88X3310_led(priv, PHY_LED_GREEN);;
						DBG("%s 2.5G link detected\n", PHY_NAME);
						break;

						default:
							DBG("%s internal error - unknown link speed value (0x%x) !\n", PHY_NAME, status);
							break;
					}
					break;

				case SPEED_RES_1GIG   :
					link = SPEED_1000X;
					MV88X3310_set_rate_adapt(priv, PHY_SPEED_LOW);
					MV88X3310_led(priv, PHY_LED_GREEN);
					DBG("%s 1G link detected\n", PHY_NAME);
					break;

				case SPEED_RES_100M   :
					link = SPEED_100X;
					MV88X3310_set_rate_adapt(priv, PHY_SPEED_LOW);
					MV88X3310_led(priv, PHY_LED_OFF);
					DBG("%s 100M link detected\n", PHY_NAME);
					break;

				default:
					DBG("%s internal error - unknown link speed value (0x%x) !\n", PHY_NAME, status);
					MV88X3310_led(priv, PHY_LED_OFF);
					break;
			}
        }
        else
        {
			DBG("%s auto negotiation in progress...\n", PHY_NAME);
			MV88X3310_led(priv, PHY_LED_OFF);

        }
        priv->errmsg_count = 0;
    }
    else
    {
    	if (++priv->errmsg_count < MAX_ERRMSGS)
    	{
			MV88X3310_led(priv, PHY_LED_OFF);
    		DBG("%s link down.\n", PHY_NAME);
    	}
    }

    RET(link);

} // MV88X3310_get_link_speed()

//-------------------------------------------------------------------------------------------------

u32 MV88X3310_link_changed(struct bdx_priv *priv)
{
    u32 link, speed;

    speed = MV88X3310_get_link_speed(priv);
    DBG("speed %d priv->link_speed %d\n", speed, priv->link_speed);
	if(speed != (u32)priv->link_speed)
	{
		switch (speed)
		{
			case SPEED_10000:
				DBG("%s 10G link detected\n", PHY_NAME);
#ifdef _EEE_
				ERR("DBG_EEE eee_enabled=%d\n", priv->eee_enabled);
				if(priv->eee_enabled)
				{
				}

				else
				{
#if defined(_EEE) && defined(ETHTOOL_SEEE)
					MV88X3310_set_eee(priv/*, edata*/);
#endif
				}
#endif
			break;
			case SPEED_1000:
				DBG("%s 1G link detected\n", PHY_NAME);
				break;
			case SPEED_1000X:
				DBG("%s 1GX link detected\n", PHY_NAME);
				break;
			case SPEED_5000:
				DBG("%s 5G link detected\n", PHY_NAME);
				break;
			case SPEED_2500:
				DBG("%s 2.5G link detected\n", PHY_NAME);
				break;
			case SPEED_100:
				DBG("%s 100M link detected\n", PHY_NAME);
				break;
			case SPEED_100X:
				DBG("%s 100MX link detected\n", PHY_NAME);
				break;
			default:
				DBG("%s link down.\n", PHY_NAME);
				break;
		}
		bdx_speed_changed(priv,speed);
	}
	link = 0;
	if((!speed) || (!(link =  (READ_REG(priv, regMAC_LNK_STAT) & MAC_LINK_STAT))))  // // XAUI link
	{
		u32 timeout;
		if (speed)
		{
			timeout = 1000000;      // 1/5 sec
			if(priv->link_loop_cnt++ > LINK_LOOP_MAX)
			{
				bdx_speed_changed(priv,0);
				priv->link_loop_cnt = 0;
				DBG("%s trying to recover link after %d tries\n", PHY_NAME, LINK_LOOP_MAX);
			}
		}
		else
		{
			timeout = 5000000;      // 1 sec
		}
		DBG("%s link = 0x%x speed = 0x%x setting %d timer\n", PHY_NAME, link, speed, timeout);
		WRITE_REG(priv, 0x5150,timeout);
	}

	return link;

} // MV88X3310_link_changed()

//-------------------------------------------------------------------------------------------------

void MV88X3310_leds(struct bdx_priv *priv, enum PHY_LEDS_OP op)
{
	switch (op)
	{
		case PHY_LEDS_SAVE:
			priv->phy_ops.leds[0] = bdx_mdio_read(priv, 31, priv->phy_mdio_port, MV88X3310_LED_0);
			priv->phy_ops.leds[1] = bdx_mdio_read(priv, 31, priv->phy_mdio_port, MV88X3310_LED_1);
			priv->phy_ops.leds[2] = bdx_mdio_read(priv, 31, priv->phy_mdio_port, MV88X3310_LED_2);
			priv->phy_ops.leds[3] = bdx_mdio_read(priv, 31, priv->phy_mdio_port, MV88X3310_LED_3);
			break;

		case PHY_LEDS_RESTORE:
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_0, priv->phy_ops.leds[0]);
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_1, priv->phy_ops.leds[1]);
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_2, priv->phy_ops.leds[2]);
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_3, priv->phy_ops.leds[3]);
			break;

		case PHY_LEDS_ON:
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_0, 0xb8);
			//bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_1, 0xb8);
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_2, 0xb8);
			//bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_3, 0xb8);
			break;

		case PHY_LEDS_OFF:
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_0, 0);
			//bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_1, 0);
			bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_2, 0);
			//bdx_mdio_write(priv, 31, priv->phy_mdio_port, MV88X3310_LED_3, 0);
			break;

		default:
			DBG("unknown op 0x%x\n", op);
			break;

	}

} // MV88X3310_leds()

//-------------------------------------------------------------------------------------------------

__init enum PHY_TYPE MV88X3310_register(struct bdx_priv *priv)
{
	enum PHY_TYPE phyType;

    priv->isr_mask= IR_RX_FREE_0 | IR_LNKCHG0 |IR_LNKCHG1 | IR_PSE | IR_TMR0 | IR_RX_DESC_0 | IR_TX_FREE_0;
    priv->phy_ops.mdio_reset   = MV88X3310_mdio_reset;
    priv->phy_ops.link_changed = MV88X3310_link_changed;
    priv->phy_ops.ledset       = MV88X3310_leds;
    priv->phy_ops.mdio_speed   = MDIO_SPEED_6MHZ;
    MV88X3310_register_settings(priv);

    phyType = (priv->deviceId == 0x4027) ? PHY_TYPE_MV88X3310 : PHY_TYPE_MV88E2010;

    return phyType;

} // MV88X3310_init()

//-------------------------------------------------------------------------------------------------

