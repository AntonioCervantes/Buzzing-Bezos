function [Throttle1,Throttle2,Throttle3,Throttle4 ] = BB_yawRoC(percent)
%Here we are trying to find the individual thrust values for each motor
%using concepts of torque and sum of forces, as well as angular momentum.
%These will be using arbitrary values until drone dimensions and motor data
%is provided.

%   This function should be used to indirectly change the throttle values
%   of each motor by outputting how much the individual throttles should
%   change by


Mass = 15; %in kg
Gravity = 9.807; %m/s^2

%A positive yaw Rate of Change (RoC) value will have the drone rotate
%counterclockwise, while a negative YawRoC value will have the drone rotate
%clockwise

while true
    YawRoC = percent;
    if (-100 <= YawRoC)&&(YawRoC <= 100)
        disp("The drone will change its yaw at: " + YawRoC + " percent.")
        break
    elseif YawRoC < -100
        disp("You have inputted a value exceeding -100%, please try again")
        YawRoC = 0;
        break
    elseif YawRoC > 100
        disp("You have inputted a value exceeding 100%, please try again.")
        YawRoC = 0;
        break
    else
        disp("You have inputted an invalid value, please enter a number.")
    end
end

TotalThrust = Mass*Gravity;
NeutralThrust = TotalThrust/4;
NeutralThrottle = 0.0837*(NeutralThrust)^0.629;
MaxThrustChange = 0.2; %Newtons

%The Matrix of Thrust values follows with the positions of each motor

Motor1 = NeutralThrust + MaxThrustChange*(YawRoC/100);
Motor2 = NeutralThrust - MaxThrustChange*(YawRoC/100);
Motor3 = Motor2;
Motor4 = Motor1;

%Converting thrust (N) to throttle uses the following relation:
%(Throttle %) = 0.0837*(Thrust (N))^0.629
%   Outputs the CHANGE in throttle

Throttle1 = 0.0837*(Motor1)^0.629 - NeutralThrottle;
Throttle2 = 0.0837*(Motor2)^0.629 - NeutralThrottle;
Throttle3 = 0.0837*(Motor3)^0.629 - NeutralThrottle;
Throttle4 = 0.0837*(Motor4)^0.629 - NeutralThrottle;
end

