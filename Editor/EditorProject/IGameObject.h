#pragma once

__interface IGameObject
{
	virtual void Update( float deltaTime ) = 0;
	virtual void Render() = 0;

};