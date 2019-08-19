/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------/*
 * Rosserial controller engine & encoder driver
 * Topics:
 *      name                    type        description
 *      cmd_vel                 Vector3     x = PWM value for the left eng, y - for the right
 *      sensors/wheel_encoder   Vector3     x = dist_travelled by left track for last 200ms, y - for the right
 */

#include <mbed.h>
#include <ros.h>
#include <dc_engine.h>
#include <spd_sensor.h>
#include <configurations.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Bool.h>

/******************************************************************************/
                            //HARDWARE INITIALISATION
DC_engine eng_l(SPEEDPIN_A , MOTORSHIELD_IN1, MOTORSHIELD_IN2, true);
DC_engine eng_r(SPEEDPIN_B, MOTORSHIELD_IN3, MOTORSHIELD_IN4, true);

Spd_sensor sens_l(ENCODER_LEFT_IN1, ENCODER_LEFT_IN2, GEAR_RATIO);
Spd_sensor sens_r(ENCODER_RIGHT_IN1, ENCODER_RIGHT_IN2, GEAR_RATIO);

DigitalOut led(LED1);
DigitalOut light(LIGHTS);

/******************************************************************************/
ros::NodeHandle  nh;

void motor_messageCb(const geometry_msgs::Vector3& msg)
{
    eng_l.write(msg.x);
    eng_r.write(msg.y);
}

void lights_messageCb(const std_msgs::Bool& msg)
{
    light.write(msg.data);
}

ros::Subscriber<geometry_msgs::Vector3> motor_sub("cmd_vel", &motor_messageCb);
ros::Subscriber<std_msgs::Bool> lights_sub("control/lights", &lights_messageCb);

geometry_msgs::Vector3 sensor_msg;

ros::Publisher sensor_pub("sensors/wheel_encoder", &sensor_msg);

Ticker encode_ticker;

void send_enc_data()
{
    sensor_msg.x = sens_l.getValue();
    sensor_msg.y = sens_r.getValue();
    if (sensor_msg.x != 0 || sensor_msg.y != 0)
        sensor_pub.publish(&sensor_msg);
}

int main() {
    nh.initNode();
    nh.subscribe(motor_sub);
    nh.subscribe(lights_sub);
    nh.advertise(sensor_pub);
    encode_ticker.attach(&send_enc_data ,0.2);

    while (1) {
        nh.spinOnce();
    }
}

