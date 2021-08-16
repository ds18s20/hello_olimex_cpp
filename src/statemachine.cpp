/*
 * statemachine.cpp
 *
 *  Created on: Jul 3, 2021
 *      Author: dima
 */
#include "statemachine.h"
#include "gpio.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <iomanip>
using namespace std;
#define TIMEOUT_CNT					1800



StateMachine::StateMachine()
{
	cout << "State Machine" << endl;
	Gpio gpio;
	gpio.gpio_map();

	blue_led.init(0, 2,&gpio);
	green_led[0].init(0, 4, &gpio);
	green_led[1].init(0, 5, &gpio);
	green_led[2].init(0, 6, &gpio);
	green_led[3].init(0, 7, &gpio);

	timeout_cnt = 0;

	logFile = new ofstream("logFile.txt") ;
    if (!logFile->is_open())
    	cout << "Unable to open file";
}

StateMachine::~StateMachine()
{
	logFile->close();
	delete logFile;
}

void StateMachine::timerEvent()
{

    // --- Code --------------------------------------------------------------- //
        _tsc_prm.flg._1_ms = 1;
        _tsc_prm.cnt_1_ms++;
    // Interval 10 ms
        if(++_tsc_prm.time_cadr_1_ms >= 10)
        {

            _tsc_prm.flg._10_ms = 1;
            _tsc_prm.time_cadr_1_ms -= 10;
            _tsc_prm.time_cadr_10_ms++;
        //  _tsc_prm.cnt_10_ms++;
        }

        if(_tsc_prm.time_cadr_10_ms >= 10)
        {

            _tsc_prm.flg._100_ms = 1;
            _tsc_prm.time_cadr_10_ms -= 10;
            _tsc_prm.time_cadr_100_ms++;
            _tsc_prm.cnt_100_ms++;

        }
    // Interval 1 s
        if(_tsc_prm.time_cadr_100_ms >= 10)
        {

            _tsc_prm.flg._1_s = 1;
            _tsc_prm.time_cadr_100_ms -= 10;
        //  _tsc_prm.cnt_1_s++;
        }
}

