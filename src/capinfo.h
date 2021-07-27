/*
 * capinfo.h
 *
 *  Created on: Jul 26, 2021
 *      Author: dima
 */

#ifndef CAPINFO_H_
#define CAPINFO_H_
#include <stdint.h>
#include <linux/i2c-dev.h> //#Make sure to copy corresponding smbus.c from i2c-tools to source dir!
#include <errno.h>
#include <stdio.h>
#include "LTC3350/LTC3350.h"

	typedef union _chrg_status_reg
	{
	    struct
	    {
	        uint16_t chrg_stepdown                      :1;
	        uint16_t chrg_stepup                        :1;
	        uint16_t chrg_cv                   			:1;
	        uint16_t chrg_uvlo                        	:1;
	        uint16_t chrg_input_ilim                    :1;
	        uint16_t chrg_cappg                         :1;
	        uint16_t chrg_shnt                          :1;
	        uint16_t chrg_bal                          	:1;

	        uint16_t chrg_dis                           :1;
	        uint16_t chrg_ci                            :1;
	        uint16_t not_used                    		:1;
	        uint16_t chrg_pfo                        	:1;
	        uint16_t reserved1                       	:1;
	        uint16_t reserved2                          :1;
	        uint16_t reserved3                          :1;
	        uint16_t reserved4                          :1;
	    };// __attribute__ ((packed));

	    uint16_t reg;
	} chrg_status_reg;

	typedef union _mon_status_reg
	{
	    struct
	    {
	        uint16_t mon_capesr_active                  :1;
	        uint16_t mon_capesr_scheduled               :1;
	        uint16_t mon_capesr_pending                 :1;
	        uint16_t mon_cap_done                       :1;
	        uint16_t mon_esr_done                       :1;
	        uint16_t mon_cap_failed                     :1;
	        uint16_t mon_esr_failed                     :1;
	        uint16_t not_used                          	:1;

	        uint16_t mon_power_failed                   :1;
	        uint16_t mon_power_returned                 :1;
	        uint16_t reserved1                    		:1;
	        uint16_t reserved2                        	:1;
	        uint16_t reserved3                       	:1;
	        uint16_t reserved4                          :1;
	        uint16_t reserved5                          :1;
	        uint16_t reserved6                          :1;
	    };// __attribute__ ((packed));

	    uint16_t reg;
	} mon_status_reg;

	typedef struct _Capstate
	{
		double vin;
		uint16_t vin_reg;
		double vout;
		uint16_t vout_reg;
		double iin;
		uint16_t iin_reg;
		double ichrg;
		uint16_t ichrg_reg;
		double vcap1;
		uint16_t vcap1_reg;
		double vcap2;
		uint16_t vcap2_reg;
		double vcap3;
		uint16_t vcap3_reg;
		double vcap4;
		uint16_t vcap4_reg;
		double vcap;
		uint16_t vcap_reg;
		uint8_t num_caps;
		uint16_t cap_meas;
		mon_status_reg mon_status;
		chrg_status_reg chrg_status;
	} Capstate;



class CapInfo
{

public:
	CapInfo();
	~CapInfo();

	static int read_register(uint8_t addr, //!< Target IC's SMBus address
	                  uint8_t command_code, //!< Command Code to be read from
	                  uint16_t *data, //!< Pointer to data destination
	                  port_configuration_t *port_configuration //!< Pointer to port configuration struct
	                 );
	static int write_register(uint8_t addr, //!< Target IC's SMBus address
            uint8_t command_code, //!< Command Code to be written to
            uint16_t data, //!< Data to be written
            port_configuration_t *port_configuration //!< Pointer to port configuration struct
           );


	Capstate *getCapInfo();

private:

	Capstate capstate;

	//chrg_status_reg chrg_status;
	//mon_status_reg mon_status;
	int fd;
	port_configuration_t pc;
	LTC3350_chip_cfg_t cfg;
	LTC3350 chip;
};


#endif /* CAPINFO_H_ */
