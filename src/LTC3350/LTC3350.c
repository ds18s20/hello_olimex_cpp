/*!
LTC3350: High Current Supercapacitor Backup Controller and System Monitor

@verbatim
The  LTC®3350 is a backup power controller that can charge and monitor a series
stack  of  one  to  four  super-capacitors.  The LTC3350's synchronous step-down
controller  drives  N-channel  MOSFETs  for  constant  current/constant  voltage
charging  with  programmable  input  current  limit.  In addition, the step-down
converter  can  run  in reverse as a step-up converter to deliver power from the
supercapacitor stack to the backup supply rail. Internal balancers eliminate the
need for external balance resistors and each capacitor has a shunt regulator for
overvoltage  protection.  The  LTC3350 monitors system voltages, currents, stack
capacitance  and  stack  ESR which can all be read over the I²C/SMBus. The dual
ideal  diode controller uses N-channel MOSFETs for low loss power paths from the
input and super-capacitors to the backup system supply. The LTC3350 is available
in a low profile 38-lead 5mm × 7mm × 0.75mm QFN surface mount package.
@endverbatim

http://www.linear.com/product/LTC3350

http://www.linear.com/product/LTC3350#demoboards

REVISION HISTORY
$Revision: $
$Date: $

Copyright (c) 2016, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1.  Redistributions  of source code must retain the above copyright notice, this
    list  of conditions and the following disclaimer.

2.  Redistributions  in  binary  form must reproduce the above copyright notice,
    this  list of conditions and  the following disclaimer in the  documentation
    and/or other materials provided with the distribution.

THIS  SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY  EXPRESS  OR  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES   OF  MERCHANTABILITY  AND  FITNESS  FOR  A  PARTICULAR  PURPOSE  ARE
DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY  DIRECT,  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING,  BUT  NOT  LIMITED  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS  OF  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT  LIABILITY,  OR TORT
(INCLUDING  NEGLIGENCE  OR  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The  views and conclusions contained in the software and documentation are those
of  the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However,  the Linduino is only possible because of the Arduino team's commitment
to   the   open-source   community.   Please,  visit  http://www.arduino.cc  and
http://store.arduino.cc  ,  and  consider  a  purchase that will help fund their
ongoing work.

Generated on: 2016-01-08
*/


//! @defgroup LTC3350 LTC3350: High Current Supercapacitor Backup Controller and System Monitor

/*! @file
 *  @ingroup LTC3350
 *  @brief LTC3350 lightweight, hardware ambiguous, embeddable C Communication
 *  Library.
 *
 * Communication  is  bit-field based as well as whole-register based. This library
 * automatically masks and right-justifies bit fields to  ease programmer workload.
 *
 * This C library provides a header file with the complete definitions of registers
 * and  bit  fields  within  those  registers, and routines to read and write those
 * registers and individual bit-fields.
 *
 * Outside  of  the  Linduino  environment,  this  library  can  be  built with any
 * standard  C  compiler  and  can  be  used with different I2C/SMBus communication
 * hardware  simply  by  swapping  out  the  pointers  to appropriate user-supplied
 * functions   *@ref   smbus_write_register   and   *@ref  smbus_read_register.  An
 * example  is  provided  using  the  i2c-tools  Linux kernel driver which has been
 * tested  on  the  BeagleBone  Black  Linux  single board computer. It can also be
 * readily  adapted  to  common  microcontrollers  with  minimal  memory  impact on
 * embedded systems.
 *
 * A higher level hardware ambiguous Python communication library is also available.
 *
 * Please   visit   http://www.linear.com/product/LTC3350#code  or  contact  the
 * factory at 408-432-1900 or www.linear.com for further information.
 */

#include "LTC3350.h"

#ifndef LTC3350_USE_MALLOC
int LTC3350_instances = 0;
LTC3350_chip_cfg_t LTC3350_chip_array[MAX_NUM_LTC3350_INSTANCES];
#endif


//private function
LTC3350 LTC3350_alloc(void)
{
  //! this function "allocates" a LTC3350_chip structure.
  //! It may or may not use malloc.
#ifdef LTC3350_USE_MALLOC
  return malloc(sizeof(LTC3350_chip_cfg_t));
#else
  if (LTC3350_instances < MAX_NUM_LTC3350_INSTANCES)
  {
    return &LTC3350_chip_array[LTC3350_instances++];
  }
  else
  {
    return 0;
  }
#endif
}

LTC3350 LTC3350_init(LTC3350_chip_cfg_t *cfg)
{
  LTC3350_chip_cfg_t *chip = LTC3350_alloc();
  if (chip == NULL) return NULL;
  chip->addr = cfg->addr;
  chip->write_register = cfg->write_register;
  chip->read_register = cfg->read_register;
  chip->port_configuration = cfg->port_configuration;
  return (LTC3350) chip;
}

static inline uint8_t get_size(uint16_t registerinfo)
{
  return ((registerinfo >> 8) & 0x0F) + 1;
}
static inline uint8_t get_subaddr(uint16_t registerinfo)
{
  return (registerinfo) & 0xFF;
}
static inline uint8_t get_offset(uint16_t registerinfo)
{
  return (registerinfo >> 12) & 0x0F;
}
static inline uint16_t get_mask(uint16_t registerinfo)
{
  uint16_t mask = 1 << get_offset(registerinfo);
  uint8_t size = get_size(registerinfo);
  uint8_t i;
  for (i=0; i<size-1; i++)
  {
    mask |= mask << 1;
  }
  return mask;
}

int LTC3350_write_register(LTC3350 chip_handle, uint16_t registerinfo, uint16_t data)
{
  LTC3350_chip_cfg_t *chip = (LTC3350_chip_cfg_t *) chip_handle;
  int failure;
  uint8_t command_code = get_subaddr(registerinfo);
  if (get_size(registerinfo) != 16)
  {
    uint8_t offset = get_offset(registerinfo);
    uint16_t mask = get_mask(registerinfo);
    uint16_t read_data;
    failure = chip->read_register(chip->addr,command_code,&read_data,chip->port_configuration);
    if (failure) return failure;
    data = (read_data & ~mask) | (data << offset);
  }
  return chip->write_register(chip->addr,command_code,data,chip->port_configuration);
}

int LTC3350_read_register(LTC3350 chip_handle, uint16_t registerinfo, uint16_t *data)
{
  LTC3350_chip_cfg_t *chip = (LTC3350_chip_cfg_t *) chip_handle;
  int result;
  uint8_t command_code = get_subaddr(registerinfo);
  uint8_t offset = get_offset(registerinfo);
  uint16_t mask = get_mask(registerinfo);
  result = chip->read_register(chip->addr,command_code,data,chip->port_configuration);
  *data &= mask;
  *data = *data >> offset;
  return result;
}
