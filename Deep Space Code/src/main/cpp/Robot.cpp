#include <frc/Joystick.h>
#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/WPILib.h>

class Robot : public frc::TimedRobot
{
  //Special Variables
  float adjustedFlipSpeed; //See note below
  float adjustedRotateSpeed;
  float forward; //Self Explanatory
  float turn; //Self Explanatory
  bool driving; //Driving Check
  bool turning; //Turning Check idk if I actually use this
  double idrive; //Raw Axis 1
  double iturn; //Raw Axis 4
  double iinit; //Raw Axis 3
  double iovertime; //Raw Axis 2
  int currentPOV;
  double miniMotor;
  //final RumbleType kLeftRumble;

  //double acceleration = driveController.GetRawAxis(1);

  //Camera Variables
  cs::UsbCamera camera1;
 //cs::UsbCamera camera2;
  //cs::VideoSink server;

  //Variables for Driving
  frc::PWMVictorSPX m_leftMotor{9}; //Location on the RoboRIO
  frc::PWMVictorSPX m_rightMotor{8}; //Location on the RoboRIO
  frc::DifferentialDrive m_robotDrive{m_leftMotor, m_rightMotor}; //Attaches the motors to the Drive class
  frc::Joystick driveController{1}; //The number is referring to the order of the Joysticks in the Driver Station. This is for the Xbox Controller so it should be 1 in both here and the DS
  public:
    void RobotInit()
    {
      camera1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      //camera2 = frc::CameraServer::GetInstance()->StartAutomaticCapture(1);
      //server = frc::CameraServer::GetInstance()->GetServer();
      //camera1.SetConnectionStrategy(cs::VideoSource::ConnectionStrategy::kConnectionKeepOpen);
      //camera2.SetConnectionStrategy(cs::VideoSource::ConnectionStrategy::kConnectionKeepOpen);
    }
    void AutonomousInit()
    {

    }

    void AutonomousPeriodic()
    {
      TeleopPeriodic();
    }

    void TeleopInit()
    {
      printf("%s\n", "brooke was here :)");
      printf("%s\n", "riley");
      printf("%s\n", "tom holland is father");
    }
  //Robot Control
    void TeleopPeriodic() override
    {
      //Camera Stuff YEET
      //if (m_stick.GetRawButton(3))
      //{
      //  printf("Camera 2\n");
      //  server.SetSource(camera2);
      //}
      //else if (m_stick.GetRawButton(4)) 
      //{
      //  printf("Camera 1\n");
      //  server.SetSource(camera1);
      //}

      //Flipping
      if (m_stick.GetRawButton(1) > .1)
      {
        adjustedFlipSpeed = (m_stick.GetY() * -0.65); //Explanation: adjustedFlipSpeed is a variable which is just the Y joystick value multiplied by 0.4 (a random number used to decrease the power). CIM motors are incredibly powerful, so decreasing the power of them through the code is an easy way to avoid parts being broken
        m_motor.Set(adjustedFlipSpeed); //Flipping Mechanism
      } else if (m_stick.GetRawButton(1) < -.1)
      {
        adjustedFlipSpeed = (m_stick.GetY() * -0.65); //Explanation: adjustedFlipSpeed is a variable which is just the Y joystick value multiplied by 0.4 (a random number used to decrease the power). CIM motors are incredibly powerful, so decreasing the power of them through the code is an easy way to avoid parts being broken
        m_motor.Set(adjustedFlipSpeed); //Flipping Mechanism
      }
      else
      {
          m_motor.Set(0);
      }
      

      idrive = driveController.GetRawAxis(1); //Setting forward/backwards to variable
      iturn = driveController.GetRawAxis(4); //Setting turning to variable
      iinit = driveController.GetRawAxis(3); //Pedal/Rev to variable
      iovertime = driveController.GetRawAxis(2); //Overtime Pedal to variable
      currentPOV = m_stick.GetPOV();
      //miniMotor = .075;
      
      //printf("turn=%1.00f\n", m_stick.GetRawAxis(3));

      switch (currentPOV)
      {
        case 0: miniMotor = -0.4;
        break;

        case 180: miniMotor = 0.4;
        break;

        default: miniMotor = 0;
          break;
      }

     

      if (iinit > .1) //Pedal/Rev
      {
        driving = true;
      }
      else
      {
        driving = false; //If you're NOT driving, make sure you aren't actually driving by seting the variables to false
        forward = 0;
      }

      if (iturn > .1 || iturn < -.1) //Turning system
      {
        turn = 1*iturn/1.65;
      }
      else if (driveController.GetRawButton(5)) //Slow Turn
      {
        turn = -.7;
      }
      else if (driveController.GetRawButton(6)) //Slow Turn
      {
        turn = .74; //brooke was here :)
      }
      else //Set Turn to Zero
      {
        turn = 0;
      }

      //For anyone in the future loking at this: I separated the forwards drive and the backwards drive to account for the lean.
      //It Works... So I'm not gonna change it
      if (driving == true) //Makes sure you're holding onto the button before allowing to drive
      {
        if (idrive > .1)
        {
          if (iovertime < .1 ) {
            forward = (idrive)*(iinit)/1.5; //Normal Drive
          }
          
          
          if (iovertime > .1)
          {
            forward = (idrive)*(iinit)*(iovertime); //Overtime Drive
          }
        }
        else if (idrive < -.1)
        {
          if (iovertime < .1)
          {
            forward = (idrive)*(iinit)/1.5; //Normal Drive
            turn += .28*(iinit); //Turn with addition modifier to fix the lean
          }
          else if (iovertime > .9)
          {
            forward = (idrive)*(iovertime)*(iinit); //Overtime Drive
            turn += .2*(iovertime); //Turn with addition modifier to fix the lea 
          }
        }
        else 
        {
          forward = 0; //Set Forward to 0 if not driving
        }
      }

       if (driveController.GetRawButton(9) && driveController.GetRawButton(10))
       {
        forward = 0;
        turn = 0;
      }

      if (forward > .1 || turn > .1 || forward < -.1 || turn < -.1) {
        driveController.SetRumble(, .3);
      }

      //printf("turn=%5.2f\n", turn); //TESTING ONLY TESTING ONLY 
      //printf("forward=%5.2f\n", forward);
      m_robotDrive.ArcadeDrive(forward,turn);
      m_motor2.Set(miniMotor);
    }
    void TestPeriodic()
    {
      
    }

    void DisabledInit()
    {
      printf("%s\n", "Okay, that was epic.");
    }


 private:
  frc::Joystick m_stick{0}; //Joystick number in the DS.
  frc::PWMVictorSPX m_motor{6}; //Location on the RoboRIO
  frc::PWMVictorSPX m_motor2{7}; //Location on the RoboRIO
};
//This stuff is important, I guess VVVVVV
#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif