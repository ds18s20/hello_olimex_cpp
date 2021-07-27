/*
 * statemachine.h
 *
 *  Created on: Jul 3, 2021
 *      Author: dima
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <stdio.h>
#include "cppTimer/CppTimer.h"
#include <unistd.h>
#include <time.h>
#include <thread>
#include <ctime>
#include <fstream>
#include "led.h"
#include "capinfo.h"

#define RECEIVED_TIMEOUT		23
#define TRANSMIT_TIMEOUT		(17)


#define __IO volatile

class StateMachine : public CppTimer
{
public:
	StateMachine();
	~StateMachine();

	void timerEvent();
	void run();
	void write_log();


private:

	Led blue_led;
	Led green_led[4];
	int timeout_cnt;
	std::ofstream *logFile;
	std::string time_str;
	CapInfo capInfo;

	typedef union _tsc_flgs_ut
	{
	    struct
	    {
	        uint8_t _1_ms               :1;
	        uint8_t _10_ms              :1;
	        uint8_t _50_ms              :1;
	        uint8_t _100_ms             :1;
	        uint8_t _1_s                :1;
	        uint8_t _10_s               :1;
	        uint8_t _1_m                :1;
	        uint8_t _10_m               :1;
	    };// __attribute__ ((packed));
	    uint8_t flgs;
	} tsc_flgs_ut;

	typedef struct
	{
	    __IO tsc_flgs_ut  flg;                          //
	    __IO uint8_t  time_cadr_1_ms;                   //
	    __IO uint8_t  time_cadr_10_ms;
	    __IO uint8_t  time_cadr_100_ms;
	    __IO uint8_t  cnt_1_ms;
	    __IO uint8_t  cnt_100_ms;
	} _tsc_st;

	_tsc_st     _tsc_prm;


	typedef union _state_machine_flg
	{
	    struct
	    {
	        uint8_t start_receive                              :1;
	        uint8_t start_transmit                             :1;
	        uint8_t get_cap_info		                       :1;
	        uint8_t get_alarm_info         		               :1;
	        uint8_t process_received_data     		           :1;
	        uint8_t receive_in_progress	               	       :1;
	        uint8_t transmit_in_progress                   	   :1;
	        uint8_t not_used3                                  :1;
	    };// __attribute__ ((packed));

	    uint8_t flgs;
	} state_machine_flg;

	typedef struct
	{
	    uint32_t timeout_transmit_cnt;
	    uint32_t timeout_receive_cnt;
	    uint32_t timeout_transmit;
	    uint32_t timeout_received;
	   __IO state_machine_flg flg;
	} _state_machine;

	_state_machine state_machine;
};


#endif /* STATEMACHINE_H_ */
