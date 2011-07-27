-- This file contains some Lua functions to control shaders semantics

function worldViewProj()
	renderer = getRenderer()
	matWorld = renderer:getWorldMatrix()
	matView = renderer:getViewMatrix()
	matProj = renderer:getProjectionMatrix()
	
	return matWorld * matView * matProj
end

-- Function returning projection matrix of the active camera's frustum
function cameraProjection()
	camera = getEngine():getActiveCamera()
	frustum = camera:getFrustum()
	
	return frustum:getProjectionMatrix()
end

-- Function returning camera forward vector
function cameraForward()
	camera = getEngine():getActiveCamera()
	
	return camera:getForward()
end

-- Function returning combined camera view projection matrix
function cameraViewProjection()
	camera = getEngine():getActiveCamera()
	frustum = camera:getFrustum()
	
	return camera:getViewMatrix() * frustum:getProjectionMatrix()
end