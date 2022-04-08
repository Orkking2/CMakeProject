#include "Image.h"

/*
Hex hexUtility;
std::vector<Point> bounds;
std::vector<int> size, RGB, axesRGB, backRGB;
std::vector<Pixel> pixels;
Point transVector;
double transSlope[2];
*/
PersonalImage::PersonalImage(std::vector<Point> xyBounds, std::vector<int> sizeIn, std::vector<int> backgroundRGB) : size(sizeIn), bounds(xyBounds) {

	for (Point p : Matrix(xyBounds[0], xyBounds[1], size).getMatrix()) {

	}
}

void PersonalImage::SetNewRGBStr(std::string s, std::vector<int> i) {
	if (i.size() == 3) {
		bool legalityFlag = true;
		for (std::string sin : colourSwitchLegalIndexes) {
			if (sin == s) {
				std::cout << "Error: Attempted to add a colourSwitchLegalIndex which already exists.\n";
				std::cout << "Attempted to add string " << s << " to colourSwitch index.\n";
				std::cout << "This value already exists and is set to ";
				for (int j : colourSwitch[s]) {
					std::cout << j << " ";
				}
				legalityFlag = false;
				break;
			}
		}
		if (legalityFlag) {
			colourSwitch[s] = i;
			colourSwitchLegalIndexes.emplace_back(s);
		}
	}
	else {
		std::cout << "Invalid RGB\n";
	}
}
void PersonalImage::PlotPoint(Point p) {
	p = TranslatePoint(p);
	IncludePixel(Pixel(p.x, p.y, RGB));
}
void PersonalImage::PlotSegment(Segment s) {
	s = TranslateSegment(s);
	s.ToInt();
	if (s.p2.x > s.p1.x) {
		s.Reverse();
	}
	double slope = (s.p1.y - s.p2.y) / (s.p1.x - s.p2.x);
	int xDist = s.p1.x - s.p2.x;
	for (int i = 0; i <= xDist; i++) {
		IncludePixel(Pixel(i + s.p1.x, slope * i + s.p1.y, RGB));
	}
}
void PersonalImage::PlotSegmentVector(std::vector<Segment> segments) {
	for (int i = 0; i < segments.size(); i++) {
		PlotSegment(segments[i]);
	}
}
void PersonalImage::PlotParticle(Particle p) {
	IncludePixel(Pixel(p.p.x, p.p.y, RGB));
	while (p.p.x > 0 && p.p.y > 0 && p.p.x <= size[0] && p.p.y <= size[1]) {
		p.Update();
		IncludePixel(Pixel(p.p.x, p.p.y, RGB));
	}
}
void PersonalImage::PlotEXFunction(std::string f, int bounds[2]) {
	std::deque<Token> queue = ParseExpression(f);
	int xDist = static_cast<int> ((bounds[1] - bounds[0]) * transSlope[0]);
	for (int i = 0; i <= xDist; i++) {
		int x = (i - transVector.x) / transSlope[0] + bounds[0];
		int y = static_cast<int> (EvaluateExpression(queue, std::map<char, double> { { 'x', x } }));
		Point p = TranslatePoint(Point(x, y));
		IncludePixel(Pixel(p.x, p.y, RGB));
	}
}
void PersonalImage::PlotIMPFunction(std::string f) {
	for (int i = 0; i < f.length(); i++) {
		if (f[i] == '=') {
			f[i] = '-';
			break;
		}
	}
	std::deque<Token> queue = ParseExpression(f);
	for (Pixel pix : pixels) {
		Pixel p = UntranslatePixel(pix);
		if (abs(EvaluateExpression(queue, std::map<char, double> { { 'x', p.x }, { 'y', p.y } })) <= 1) {
			IncludePixel(pix);
		}
	}
}
void PersonalImage::PlotAxes() {
	for (int i = 0; i < size[0]; i++) {
		IncludePixel(Pixel(i, transVector.y, axesRGB));
	}
	for (int i = 0; i < size[1]; i++) {
		IncludePixel(Pixel(transVector.x, i, axesRGB));
	}
}
std::vector<int> PersonalImage::HexToRGB(std::string in) {
	std::string inSplit[3];
	std::vector<int> out;
	for (int i = 0; i < 3; i++) {
		inSplit[i] = in[2 * i] + in[1 + 2 * i];
	}
	for (std::string s : inSplit) {
		out.push_back(hexUtility.HexToInt(s));
	}
	return out;
}