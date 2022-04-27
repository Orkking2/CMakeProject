#pragma once

#ifndef _GEOMETRY_
#define _GEOMETRY_

#include "ShuntingYard.h"
#include <vector>
#include <iostream>

struct Point {
	Point() : x(0), y(0) {}
	Point(double XComponent, double YComponent) : x(XComponent), y(YComponent) {}
	void Scale(double scaler);
	inline void ToInt() {
		x = static_cast<int> (x);
		y = static_cast<int> (y);
	}
	inline bool inRadius(Point p, double r) {
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2)) <= r;
	}
	inline void Translate(double transSlope[2], Point transVector) {
		x *= transSlope[0];
		x += transVector.x;
		y *= transSlope[1];
		y += transVector.y;
	}

	friend std::ostream& operator << (std::ostream& os, const Point& p);
	friend inline bool operator < (const Point& l, const Point& r);
	friend inline bool operator > (const Point& l, const Point& r);
	friend inline bool operator <= (const Point& l, const Point& r);
	friend inline bool operator >= (const Point& l, const Point& r);

	Point operator + (const Point& p) {
		return Point(x + p.x, y + p.y);
	}

	inline bool operator == (const Point& p) {
		return x == p.x && y == p.y;
	}

	inline bool operator != (const Point& p) {
		return !(x == p.x || y == p.y);
	}

	inline void operator += (const Point& p) {
		x += p.x;
		y += p.y;
	}

	double x, y;
};

struct Segment {
	Segment(Point point1 = Point(), Point point2 = Point()) {
		p1 = point1;
		p2 = point2;
	}
	Point p1, p2;

	inline void Scale(double scaler) {
		p1.Scale(scaler);
		p2.Scale(scaler);
	}
	inline void Reverse() {
		Segment s(p2, p1);
		p1 = s.p1;
		p2 = s.p2;
	}
	inline void Order() {
		if (p2 > p1) {
			Reverse();
		}
	}
	inline void Translate(Point vector) {
		p1 += vector;
		p2 += vector;
	}
	inline void ToInt() {
		p1.ToInt();
		p2.ToInt();
	}
	friend inline bool operator < (const Segment& l, const Segment& r);
	Point operator ++ () {
		return p1 + p2;
	}
	inline void operator += (const Segment& s) {
		p1 += s.p1;
		p2 += s.p1;
	}
};

inline double InvSigmoid(double x, double a) {
	return -log(a / (x + 0.5 * a) - 1);
}

struct Particle {
	Particle(Point origin = Point(), double thetaIn = 0) {
		p = origin;
		theta = thetaIn;
		vector = Point(r * cos(theta), r * sin(theta));
	}
	inline double DeltaThetaGen(int seedIn = 0) {
		return 1 / 3 * InvSigmoid(((rand() + seedIn) % 1000 - 500) / 50, 10); // https://www.desmos.com/calculator/hmxc9uegiz
	}
	inline void UpdateVector(int seedIn = 0) {
		theta += DeltaThetaGen(seedIn);
		vector = Point(r * cos(theta), r * sin(theta));
	}
	inline void UpdatePoint(Point vector) {
		p += vector;
	}
	inline void UpdatePoint() {
		p += vector;
	}
	void Update() {
		UpdatePoint();
		UpdateVector();
	}
	Point p, vector;
	double theta;
	static constexpr double r = 0.5;
};

struct Slope {
	Slope(std::string s) {
		queue = std::make_unique<std::deque<Token>>(ParseExpression(s));
	}
	double GetSlope(double x, double y) {
		std::map<char, double> m;
		m['x'] = x;
		m['y'] = y;
		return EvaluateExpression(*queue, m);
	}
	std::unique_ptr<std::deque<Token>> queue;
};

class Matrix {
public:
	Matrix() = default;
	Matrix(Point pMin, Point pMax, std::vector<int> dimensions);
	const std::vector<Point>& getMatrix() {
		return data;
	}
protected:
	std::vector<Point> data;
	std::vector<int> d;
	double yDist = 0, xDist = 0;
	Point startMin, startMax;
	std::vector<double> xList, yList;
};

class SlopeField : public Matrix {
public:
	SlopeField(Point pMin, Point pMax, std::vector<int> dimensions, std::string string) : Matrix(pMin, pMax, dimensions) {
		GenSegments(string);
	}
	const std::vector<Segment>& GetSegments() const {
		return segments;
	}
	const std::vector<Point>& GetBounds() const {
		return bounds;
	}
private:
	void GenSegments(std::string string);

	// Members
	std::vector<Point> data;
	std::vector<Segment> segments;
	std::vector<Point> bounds;
};

std::vector<double> NHSelect(double targetX, double x);

std::vector<Point> Approximate(std::vector<double> in, bool isTesting);

std::vector<double> StartSelect();

inline double slope(double x, double y) {
	return (pow(x, 2) + pow(y, 2) - 2 * x * y - 3);  // x^2 + y^2 - 2xy - 3
}

#endif // ifndef _GEOMETRY_