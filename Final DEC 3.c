#pragma config(Sensor, in1,    gyro_sensor,    sensorGyro)
#pragma config(Sensor, in2,    Pexpander,      sensorAnalog)
#pragma config(Sensor, dgtl1,  left_sensor,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  right_sensor,   sensorQuadEncoder)
#pragma config(Sensor, dgtl11, arms_sensor,    sensorQuadEncoder)
#pragma config(Motor,  port1,           L_one,         tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           L_two,         tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           L_Y,           tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           DL1,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           DL2,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           DR1,           tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           DR2,           tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           R_Y,           tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           R_two,         tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          R_one,         tmotorVex393TurboSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
bool done = false; // remove
int hold_angle = 40;
bool hold;
bool arms_returned = true;
int lift;
int counter = 0;
int Lift_I = 0;
int Lift_hv = 20;
int height = 58;

int Lift_P;
bool uf_works = false;
bool works2 = false;
int step = 0;
int auton = 0;
string mainBattery, batteryTwo;
int left_drive;
int right_drive;
int x_axis;
int y_axis;
float a_value = 0.000062;
int mode = 1;
task LCD()
{
	while(true)
	{

		if(nLCDButtons > 0)
		{
			if(mode == 1)
			{
				mode = 0;
				clearLCDLine(0);
				clearLCDLine(1);
				wait10Msec(50);
			}
			else if(mode == 0)
			{
				mode = 1;
				clearLCDLine(0);
				clearLCDLine(1);
				wait10Msec(50);

			}
		}
		if(mode == 1)
		{

			displayLCDString(0, 0, "P:");
			sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
			displayNextLCDString(mainBattery);
			displayLCDString(1, 0, "E:");
			sprintf(batteryTwo, "%1.2f%c", SensorValue[Pexpander]/280.0, 'V');    //Build the value to be displayed
			displayNextLCDString(batteryTwo);
		}
		if(mode == 0)
		{

			displayLCDPos(0,0);                 // Set the cursor to line 0, position 0 (top line, far left)
			displayNextLCDString("99000B");
			displayLCDPos(1,0);                   // Set the cursor to line 1, position 0 (bottom line, far left)
			displayNextLCDString("BATTLECRUISER");
		}
	}
}
void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	bDisplayCompetitionStatusOnLcd = false;
	clearLCDLine(0);                      // Clear line 1 (0) of the LCD
	clearLCDLine(1);                      // Clear line 2 (1) of the LCD
	bLCDBacklight = true;
	startTask(LCD);



	SensorValue[in1] = 0;
}

void gyro_turn(int angle)
{
	int motorpower = 1;
	bool turned = false;

	int tol_gyro = 20;
	SensorValue[in1] = 0;
	while(turned == false)
	{
		if(motorpower == 0)
		{

			turned = true;

		}
		else
		{
			motorpower = (SensorValue[in1] - angle)*0.55;
			if(motorpower < 25 &&  motorpower > -25 && motorpower != 0)
			{
				motorpower = 0;
			}
			motor[DL1] = -motorpower;
			motor[DL2] = -motorpower;
			motor[DR1] = motorpower;
			motor[DR2] = motorpower;
		}
		//else if(SensorValue[in1] > (angle + tol_in1))
		//{
		//	if(SensorValue[in1] < (angle + 200))
		//	{
		//		motor[DL1] = -24;
		//		motor[DL2] = -24;
		//		motor[DR1] = 24;
		//		motor[DR2] = 24;

		//	}
		//	else if(SensorValue[in1] > (angle + 200))
		//	{
		//		motor[DL1] = -speed;
		//		motor[DL2] = -speed;
		//		motor[DR1] = speed;
		//		motor[DR2] = speed;
		//	}
		//}
		////clockwise
		//else if(SensorValue[in1] < (angle - tol_in1))
		//{
		//	if(SensorValue[in1] > (angle - 200))
		//	{
		//		motor[DL1] = 24;
		//		motor[DL2] = 24;
		//		motor[DR1] = -24;
		//		motor[DR2] = -24;

		//	}
		//	else if(SensorValue[in1] < (angle - 200))
		//	{
		//		motor[DL1] = speed;
		//		motor[DL2] = speed;
		//		motor[DR1] = -speed;
		//		motor[DR2] = -speed;
		//	}
		//}

	}
}

