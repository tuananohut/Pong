#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"

class CTransform
{
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform(const Vec2& p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a) {}

};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setPointCount(points);
		circle.setOutlineThickness(thickness);
	}
};

class CRectangle
{
public:
	sf::RectangleShape rectangle;

	CRectangle(float width, float height, const sf::Color& fill)
		: rectangle(sf::Vector2f(width, height))
	{
		rectangle.setOutlineColor(fill);
	}
};

class CCollision
{
public:
	float radius = 0;

	CCollision(float r)
		: radius(r) {}

};

class CInput
{
public:
	bool up = false;
	bool down = false;

	CInput() {}
};

class CScore
{
public:
	int score = 0;

	CScore(int s)
		: score(s) {}
};

class CLifespan
{
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int total)
		: remaining(total), total(total)
	{}

};