#include "mode.h"
#include "Rover.h"

void ModeManual::update()
{
	float desired_steering, desired_throttle;

    get_pilot_desired_steering_and_throttle(desired_steering, desired_throttle);

    // copy RC scaled inputs to outputs
    g2.motors.set_throttle(desired_throttle);
    g2.motors.set_steering(desired_steering);

    // mark us as in_reverse when using a negative throttle to stop AHRS getting off
    rover.set_reverse(is_negative(g2.motors.get_throttle()));
/*
    desired_steering=_desired_yaw_cd;
    desired_throttle=_desired_speed*50.0f;

    g2.motors.set_steering(desired_steering);
    g2.motors.set_throttle(desired_throttle);
//*/
/*
    gcs().send_text(MAV_SEVERITY_CRITICAL,"mode manual: steering: %f",desired_steering);
    gcs().send_text(MAV_SEVERITY_CRITICAL,"mode manual: throttle: %f",desired_throttle);
    for (int i =0; i< 3; i+=2)
	{
		uint16_t output_pwm;
		output_pwm = SRV_Channels::output_ch(i);
		 gcs().send_text(MAV_SEVERITY_CRITICAL,"channel %d pwm %d", i, output_pwm);
	}*/
}
