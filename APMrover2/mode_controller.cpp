#include "mode.h"
#include "Rover.h"
float steer=0;
float thro=0;

void ModeController::update()
{

    SRV_Channels::set_output_pwm_chan(_ch_number, _ch_pwm);


    // send the pwm values to ground station
    for (uint8_t i =0; i< NUM_SERVO_CHANNELS; i++)
	{
		uint16_t output_pwm;
		output_pwm = SRV_Channels::output_ch(i);
		 gcs().send_text(MAV_SEVERITY_CRITICAL,"channel %d pwm %d", i, output_pwm);
	}



    /*
    steer=steer+10;
    thro=thro+1;
    if (steer >= 4500)
    	steer=-4500;
    if (thro >= 100)
    	thro=-100;
    desired_steering=steer;
    desired_throttle=thro;
    rover.set_reverse(false);

    // copy RC scaled inputs to outputs
    gcs().send_text(MAV_SEVERITY_CRITICAL,"MARCELO manual: update1");
    gcs().send_text(MAV_SEVERITY_CRITICAL,"MARCELO manual: left: %f",desired_steering);
    gcs().send_text(MAV_SEVERITY_CRITICAL,"MARCELO manual: right: %f",desired_throttle);


*/
	//g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_THROTTLE, 100);
	/*bool returned = g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_STEERING, steer);
	if (returned)
		gcs().send_text(MAV_SEVERITY_CRITICAL,"MARCELO manual returned true");
		*/
    //g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_THROTTLE_LEFT, 100);
    //g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_THROTTLE_RIGHT, 100);
    //g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_THROTTLE_LEFT, desired_steering);
    //g2.motors.output_test_pwm(AP_MotorsUGV::MOTOR_TEST_THROTTLE_RIGHT, desired_throttle);
    //g2.motors.output_test_pct(AP_MotorsUGV::MOTOR_TEST_THROTTLE, 100.0);
    //g2.motors.output_test_pct(AP_MotorsUGV::MOTOR_TEST_STEERING, desired_steering);
    //g2.motors.output_test_pct(AP_MotorsUGV::MOTOR_TEST_THROTTLE, 100.0);
    //g2.motors.set_throttle(desired_throttle);
    /*
    gcs().send_text(MAV_SEVERITY_CRITICAL,"MARCELO manual: frameClass: %d", g2.frame_class);

    g2.motors.set_steering(steer);
    g2.motors.set_throttle(thro);

    for (int i =0; i< 3; i++)
	{
		uint16_t output_pwm;
		output_pwm = SRV_Channels::output_ch(i);
		 gcs().send_text(MAV_SEVERITY_CRITICAL,"channel %d pwm %d", i, output_pwm);
	}
    //g2.motors.output_skid_steering(true, steer, thro);
    //calc_throttle(20, false, true);
    //g2.motors.set_throttle(thro);

    //SRV_Channels::set_output_scaled(SRV_Channel::k_throttleLeft, thro);
    //SRV_Channels::set_output_scaled(SRV_Channel::k_throttleRight, thro);
    //SRV_Channels::set_output_scaled(SRV_Channel::k_steering, steer);
    //SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, thro);


*/
/*
	// send values to the PWM timers for output
	SRV_Channels::calc_pwm();
	SRV_Channels::cork();
	//SRV_Channels::output_ch_all();
	for (int i =0; i< 3; i++)
	{
		uint16_t output_pwm;
		output_pwm = SRV_Channels::output_ch(i);
		 gcs().send_text(MAV_SEVERITY_CRITICAL,"channel %d pwm %d", i, output_pwm);
	}
	SRV_Channels::push();
*/
    gcs().send_text(MAV_SEVERITY_CRITICAL,"Control mode: update2");
}
