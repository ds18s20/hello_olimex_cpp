/*
 * capinfo.cpp
 *
 *  Created on: Jul 26, 2021
 *      Author: dima
 */
#define _BSD_SOURCE
#include "capinfo.h"
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>
#include <iomanip>
#include <stdio.h>

extern "C" {
	#include <smbus.h>
	#include <linux/i2c-dev.h>
}

#define R_SNSI				0.015
#define R_SNSC				0.005

#define VCAP_LSB			0.0001835
#define VCAP_ALL_LSB	 	0.001476
#define VIN_LSB				0.00221
#define VOUT_LSB			0.00221
#define IIN_LSB				(0.000001983/R_SNSI)
#define ICHRG_LSB			(0.000001983/R_SNSC)
#define VSHUNT_LSB			0.0001835

using namespace std;

CapInfo::CapInfo()
{
	  fd = open("/dev/i2c-0", O_RDWR);
	  if (fd < 0)
	  {
	    std::cout<<"Error opening file: "<<endl;
	  }
	  if (ioctl(fd, I2C_PEC, 0) < 0) //set non-zero to enable PEC
	  {
		  std::cout<<"Error disabling Packet Error Checking: %s\n"<<endl;
	  }

	pc.file_descriptor = fd;

	cfg.addr = LTC3350_ADDR_09;
	cfg.read_register = read_register;
	cfg.write_register = write_register;
	cfg.port_configuration = &pc;

	chip = LTC3350_init(&cfg);
}

int CapInfo::read_register(uint8_t addr, uint8_t command_code, uint16_t *data, port_configuration_t *port_configuration)
{
	  if (ioctl(port_configuration->file_descriptor, I2C_SLAVE, addr) < 0)
	  {
	    printf("Error setting slave address: %s\n", strerror(errno));
	    return 1;
	  }
	  //__s32 i2c_smbus_read_byte_data(int file, __u8 command);
	  //__s32 i2c_smbus_read_word_data(int file, __u8 command);
	  __s32 ret_val = i2c_smbus_read_word_data(port_configuration->file_descriptor, command_code);
	  if (ret_val == -1)
	  {
	    printf("Read error: %s\n", strerror(errno));
	    return 1;
	  }
	  *data = (uint16_t)ret_val;
	  return 0;
}

int CapInfo::write_register(uint8_t addr, uint8_t command_code, uint16_t data, port_configuration_t *port_configuration)
{
	  if (ioctl(port_configuration->file_descriptor, I2C_SLAVE, addr) < 0)
	  {
	    printf("Error setting slave address: %s\n", strerror(errno));
	    return 1;
	  }
	  //__s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value);
	  //__s32 i2c_smbus_write_word_data(int file, __u8 command, __u16 value);
	  __s32 ret_val = i2c_smbus_write_word_data(port_configuration->file_descriptor, command_code, data);
	  if (ret_val == -1)
	  {
	    printf("Write error: %s\n", strerror(errno));
	    return 1;
	  }
	  return 0;
}

Capstate* CapInfo::getCapInfo()
{
	uint16_t data;

	LTC3350_read_register(chip, LTC3350_NUM_CAPS, &data);
	capstate.num_caps = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VIN , &data);
	capstate.vin = data*VIN_LSB;
	capstate.vin_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VOUT , &data);
	capstate.vout = data*VOUT_LSB;
	capstate.vout_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_IIN  , &data);
	capstate.iin = data*IIN_LSB;
	capstate.iin_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_ICHG , &data);
	capstate.ichrg = data*ICHRG_LSB;
	capstate.ichrg_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VCAP1 , &data);
	capstate.vcap1 = data*VCAP_LSB;
	capstate.vcap1_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VCAP1 , &data);
	capstate.vcap2 = data*VCAP_LSB;
	capstate.vcap2_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VCAP1 , &data);
	capstate.vcap3 = data*VCAP_LSB;
	capstate.vcap3_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VCAP1 , &data);
	capstate.vcap4 = data*VCAP_LSB;
	capstate.vcap4_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_VCAP , &data);
	capstate.vcap = data*VCAP_ALL_LSB;
	capstate.vcap_reg = data;
	LTC3350_read_register(chip, LTC3350_MEAS_CAP , &data);
	capstate.cap_meas = data;
	LTC3350_read_register(chip, LTC3350_CHRG_STATUS , &data);
	capstate.chrg_status.reg = data;

	return &capstate;
}

CapInfo::~CapInfo()
{

}
