#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>

/// <summary>
/// 2 dimensional vector:
/// </summary>
struct Vector2 {
	float x;
	float y;
};

/// <summary>
/// typedef for N dimensional vector
/// </summary>
typedef std::vector<float> VectorN;

std::string to_string(const Vector2& value) {
	return "<" + std::to_string(value.x) + ", " + std::to_string(value.y) + ">";
}

Vector2 add(const Vector2& lhs, const Vector2& rhs) {
	Vector2 res;
	res.x = lhs.x + rhs.x;
	res.y = lhs.y + rhs.y;

	return res;
}

Vector2 sub(const Vector2& lhs, const Vector2& rhs) {
	Vector2 res;
	res.x = lhs.x - rhs.x;
	res.y = lhs.y - rhs.y;

	return res;
}

float dot(const Vector2& lhs, const Vector2& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

std::string to_string(const VectorN& value) {
	std::string res = "<";

	for (size_t i = 0; i < value.size(); i++)
	{
		if (i == value.size() -1)
		{
			res += std::to_string(value[i]);
		}
		else {
			res += std::to_string(value[i]) + ", ";
		}
	}

	res += ">";
	return res;
}

VectorN add(const VectorN& lhs, const VectorN& rhs) {
	assert(lhs.size() == rhs.size());
	VectorN res;
	for (size_t i = 0; i < lhs.size(); ++i) {
		res.push_back(lhs[i] + rhs[i]);
	}
	return res;
}

VectorN sub(const VectorN& lhs, const VectorN& rhs) {
	assert(lhs.size() == rhs.size());
	VectorN res;
	for (size_t i = 0; i < lhs.size(); ++i) {
		res.push_back(lhs[i] - rhs[i]);
	}
	return res;
}

float dot(const VectorN& lhs, const VectorN& rhs) {
	float res = 0;
	assert(lhs.size() == rhs.size());
	for (size_t i = 0; i < lhs.size(); ++i) {
		res += lhs[i] * rhs[i];
	}

	return res;
}


float distance(auto p1, auto p2) {
	/*
	float dx = p2.x-=p1.y;
	float dx = p2.y-=p1.y;
	return sqrt(dx*dx + dy*dy);
	*/

	auto c = sub(p2, p1);
	return sqrtf(dot(c, c));
}

float fastDistance2d(auto p1, auto p2) {
	auto c = sub(p2, p1);
	c.x = std::abs(c.x);
	c.x = std::abs(c.y);
	float min = std::min(c.x, c.y);
	// Something something approxemately calculated
	return (c.x + c.y - 0.5f * min - 0.25f * min + 0.125f * min);
}

int main() {
	std::string text = "Hello Text";
	printf("Floating point number: %f\n", 1.012345f);
	printf("Integer              : %d\n", 4);
	printf("String               : %s\n", text.c_str());

	{
		Vector2 a;
		a.x = 10.1f;
		a.y = 5.5f;

		Vector2 b;
		b.x = 2.1f;
		b.y = 3.3f;

		auto c = add(a, b);
		printf("2 dimensional vector:\n");
		printf("a: %s\n", to_string(a).c_str());
		printf("b: %s\n", to_string(b).c_str());
		printf("a+b: %s\n", to_string(c).c_str());
		printf("a-b: %s\n", to_string(sub(a, b)).c_str());
		printf("a.b: %f\n", dot(a, b));
	}

	{
		VectorN a = {0, 2, 4, 4, 6 };
		VectorN b = {0, 3, 5, 6, 2 };
		auto c = add(a, b);
		printf("5 dimensional vector:\n");
		printf("a  : %s\n", to_string(a).c_str());
		printf("b  : %s\n", to_string(b).c_str());
		printf("a+b: %s\n", to_string(c).c_str());
		printf("a-b: %s\n", to_string(sub(a, b)).c_str());
		printf("a.b: %s\n", std::to_string(dot(a, b)).c_str());
	}

	{
		Vector2 p2 = { 4,3 };
		Vector2 p1 = { 0,0 };
		printf("p1: %s\n", to_string(p1).c_str());
		printf("p2: %s\n", to_string(p2).c_str());

		float lenC = distance(p1, p2);
		float fastLenC = fastDistance2d(p1, p2);
		printf("distance(p1,p2): %f\n", lenC);
		printf("fastDistance(p1,p2): %f\n", fastLenC);
	}

	return 0;
}