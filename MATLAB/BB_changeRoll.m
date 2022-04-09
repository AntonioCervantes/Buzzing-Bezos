function [Throttle1,Throttle2,Throttle3,Throttle4] = BB_changeRoll(angle)
%Here we are trying to find the individual thrust values for each motor
%using concepts of torque and sum of forces.
%These will be using arbitrary values until drone dimensions and motor data
%is provided.

%   This function should be used to indirectly change the throttle values
%   of each motor by outputting how much the individual throttles should
%   change by

Mass = 15; %in kg
% MaxAcc = 0.9; m/s^2
MaxRoll = 5; %degrees (change to radians)
MotorToRotDist = 0.5; %meters
MassToRotDist = 0.05; %meters

Gravity = 9.807; %m/s^2

TotalThrust = Mass*Gravity;
NeutralThrust = TotalThrust/4;
NeutralThrottle = 0.0837*(NeutralThrust)^0.629;

while true
    Roll = angle;
    if (Roll <= MaxRoll)&&(Roll >= -MaxRoll)
        disp("The drone will travel at a Roll of: " + Roll + " degrees.")
        break
    elseif Roll < -MaxRoll
        disp("You have inputted a value exceeding negative Max Roll, please try again.")
        Roll = 0;
        break
    elseif Roll > MaxRoll
        disp("You have inputted a value exceeding positive Max Roll, please try again.")
        Roll = 0;
        break
    else
        disp("You have inputted an invalid value, please enter a number.")
        Roll = 0;
        break
    end
end

TiltAngle = (90-Roll)*pi/180;
HorizAcc = Gravity/tan(TiltAngle);

%This if/then statement is just figuring out what motors to calculate
%specific values for.
%The Matrix of Thrust values follows with the positions of each motor

Motor2 = (Mass*HorizAcc/4)*(1/cos(TiltAngle) + MassToRotDist*sin(TiltAngle)/MotorToRotDist);
Motor4 = Motor2;
Motor1 = ((Mass*HorizAcc)/cos(TiltAngle) - (Motor2 + Motor4))/2;
Motor3 = Motor1;

%Converting thrust (N) to throttle uses the following relation:
%(Throttle %) = 0.0837*(Thrust (N))^0.629
%   Outputs the CHANGE in throttle

Throttle1 = 0.0837*(Motor1)^0.629 - NeutralThrottle;
Throttle2 = 0.0837*(Motor2)^0.629 - NeutralThrottle;
Throttle3 = 0.0837*(Motor3)^0.629 - NeutralThrottle;
Throttle4 = 0.0837*(Motor4)^0.629 - NeutralThrottle;
end