void drive(int dist)
{
	bool drive_a_ = true;
	SensorValue[left_sensor] = 0;
	SensorValue[right_sensor] = 0;
	int tol_da = 25;
	//while(drive_a_ == true)
	//{

	//	bool kill_loop = false;
	//	bool kill_loop_r = false;

	//	if(dist + tol_da > SensorValue[left_sensor] && dist - tol_da < SensorValue[left_sensor] && kill_loop == false)
	//	{
	//		wait10Msec(5);
	//		kill_loop = true;
	//		motor[DL1] = 0 ;
	//		motor[DL2] = 0;
	//	}
	//	else if((SensorValue[left_sensor]) < dist)
	//	{
	//		motor[DL1] = -speed;
	//		motor[DL2] = -speed;
	//	}
	//	else if((SensorValue[left_sensor]) > dist)
	//	{
	//		motor[DL1] = speed;
	//		motor[DL2] = speed;
	//	}
	//	if(dist + tol_da > SensorValue[right_sensor] && dist - tol_da < SensorValue[right_sensor] && kill_loop_r == false)
	//	{
	//		wait10Msec(5);
	//		kill_loop_r = true;
	//		motor[DR1] = 0;
	//		motor[DR2] = 0;
	//	}
	//	else if((SensorValue[right_sensor]) < dist)
	//	{
	//		motor[DR1] = -speed;
	//		motor[DR2] = -speed;
	//	}
	//	else if((SensorValue[right_sensor]) > dist)
	//	{
	//		motor[DR1] = speed;
	//		motor[DR2] = speed;
	//	}
	//	if(kill_loop == true && kill_loop_r == true)
	//	{
	//		drive_a_ = false;
	//	}
	//}
	int counter_d = 0;

	while(drive_a_ == true)
	{



		if(dist == -400)
		{
			SensorValue[dgtl10] = 1;
		}
		else
		{
			SensorValue[dgtl10] = 0;
		}
		if(counter_d > 150)
		{
			motor[DL1] = 0;
			motor[DL2] = 0;
			motor[DR1] = 0;
			motor[DR2] = 0;
			drive_a_ = false;

		}
		if(dist + tol_da > SensorValue[left_sensor] && dist - tol_da < SensorValue[left_sensor])
		{
			counter_d ++;
		}

		else
		{
			int motorpower;
			motorpower = (SensorValue[left_sensor] - dist)* 0.55;
			if(motorpower < tol_da && motorpower > -tol_da && motorpower != 0)
			{
				motorpower = 0;
			}
			motor[DL1] = motorpower;
			motor[DL2] = motorpower;
			motor[DR1] = motorpower;
			motor[DR2] = motorpower;

		}

	}
}

void shoot(int arms, int wait)
{
	bool arms_up = false;
	bool arms_returned = false;
	while(arms_returned == false)
	{
		if(SensorValue[arms_sensor] > arms && arms_up == false)
		{

			motor[L_one] = 0;
			motor[L_two] = 0;
			motor[L_Y] = 0;
			motor[R_one] = 0;
			motor[R_two] = 0;
			motor[R_Y] = 0;
			arms_up = true;
		}

		else if(SensorValue[arms_sensor] < arms && arms_up == false)
		{
			motor[L_one] = 127;
			motor[L_two] = 127;
			motor[L_Y] = 127;
			motor[R_one] = 127;
			motor[R_two] = 127;
			motor[R_Y] = 127;
		}
		if(SensorValue[arms_sensor] > 10 &&  arms_up == true)
		{
			wait10Msec(wait);
			motor[L_one] = -60;
			motor[L_two] = -60;
			motor[L_Y] = -60;
			motor[R_one] = -60;
			motor[R_two] = -60;
			motor[R_Y] = -60;
		}
		else if(SensorValue[arms_sensor] < 15 && arms_up == true)
		{
			motor[L_one] = 0;
			motor[L_two] = 0;
			motor[L_Y] = 0;
			motor[R_one] = 0;
			motor[R_two] = 0;
			motor[R_Y] = 0;
			arms_returned = true;
		}
	}
}
void deploy()
{
	motor[DL1] = 20;
	motor[DL2] = -20;
	motor[DR1] = 20;
	motor[DR2] = -20;
	motor[L_one] = 80;
	motor[L_two] = 80;
	motor[L_Y] = 80;
	motor[R_one] = 80;
	motor[R_two] = 80;
	motor[R_Y] = 80;
	wait10Msec(50 );
	motor[L_one] = 0;
	motor[L_two] = 0;
	motor[L_Y] = 0;
	motor[R_one] = 0;
	motor[R_two] = 0;
	motor[R_Y] = 0;
	wait10Msec(20);
	motor[L_one] = -127;
	motor[L_two] = -127;
	motor[L_Y] = -127;
	motor[R_one] = -127;
	motor[R_two] = -127;
	motor[R_Y] = -127;
	wait10Msec(60);
	motor[R_Y] = 0;
	motor[DL1] = 0;
	motor[DL2] = 0;
	motor[DR1] = 0;
	motor[DR2] = 0;
}
//void hold_arms_temp() //temp
//{
//	bool loopback = false;
//	while(loopback == false)
//	if(SensorValue[arms_sensor] > 50 && SensorValue[arms_sensor] < 60)
//	{
//		wait10Msec(10);
//		motor[L_one] = 0;
//		motor[L_two] = 0;
//		motor[L_Y] = 0;
//		motor[R_one] = 0;
//		motor[R_two] = 0;
//		motor[R_Y] = 0;
//		loopback = true;
//	}
//	if(SensorValue[arms_sensor] < 55)
//	{
//		motor[L_one] = 60;
//		motor[L_two] = 60;
//		motor[L_Y] = 60;
//		motor[R_one] = 60;
//		motor[R_two] = 60;
//		motor[R_Y] = 60;
//	}
//	else if (SensorValue[arms_sensor] > 55)
//	{
//		motor[L_one] = -30;
//		motor[L_two] = -30;
//		motor[L_Y] = -30;
//		motor[R_one] = -30;
//		motor[R_two] = -30;
//		motor[R_Y] = -30;
//	}

