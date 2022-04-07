#pragma once

#include <vector>
#include "ShuntingYard.h"
#include "Geometry.h"
#include "Utilities.h"

struct Pixel {
	Pixel(int xIn = 0, int yIn = 0, std::vector<int> RGBIn = { 0,0,0 }) {
		x = xIn;
		y = yIn;
		RGB = RGBIn;
	}
	void ToInt() {
		x = static_cast<int> (x);
		y = static_cast<int> (y);
	}
	int x, y;
	std::vector<int> RGB;
};

class PersonalImage {
public:
	PersonalImage(std::vector<Pixel> baseImage) {
		pixels.resize(baseImage.size());
		int i = 0;
		for (Pixel p : baseImage) {
			pixels[i] = p;
			i++;
		}
	}
	PersonalImage(std::vector<Point> xyBounds, std::vector<int> sizeIn, std::vector<int> backgroundRGB);
	void ChangeBackgroundRGB(std::vector<int> newRGB) {
		for (Pixel p : pixels) {
			if (p.RGB == backRGB) {
				IncludePixel(Pixel(p.x, p.x, newRGB));
			}
		}
	}
	inline void SetRGBFromStr(std::string s) {
		RGB = HexToRGB(s);
	}
	inline void SetRGBFromHex(std::string HexIn) {
		RGB = HexToRGB(HexIn);
	}
	inline void SetRGB(std::vector<int> RGBIn) {
		RGB = RGBIn;
	}
	inline void SetAxesRGBFromStr(std::string s) {
		axesRGB = HexToRGB(s);
	}
	inline void SetAxesRGBFromHex(std::string HexIn) {
		axesRGB = HexToRGB(HexIn);
	}
	inline void SetAxesRGB(std::vector<int> RGBIn) {
		axesRGB = RGBIn;
	}
	std::vector<Pixel> GetImage() {
		return pixels;
	}
	std::vector<Point> GetBounds() {
		return bounds;
	}
	std::vector<int> GetSize() {
		return size;
	}
	void PlotPoint(Point p);
	void PlotSegment(Segment s);
	void PlotSegmentVector(std::vector<Segment> segments);
	void PlotParticle(Particle p);
	void PlotEXFunction(std::string f, int bounds[2]);
	void PlotIMPFunction(std::string f);
	void SetNewRGBStr(std::string s, std::vector<int> i);
	std::vector<std::string> colourSwitchLegalIndexes = {
		"black",
		"blue",
		"lime",
		"red",
		"yellow",
		"magenta",
		"cyan",
		"white",
		"silver",
		"grey",
		"maroon",
		"green",
		"navy",
		"olive",
		"purple",
		"teal"
	};
	std::map<std::string, std::vector<int>> colourSwitch = {
		{"black",	{	0,	 0,	  0 } },
		{"blue",	{   0,   0, 255 } },
		{"lime",	{   0, 255,   0 } },
		{"red",		{ 255,   0,	  0 } },
		{"yellow",	{ 255, 255,   0 } },
		{"magenta",	{ 255,   0, 255 } },
		{"cyan",	{   0, 255, 255 } },
		{"white",	{ 255, 255, 255 } },
		{"silver",	{ 192, 192, 192 } },
		{"grey",	{ 128, 128, 128 } },
		{"maroon",	{ 128,   0,   0 } },
		{"green",	{   0, 128,   0 } },
		{"navy",	{   0,   0, 128 } },
		{"olive",	{ 128, 128,   0 } },
		{"purple",	{ 128,   0, 128 } },
		{"teal",	{   0, 128, 128 } }
	};
private:
	void PlotAxes();
	void IncludePixel(Pixel p) {
		p.ToInt();
		pixels[abs(p.x) + abs(p.y) * size[0]].RGB = p.RGB;
	}
	int UniqueStrInt(std::string s) {
		int out = 0;
		for (int i = 0; i < s.length(); i++) {
			out += s[i] * pow(100, i);
		}
		return out;
	}
	Point TranslatePoint(Point p) {
		p.Translate(transSlope, transVector);
		return p;
	}
	Pixel UntranslatePixel(Pixel p) {
		p = Pixel((p.x - transVector.x) / transSlope[0], (p.y - transVector.y) / transSlope[1], p.RGB);
		return p;
	}
	Segment TranslateSegment(Segment s) {
		s.p1 = TranslatePoint(s.p1);
		s.p2 = TranslatePoint(s.p2);
		return s;
	}
	std::string RGBToHex(std::vector<int> in) {
		std::string hex;
		for (int i : in) {
			hex += hexUtility.IntToHex(i);
		}
		return hex;
	}
	std::vector<int> HexToRGB(std::string in);

	Hex hexUtility;
	std::vector<Point> bounds;
	std::vector<int> size, RGB, axesRGB, backRGB;
	std::vector<Pixel> pixels;
	Point transVector;
	double transSlope[2];
};