/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: ultrasonic.h
 *
 * Description: Header file for the AVR ICU driver
 *
 * Author: Kirollos Medhat Massoud
 *
 *******************************************************************************/
#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include "icu.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ULTRASONIC_TRIGGER_PORT					PORTB_ID
#define ULTRASONIC_TRIGGER_PIN 					PIN5_ID
#define ULTRASONIC_ECHO_PORT					PORTD_ID
#define ULTRASONIC_ECHO_PIN 					PIN6_ID
#define ULTRASONIC_DISTANCE_SCALING_FOR_8MHZ    58.31


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
void Ultrasonic_init(void);


/*
 * Description: Send the Trigger pulse to the ultrasonic sensor
 * Arguments:   None
 * Return:      None
 */
void Ultrasonic_Trigger(void);



/*
 * Description: Send the trigger pulse by using Ultrasonic_Trigger function.
 *              Start the measurements by the ICU from this moment
 * Arguments:   None
 * Return:      The measured distance in Centimeter.
 */
uint16 Ultrasonic_readDistance(void);



/*
 * Description: This is the call back function called by the ICU driver.
 *              This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 * Arguments:   None
 * Return:      None
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */