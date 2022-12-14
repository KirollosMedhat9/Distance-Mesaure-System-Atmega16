/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: ultrasonic.c
 *
 * Description: Header file for the AVR ICU driver
 *
 * Author: Kirollos Medhat Massoud
 *
 *******************************************************************************/
#include "ultrasonic.h"
#include "gpio.h"
#include <util/delay.h>
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/*Global Variable for ISR of ICU driver counting the edges*/
uint8   g_edgeCount=0;
/*Global Variable of time of ECHO signal of ULTRASONIC Sensor*/
uint16  g_timeHigh=0;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description: Initialize the ICU driver as required.
 *              Setup the ICU call back function.
 *              Setup the direction for the trigger pin as output pin through the GPIO driver.
 * Arguments:   None
 * Return:      None
 */

void Ultrasonic_init(void)
{
	Icu_ConfigType clockConfig = {F_CPU_8,RISING}; /*Configurable ICU driver's clock speed and detecting edge initial*/

	Icu_init(&clockConfig);						/*ICU Driver initialization*/

	Icu_setCallBack(Ultrasonic_edgeProcessing); /*Setting the call back function by ICU Driver*/

	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN,PIN_OUTPUT); /*Set Trigger Pin as Output*/

	GPIO_setupPinDirection(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN,PIN_INPUT); /*set ECHO pin as input*/

	GPIO_writePin(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN, LOGIC_HIGH); /*Set internal PULL up resistor*/


}

/*
 * Description: Send the Trigger pulse to the ultrasonic sensor
 * Arguments:   None
 * Return:      None
 */

void Ultrasonic_Trigger(void)
{
	/*
	 * Sending a Pulse of LOGIC HIGH during pulse duration
	 */

	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_HIGH);

	_delay_us(15); 								/*Pulse duration of at least 10 microseconds*/

	GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);

}


/*
 * Description: Send the trigger pulse by using Ultrasonic_Trigger function.
 *              Start the measurements by the ICU from this moment
 * Arguments:   None
 * Return:      The measured distance in Centimeter.
 */


uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();                /*sending the PULSE*/

	uint16 a_distanceFromSensor;
	a_distanceFromSensor = (uint16)((g_timeHigh / (uint32)ULTRASONIC_DISTANCE_SCALING_FOR_8MHZ));  /*Calculating distance*/
	return a_distanceFromSensor;

	/*
	 * Sound velocity = 340.00 m/s = 34000 cm/s
	 * The distance of Object (in cm) = (340000???Time)/2= 17000 * Time
	 * Since Prescaler F_CPU/8 for timer frequency. Then time to execute 1 instruction is 1 us.
	 * So, the timer gets incremented after 1 us time elapse.
	 * Distance= 17000 x (TIMER value) x 1 x 10^-6 cm = 0.017 x (TIMER value) cm = (TIMER value) / 58.8 cm
	 */
}



/*
 * Description: This is the call back function called by the ICU driver.
 *              This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 * Arguments:   None
 * Return:      None
 */

void Ultrasonic_edgeProcessing(void)
{

	g_edgeCount++; 						/*incrementing global variable each ISR call*/

	if(g_edgeCount == 1)
	{

		Icu_clearTimerValue();				/*clearing the initial value of time found in ICR1 to start timer from 0*/
		Icu_setEdgeDetectionType(FALLING);	/*switching the ICU detection edge to falling
												to start the timer from the rising until the falling edge*/

	}

	else if(g_edgeCount == 2)
	{
		/*ISR Call back function increments the global g_edgecount variable
		 * Detecting new edge event which is rising as adjusted before
		 * Getting the elapsed time of ECHO signal
		 * Setting the detection to Rising again to detect the new ECHO signal and clearing the ISR increment
		 * to start increment from the beginning
		 */


		g_timeHigh = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(RISING);
		g_edgeCount =0;

	}

}
