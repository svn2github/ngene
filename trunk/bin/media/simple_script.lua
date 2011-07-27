-- This file is used for tutorial 1.7 demonstrating usage
-- of nGENE Tech scripting capabilities.

hello()

now = getEngine():getTimer():getMiliseconds()
camera = getEngine():getActiveCamera()

angle = 0.0
axisX = Vector3(1.0, 0.0, 0.0)
axisY = Vector3(0.0, 1.0, 0.0)

quatX = Quaternion(axisX, 0.3)


while(1) do
	diff = getEngine():getTimer():getMiliseconds() - now
	angle = angle + diff * 0.001 * math.pi
	quatY = Quaternion(axisY, angle)
	quatY = quatY * quatX
	
	camera:setRotation(quatY)
	now = getEngine():getTimer():getMiliseconds()	

	nGENE.WaitFrame(1)
end