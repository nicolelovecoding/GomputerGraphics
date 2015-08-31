Task 3
C<i,0> = (P^(i-2) + 11P^(i-1) + 11P^i + P^(i+1))/24;
C<i,1> = (8P^(i-1) + 14P^i + 2P^(i+1))/24;
C<i,3> = (2P^(i-1) + 14P^i + 8P^(i+1))/24;
C<i,4> = (P^(i-1) + 11P^i + 11P^(i+1) + P^(i+2))/24;

Task 5
My conclusion is that the Bezier curve is on a circle. 
B(t) = C(i,0)(1-t)^4 + 4tC(i,1)(1-t)^3 + 6t^2C(i,2)(1-t)^2 + 4C(i,3)t^3(1-t) + C(i,4)t^4

As we know, t and 1-t range from 0 to 1. Byquadrate of t or (1-t) must be around 1/10000.
Sopposing B(t) = X(t) + Y(t), then in order to satisfy B(t) on a circle, the X(t) and Y(t)
must be X(t)^2 + Y(t)^2 = 1;

While C(i,0),C(i,1),C(i,2),C(i,3),C(i,4) comes from P(i-2),P(i-1),P(i),P(i+1),P(i+2) whose X 
and Y must be fell into (0,1).  

As a result, there equation X(t)^2 + Y(t)^2 = 1 could not be meet. The Bezier curve is not on 
a circle. 