void StateMachine::write_log()
{
	Capstate *capstate;
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    time_str = std::to_string(now->tm_year + 1900)+"-"+std::to_string(now->tm_mon + 1)+"-"+std::to_string(now->tm_mday)+" "+std::to_string(now->tm_hour)+":"+std::to_string(now->tm_min);

   // cout<<"//-----------------------------------------------------------------"<<endl;
  //  *logFile<<"//-----------------------------------------------------------------"<<endl;
    //printf("//-----------------------------------------------------------------\n");
    cout<<time_str<<endl;
    *logFile<<time_str<<endl;

    capstate = capInfo.getCapInfo();

	printf("LTC3350_NUM_CAPS: %X\n",capstate->num_caps);
	printf("LTC3350_MEAS_VIN  : %04X  VIN  = %.2f V\n",capstate->vin_reg, capstate->vin);
	printf("LTC3350_MEAS_VOUT : %04X VOUT  = %.2f V\n" ,capstate->vout_reg,capstate->vout);
	printf("LTC3350_MEAS_IIN  : %04X IIN   = %.2f A\n",capstate->iin_reg,capstate->iin);
	printf("LTC3350_MEAS_ICHG : %04X ICHRG = %.2f A\n",capstate->ichrg_reg, capstate->ichrg);

	printf("LTC3350_MEAS_VCAP1: %04X VCAP1 = %.2f V\n",capstate->vcap1_reg, capstate->vcap1);
	printf("LTC3350_MEAS_VCAP1: %04X VCAP2 = %.2f V\n",capstate->vcap2_reg, capstate->vcap2);
	printf("LTC3350_MEAS_VCAP1: %04X VCAP3 = %.2f V\n",capstate->vcap3_reg, capstate->vcap3);
	printf("LTC3350_MEAS_VCAP1: %04X VCAP4 = %.2f V\n",capstate->vcap4_reg, capstate->vcap4);
	printf("LTC3350_MEAS_VCAP: %04X  VCAP  = %.2f V\n",capstate->vcap_reg, capstate->vcap);
	printf("LTC3350_MEAS_CAP: %X\n",capstate->cap_meas);

	printf("LTC3350_CHRG_STATUS: %04X\n",capstate->chrg_status.reg);
	printf("The synchronous controller is in step-down mode <charging> : %d \n",capstate->chrg_status.chrg_stepdown);
	printf("The synchronous controller is in step-up mode <backup>     : %d \n",capstate->chrg_status.chrg_stepup);
	printf("The charger is in constant voltage mode                    : %d \n",capstate->chrg_status.chrg_cv);
	printf("The charger is in undervoltage lockout                     : %d \n", capstate->chrg_status.chrg_uvlo);
	printf("The charger is in input current limit                      : %d \n",capstate->chrg_status.chrg_input_ilim);
	printf("The capacitor voltage is above power good threshold        : %d \n",capstate->chrg_status.chrg_cappg);
	printf("The capacitor manager is shunting                          : %d \n",capstate->chrg_status.chrg_shnt);
	printf("The capacitor manager is balancing                         : %d \n",capstate->chrg_status.chrg_bal);
	printf("The charger is temporarily disabled for cap measurement    : %d \n",capstate->chrg_status.chrg_dis);
	printf("The charger is in constant current mode                    : %d \n",capstate->chrg_status.chrg_ci);
	printf("Input voltage is below PFI threshold                       : %d \n",capstate->chrg_status.chrg_pfo);




    *logFile<<"LTC3350_NUM_CAPS:"<<std::dec<<capstate->num_caps<<endl;
    *logFile<<"LTC3350_MEAS_VIN: "<<std::hex<<capstate->vin_reg<<" VIN= "<<std::fixed<<std::setprecision(2)<<capstate->vin<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_VOUT : "<<std::hex<<capstate->vout_reg<<" VOUT= "<<std::fixed<<std::setprecision(2)<<capstate->vout<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_IIN  : "<<std::hex<<capstate->iin_reg<<" IIN= "<<std::fixed<<std::setprecision(2)<<capstate->iin<<" A"<<endl;
    *logFile<<"LTC3350_MEAS_ICHG : "<<std::hex<<capstate->ichrg_reg<<" ICHRG= "<<std::fixed<<std::setprecision(2)<<capstate->ichrg<<" A"<<endl;
    *logFile<<"LTC3350_MEAS_VCAP1: "<<std::hex<<capstate->vcap1_reg<<" VCAP1 = "<<std::fixed<<std::setprecision(2)<<capstate->vcap1<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_VCAP2: "<<std::hex<<capstate->vcap2_reg<<" VCAP2 = "<<std::fixed<<std::setprecision(2)<<capstate->vcap2<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_VCAP3: "<<std::hex<<capstate->vcap3_reg<<" VCAP3 = "<<std::fixed<<std::setprecision(2)<<capstate->vcap3<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_VCAP4: "<<std::hex<<capstate->vcap4_reg<<" VCAP4 = "<<std::fixed<<std::setprecision(2)<<capstate->vcap4<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_VCAP: "<<std::hex<<capstate->vcap_reg<<" VCAP = "<<std::fixed<<std::setprecision(2)<<capstate->vcap<<" V"<<endl;
    *logFile<<"LTC3350_MEAS_CAP: "<<std::hex<<capstate->cap_meas<<endl;
    *logFile<<"LTC3350_CHRG_STATUS: "<<std::hex<<capstate->chrg_status.reg<<endl;
    *logFile<<"The synchronous controller is in step-down mode <charging> : "<<capstate->chrg_status.chrg_stepdown<<endl;
    *logFile<<"The synchronous controller is in step-up mode <backup>     : "<<capstate->chrg_status.chrg_stepup<<endl;
    *logFile<<"The charger is in constant voltage mode                    : "<<capstate->chrg_status.chrg_cv<<endl;
    *logFile<<"The charger is in undervoltage lockout                     : "<<capstate->chrg_status.chrg_uvlo<<endl;
    *logFile<<"The charger is in input current limit                      : "<<capstate->chrg_status.chrg_input_ilim<<endl;
    *logFile<<"The capacitor voltage is above power good threshold        : "<<capstate->chrg_status.chrg_cappg<<endl;
    *logFile<<"The capacitor manager is shunting                          : "<<capstate->chrg_status.chrg_shnt<<endl;
    *logFile<<"The capacitor manager is balancing                         : "<<capstate->chrg_status.chrg_bal<<endl;
    *logFile<<"The charger is temporarily disabled for cap measurement    : "<<capstate->chrg_status.chrg_dis<<endl;
    *logFile<<"The charger is in constant current mode                    : "<<capstate->chrg_status.chrg_ci<<endl;
    *logFile<<"Input voltage is below PFI threshold                       : "<<capstate->chrg_status.chrg_pfo<<endl;
    *logFile<<"//-----------------------------------------------------------------"<<endl;
    cout<<"//-----------------------------------------------------------------"<<endl;

}

void StateMachine::run()
{
	uint8_t data[4];
	while(1)
	{
        if(_tsc_prm.flg._1_ms)
            _tsc_prm.flg._1_ms = 0;
        if(_tsc_prm.flg._10_ms)
            _tsc_prm.flg._10_ms = 0;
        if(_tsc_prm.flg._100_ms)
        {
            _tsc_prm.flg._100_ms = 0;
         //   for (int i=0; i<4; i++)
           // 	green_led[i].toggle();
        }
        if(_tsc_prm.flg._1_s)
        {
            _tsc_prm.flg._1_s = 0;

            blue_led.toggle();
            if (++timeout_cnt>=TIMEOUT_CNT)
            {
            	green_led[0].toggle();
            	timeout_cnt = 0;
            	this->write_log();
            }

          //  cout<<"timeout : "<<endl;
        }

	}


}
