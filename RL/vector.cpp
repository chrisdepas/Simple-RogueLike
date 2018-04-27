#include "stdafx.h"
#include "vector.h"


Vector2i::Vector2i( int _X, int _Y )
{
	X = _X;
	Y = _Y;
}
float Vector2i::Length()
{
	return (float)sqrt(X*X + Y*Y);
}
Vector2i Vector2i::operator/( int factor )
{
	return Vector2i(X / factor, Y / factor);
}
Vector2i Vector2i::operator-( Vector2i c )
{
	return Vector2i( X - c.X, Y - c.Y );
}
Vector2i Vector2i::operator*( int factor )
{
	return Vector2i( X*factor, Y*factor );
}
bool Vector2i::operator==( Vector2i c )
{
	return ( c.X == X && c.Y == Y );
}
bool Vector2i::operator!=( Vector2i c )
{
	return ( !( c.X == X && c.Y == Y ) );
}

Vector2f::Vector2f( float _X, float _Y )
{
	X = _X;
	Y = _Y;
}
float Vector2f::Length()
{
	return sqrt( X*X + Y*Y );
}
Vector2f Vector2f::operator/( float f )
{
	return Vector2f( X/f, Y/f );
}
void Vector2f::operator+=( Vector2f c )
{
	X += c.X;
	Y += c.Y;
}
Vector2f Vector2f::operator*( float f )
{
	return Vector2f( X*f, Y*f );
}
float Vector3f::Length()
{
	return sqrt( X*X + Y*Y + Z*Z );
}