//}
void arm_down()
{
	motor[L_one] = -20;
	motor[L_two] = -20;
	motor[L_Y] = -20;
	motor[R_one] = -20;
	motor[R_two] = -20;
	motor[R_Y] = -20;
	wait10Msec(50);
	motor[L_one] = 0;
	motor[L_two] = 0;
	motor[L_Y] = 0;
	motor[R_one] = 0;
	motor[R_two] = 0;
	motor[R_Y] = 0;
}
task arms_PI()
{

	while(true)
	{

		while(arms_returned == true && hold == true)
		{
			//Liftx = (((40 - SensorValue[arms_sensor])*100)/ 40;
			Lift_P = (1.75 * (height - SensorValue[arms_sensor]));

			if(hold == true)
			{
				lift = (20+(Lift_P));
				counter ++;

			}
			if(counter > 500)
			{
				Lift_I += 1;
				counter = 0;
			}
			motor[L_one] = lift;
			motor[L_two] = lift;
			motor[L_Y] = lift;
			motor[R_one] = lift;
			motor[R_two] = lift;
			motor[R_Y] = lift;
		}

	}
}
task driver()
{
	while(true)
	{
		if(vexRT[Btn8U] == 1)
		{
			SensorValue[arms_sensor] = 0;
		}

		else if(vexRT[Btn6U] == 1)
		{
			x_axis = vexRT[Ch1]/1.5;
			y_axis = vexRT[Ch2]/1.5;

		}
		else if(vexRT[Btn5U] == 1)
		{
			x_axis = ((a_value/1.5) * (vexRT[Ch1]* vexRT[Ch1]* vexRT[Ch1]));
			y_axis = (a_value * (vexRT[Ch2]* vexRT[Ch2]* vexRT[Ch2]));
			//claw = -127;
		}
		else if(vexRT[Btn5D] == 1)
		{
			x_axis = vexRT[Ch1]/4;
			y_axis = vexRT[Ch2]/4;

		}

		else
		{
			x_axis = ((a_value/1.5) * (vexRT[Ch1]* vexRT[Ch1]* vexRT[Ch1]));
			y_axis = (a_value * (vexRT[Ch2]* vexRT[Ch2]* vexRT[Ch2]));
		}
		left_drive = (y_axis - x_axis); //converts x and y axis to left motor power
		right_drive = (y_axis + x_axis);
		motor[DL1] = left_drive;
		motor[DL2] = left_drive;
		motor[DR1] = right_drive;
		motor[DR2] = right_drive;
	}
}
task arms()
{
	while(true)
	{
		if(vexRT[Btn6U] == 1 && vexRT[Btn5U] == 0)
		{
			hold = true;
			arms_returned = true;
		}
		else
		{
			hold = false;
			arms_returned = false;
			lift = vexRT[Ch3];
			motor[L_one] = lift;
			motor[L_two] = lift;
			motor[L_Y] = lift;
			motor[R_one] = lift;
			motor[R_two] = lift;
			motor[R_Y] = lift;
		}
	}
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	startTask(arms_PI);
	SensorValue[arms_sensor] = 0;
	if(auton == 0) // go forward and block
	{
		deploy();
		drive(-400);
		height = 140;
		hold = true;
		drive(-600);
	}
	if(auton == 1) //backwards shoot + block
	{
		drive(400);
		deploy();
		height = 50;
		hold = true;
		drive(600);
		shoot(150, 50);
		height = 140;
		hold = true;
		drive(0);
	}
	if(auton == 2) //shoot cube and block (right)
	{
		drive(-400);
		deploy();
		gyro_turn(-850);
		drive(-800);
		height = 50;
		hold = true;
		drive(-100);
		gyro_turn(-950);
		drive(800);
		shoot(150,50);
		height = 140;
		hold = true;
	}
	if(auton == 3) //shoot cube and block (left)
	{
		drive(-400);
		deploy();
		gyro_turn(-850);
		drive(-800);
		height = 50;
		hold = true;
		drive(-100);
		gyro_turn(-950);
		drive(800);
		shoot(150,50);
		height = 140;
		hold = true;
	}
	if(auton == 4)
	{
		drive(400);
		deploy();
		drive(400);
		shoot(160,20);
		gyro_turn(-900);
		drive(-700);
		gyro_turn(900);
		drive(80);
	}



	//drive(-1000,80);
	//wait10Msec(90);
	//in1_turn(-1300,80);
	//drive(600,80);
	//hold = false;
	//shoot(160,50);
	//deploy();
	//SensorValue[arms_sensor] = 0;
	//drive(-600, 80);
	//hold_arms_temp();
	//in1_turn(-1000, 80);
	//drive(600, 80);
	//shoot(180,40);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	height = 50;
	startTask(arms_PI);
	startTask(driver);
	startTask(arms);


}
