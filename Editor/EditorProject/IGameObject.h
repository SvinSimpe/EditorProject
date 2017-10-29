#pragma once

/*
	*Note: Consider removing Render
*/

__interface IGameObject
{
	virtual void Update( float deltaTime ) = 0;
	virtual void Render() = 0;

};