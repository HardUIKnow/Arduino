RC Car Control state machine exemple

Be aware....  Not fully completed

Instruction have to be sent by serial using command :

	dev | pwr | state1 | state2 | pwrstate1 | pwrstate2

	dev : 
			1	motor select
			2	picture select (not implemented)
	pwr:
			0	power on
			1	power off
	state 1:
			0	forward
			1	backward
	state 2:
			0	forward
			1	backward
	pwrstate1:
			0-255	must be sent through the equivalent char (the ihm as implement as chr(0) to send 0)
	pwrstate2:
			0-255	must be sent through the equivalent char (the ihm as implement as chr(0) to send 0)

For exemple sending : 1000uu 

			1 	motor selected
			0 	power on
			0 	motor A forward
			0 	motor B forward
			u 	motor A speed 117 over 255
			u 	motor B speed 117 over 255
	