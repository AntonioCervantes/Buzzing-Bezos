function [Throttle1,Throttle2,Throttle3,Throttle4] = BB_neutralHover(~)
%The purpose of this function is to have the throttle of each motor return
%to a "neutral hover" state, so that there's an easy way to "reset" the
%pitch, roll, yaw, and throttle of the drone.

%   This function should be used to directly change the throttle values of
%   the drone's motors

Mass = 15; %in kg
Gravity = 9.807; %m/s^2
TotalThrust = Mass*Gravity;
NeutralThrust = TotalThrust/4;
NeutralThrottle = 0.0837*(NeutralThrust)^0.629;

Throttle1 = NeutralThrottle;
Throttle2 = NeutralThrottle;
Throttle3 = NeutralThrottle;
Throttle4 = NeutralThrottle;
end

