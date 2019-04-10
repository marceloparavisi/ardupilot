#include "mode.h"
#include "Rover.h"

bool ModeGuided::_enter()
{
    // initialise waypoint speed
    set_desired_speed_to_default();

    // when entering guided mode we set the target as the current location.
    set_desired_location(rover.current_loc);

    // guided mode never travels in reverse
    rover.set_reverse(false);

    return true;
}

void ModeGuided::update()
{
    switch (_guided_mode) {
        case Guided_WP:
        {
            _distance_to_destination = get_distance(rover.current_loc, _destination);
            const bool near_wp = _distance_to_destination <= rover.g.waypoint_radius;
            // check if we've reached the destination
            if (!_reached_destination && (near_wp || location_passed_point(rover.current_loc, _origin, _destination))) {
                _reached_destination = true;
                rover.gcs().send_mission_item_reached_message(0);
            }
            // determine if we should keep navigating
            if (!_reached_destination || (rover.is_boat() && !near_wp)) {
                // drive towards destination
                calc_steering_to_waypoint(_reached_destination ? rover.current_loc : _origin, _destination);
                calc_throttle(calc_reduced_speed_for_turn_or_distance(_desired_speed), true, true);
            } else {
                stop_vehicle();
            }
            break;
        }

        case Guided_HeadingAndSpeed:
        {
            // stop vehicle if target not updated within 3 seconds
            if (have_attitude_target && (millis() - _des_att_time_ms) > 3000) {
                gcs().send_text(MAV_SEVERITY_WARNING, "target not received last 3secs, stopping");
                have_attitude_target = false;
            }
            if (have_attitude_target) {
                // run steering and throttle controllers
                calc_steering_to_heading(_desired_yaw_cd, _desired_speed < 0);
                calc_throttle(_desired_speed, true, true);
            } else {
                stop_vehicle();
                g2.motors.set_steering(0.0f);
            }
            break;
        }

        case Guided_TurnRateAndSpeed:
        {
            // stop vehicle if target not updated within 3 seconds
            if (have_attitude_target && (millis() - _des_att_time_ms) > 3000) {
                gcs().send_text(MAV_SEVERITY_WARNING, "target not received last 3secs, stopping");
                have_attitude_target = false;
            }
            if (have_attitude_target) {
                // run steering and throttle controllers
                float steering_out = attitude_control.get_steering_out_rate(radians(_desired_yaw_rate_cds / 100.0f),
                                                                            g2.motors.have_skid_steering(),
                                                                            g2.motors.have_vectored_thrust(),
                                                                            g2.motors.limit.steer_left,
                                                                            g2.motors.limit.steer_right,
                                                                            _desired_speed < 0);
                g2.motors.set_steering(steering_out * 4500.0f);
                calc_throttle(_desired_speed, true, true);
            } else {
                stop_vehicle();
                g2.motors.set_steering(0.0f);
            }
            break;
        }
        case Guided_PWMvalues:
		{

			g2.motors.set_raw_pwm(1,_pwm_ch1);
			g2.motors.set_raw_pwm(2,_pwm_ch2);
			g2.motors.set_raw_pwm(3,_pwm_ch3);
			g2.motors.set_raw_pwm(4,_pwm_ch4);
			_reached_destination = false;

			/*uint16_t output_pwm1;
			output_pwm1 = SRV_Channels::output_ch(1);
			uint16_t output_pwm3;
			output_pwm3 = SRV_Channels::output_ch(3);
			gcs().send_text(MAV_SEVERITY_CRITICAL,"channel 1 and 3 pwm %d %d", output_pwm1, output_pwm3);
*/
			/*for (uint8_t i =0; i< NUM_SERVO_CHANNELS; i++)
>>>>>>> bcd0f6095473ec6eb783522d25bafba9411b3942
			{
				uint16_t output_pwm;
				output_pwm = SRV_Channels::output_ch(i);
				 gcs().send_text(MAV_SEVERITY_CRITICAL,"channel %d pwm %d", i, output_pwm);
<<<<<<< HEAD
			}
			g2.motors.set_steering(_pwm_ch1);
			g2.motors.set_throttle(_pwm_ch3);
			_reached_destination = false;
=======
			}//*/
			break;
		}

        default:
            gcs().send_text(MAV_SEVERITY_WARNING, "Unknown GUIDED mode");
            break;
    }
}

