#include "Geometry.h"



void Point::Scale(double scaler) {
	double r = sqrt(pow(x, 2) + pow(y, 2));
	double theta = atan(x * y);
	r *= scaler;
	x = r * cos(theta);
	y = r * sin(theta);
}



inline bool operator < (const Point& l, const Point& r) {
	return l.x < r.x && l.y < r.y;
}
inline bool operator > (const Point& l, const Point& r) {
	return l.x > r.x && l.y > r.y;
}
inline bool operator <= (const Point& l, const Point& r) {
	return l.x <= r.x && l.y <= r.y;
}
inline bool operator >= (const Point& l, const Point& r) {
	return l.x >= r.x && l.y >= r.y;
}
std::ostream& operator << (std::ostream& os, const Point& p) {
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}


Matrix::Matrix(Point pMin, Point pMax, std::vector<int> dimensions) {
	xDist = pMax.x - pMin.y;
	yDist = pMax.y - pMin.y;

	startMin = pMin;
	startMax = pMax;

	d = { dimensions[0], dimensions[1], dimensions[0] * dimensions[1] };

	data.resize(d[2]);

	double xSpacing = xDist / (d[0] - 1);
	double ySpacing = yDist / (d[1] - 1);

	for (int i = 0; i < d[0]; i++) {
		xList.push_back(i * xSpacing + pMin.x);
	}
	for (int i = 0; i < d[1]; i++) {
		yList.push_back(i * ySpacing + pMin.y);
	}

	for (int i = 0; i < d[2]; i++) {
		data.emplace_back(Point(xList[i % d[0]], yList[floor(i / d[0])]));
	}
}


inline bool operator < (const Segment& l, const Segment& r) {
	return l.p1 < r.p1&& l.p2 < r.p2;
}

std::vector<double> NHSelect(double targetX, double x) {
	int n;
	double h, nCap = pow(10, 7);
	char nhSelect;
	std::vector<double> out;

	std::cout << "N or H: ";
	std::cin >> nhSelect;
	if (tolower(nhSelect) == 'n') {
		std::cout << "N: ";
		std::cin >> n;
		if (n > nCap) {
			std::cout << "N > " << nCap << " -- N reset to " << nCap << std::endl;
			n = nCap;
		}
		else if (n <= 0) {
			std::cout << "N <= 0, reset to " << nCap << std::endl;
			n = nCap;
		}
		h = (targetX - x) / static_cast<double>(n);
		std::cout << "H = " << h << std::endl;
	}
	else if (tolower(nhSelect) == 'h') {
		std::cout << "H: ";
		std::cin >> h;
		n = (targetX - x) / h;
		if (n > nCap) {
			std::cout << "N > " << nCap << " -- N reset to " << nCap << std::endl;
			n = nCap;
			h = (targetX - x) / static_cast<double>(n);
			std::cout << "H = " << h << std::endl;
		}
		else if (n <= 0) {
			std::cout << "N <= 0, reset to " << nCap << std::endl;
			n = nCap;
			h = (targetX - x) / static_cast<double>(n);
			std::cout << "H = " << h << std::endl;
		}
	}
	else if (nhSelect == 'm') {
		n = pow(10, 8);
		h = (targetX - x) / static_cast<double>(n);
		std::cout << std::endl << "Max N selected " << std::endl << "N = " << n << std::endl << "H = " << h << std::endl;
	}
	else {
		std::cout << "Invalid char" << std::endl;
		out = NHSelect(targetX, x);
	}

	std::cout << std::endl;

	out.push_back(n);
	out.push_back(h);

	return out;
}

std::vector<Point> Approximate(std::vector<double> in, bool isTesting) {
	int n;
	double h, tRadius;
	Point startPoint;

	if (isTesting) {
		tRadius = 1 / 5000;
		n = pow(10, 7);
		h = 1 / n;
	}
	else {
		startPoint = Point(in[0], in[1]);

		std::vector<double> k;
		double targetX = in[2], xDist = abs(targetX - startPoint.x);
		tRadius = xDist / 5000;

		std::vector<double> nh = NHSelect(targetX, startPoint.x);

		n = static_cast<int> (nh[0]);
		h = nh[1];
	}

	Point iterativePoint = startPoint;
	std::vector<Point> pointList = { startPoint };

	int j = 0;

	for (int i = 0; i < n; i++) {
		iterativePoint += Point(h, slope(iterativePoint.x, iterativePoint.y) * h);
		if (!pointList[j].inRadius(iterativePoint, tRadius)) {
			pointList.push_back(iterativePoint);
			j++;
		}
	}

	if (pointList[pointList.size() - 1] != iterativePoint) {
		pointList.push_back(iterativePoint);
	}

	return pointList;
}

std::vector<double> StartSelect() {
	double startX, startY, targetX;
	// questions
	std::cout << "Start X: ";
	std::cin >> startX;
	std::cout << "Start Y: ";
	std::cin >> startY;
	std::cout << "Target X: ";
	std::cin >> targetX;
	// Out construction
	std::vector<double> out = { startX, startY, targetX };
	// Out
	return out;
}

void SlopeField::GenSegments(std::string string) { // Also gens bounds
	double r = std::min(xDist / (2 * d[0] - 1), yDist / (2 * d[1] - 1));
	Point pLow = data[0], pHigh = data[0];
	std::vector<double> k;
	Slope slope(string);

	for (int i = 0; i < d[2]; i++) {
		/*	-	-	-	-	-	-	-	-	*
		*	k explanation:					*
		*	* <---> * <---> * (diagram)		*
		*	|   k   |   k   | ~~ k = xDist	*
		*	* = p1  |       * = p2			*
		*		    * = data[i]             *
		*	-	-	-	-	-	-	-	-	*/

		k.push_back(r / sqrt(1 + pow(slope.GetSlope(data[i].x, data[i].y), 2)));

		// Defining points in segment
		Point pL(data[i].x - k[i], data[i].y - slope.GetSlope(data[i].x, data[i].y) * k[i]);
		Point pR(data[i].x + k[i], data[i].y + slope.GetSlope(data[i].x, data[i].y) * k[i]);

		// Evaluating bounds
		for (Point pCurr : {pL, pR}) {

			// pLow
			if (pLow.x > pCurr.x) {
				pLow.x = pCurr.x;
			}
			if (pLow.y > pCurr.y) {
				pLow.y = pCurr.y;
			}

			// pHigh
			if (pHigh.x < pCurr.x) {
				pHigh.x = pCurr.x;
			}
			if (pHigh.y < pCurr.y) {
				pHigh.y = pCurr.y;
			}
		}

		// Constructing Segment list
		segments.push_back(Segment(pL, pR));
	}

	// Constructing bounds list
	bounds.push_back(pLow);
	bounds.push_back(pHigh);
}