// return distance (in meters) to destination
float ModeGuided::get_distance_to_destination() const
{
    if (_guided_mode != Guided_WP || _reached_destination) {
        return 0.0f;
    }
    return _distance_to_destination;
}

// set desired location
void ModeGuided::set_desired_location(const struct Location& destination)
{
    // call parent
    Mode::set_desired_location(destination);

    // handle guided specific initialisation and logging
    _guided_mode = ModeGuided::Guided_WP;
    rover.Log_Write_GuidedTarget(_guided_mode, Vector3f(_destination.lat, _destination.lng, 0), Vector3f(_desired_speed, 0.0f, 0.0f));
}

// set desired attitude
void ModeGuided::set_desired_heading_and_speed(float yaw_angle_cd, float target_speed)
{
    // call parent
    Mode::set_desired_heading_and_speed(yaw_angle_cd, target_speed);

    // handle guided specific initialisation and logging
    _guided_mode = ModeGuided::Guided_HeadingAndSpeed;
    _des_att_time_ms = AP_HAL::millis();
    _reached_destination = false;

    // record targets
    _desired_yaw_cd = yaw_angle_cd;
    _desired_speed = target_speed;
    have_attitude_target = true;

    // log new target
    rover.Log_Write_GuidedTarget(_guided_mode, Vector3f(_desired_yaw_cd, 0.0f, 0.0f), Vector3f(_desired_speed, 0.0f, 0.0f));
}

void ModeGuided::set_desired_heading_delta_and_speed(float yaw_delta_cd, float target_speed)
{
    // handle initialisation
    if (_guided_mode != ModeGuided::Guided_HeadingAndSpeed) {
        _guided_mode = ModeGuided::Guided_HeadingAndSpeed;
        _desired_yaw_cd = ahrs.yaw_sensor;
    }
    set_desired_heading_and_speed(wrap_180_cd(_desired_yaw_cd + yaw_delta_cd), target_speed);
}

// set desired velocity
void ModeGuided::set_desired_turn_rate_and_speed(float turn_rate_cds, float target_speed)
{
    // handle initialisation
    _guided_mode = ModeGuided::Guided_TurnRateAndSpeed;
    _des_att_time_ms = AP_HAL::millis();
    _reached_destination = false;

    // record targets
    _desired_yaw_rate_cds = turn_rate_cds;
    _desired_speed = target_speed;
    have_attitude_target = true;

    // log new target
    rover.Log_Write_GuidedTarget(_guided_mode, Vector3f(_desired_yaw_rate_cds, 0.0f, 0.0f), Vector3f(_desired_speed, 0.0f, 0.0f));
}

void ModeGuided::set_desired_pwm_value(int channel, float pwm)
{
<<<<<<< HEAD
	gcs().send_text(MAV_SEVERITY_WARNING, "setting to Guided_PWMvalues %d %f", channel, pwm);

	_guided_mode = ModeGuided::Guided_PWMvalues;
	if (channel == 1)
		_pwm_ch1=pwm;
	if (channel == 3)
		_pwm_ch3=pwm;
=======
	_guided_mode = ModeGuided::Guided_PWMvalues;
	if (channel == 1)
		_pwm_ch1=pwm;
	if (channel == 2)
		_pwm_ch2=pwm;
	if (channel == 3)
		_pwm_ch3=pwm;
	if (channel == 4)
		_pwm_ch4=pwm;

>>>>>>> bcd0f6095473ec6eb783522d25bafba9411b3942
}

