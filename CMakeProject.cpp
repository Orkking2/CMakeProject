// CMakeProject.cpp : Defines the entry point for the application.
//

#include "CMakeProject.h"


namespace ShntYrd
{
	struct Token {
		enum struct Type {
			Unknown,
			Number,
			Operator,
			LeftParen,
			RightParen,
			Variable,
		};

		Token(Type t, const std::string& s, int prec = -1, bool ra = false) : type{ t }, str(s), precedence{ prec }, rightAssociative{ ra }	{}

		const Type type;
		const std::string str;
		const int precedence;
		const bool rightAssociative;
	};

	std::deque<Token> ParseExpression(std::string& string) {
		std::deque<Token> tokens;

		for (const auto* c = string.c_str(); *c; c++) {
			if (isblank(*c)) {
				// pass
			}
			else if (isdigit(*c)) {
				const auto* b = c;
				auto* e = c;

				while (isdigit(*e)) {
					e++;
				}
				const auto s = std::string(b, e);
				tokens.push_back(Token{ Token::Type::Number, s });
				c += s.size();
			}
			else if (isalpha(*c)) {
				tokens.push_back(Token{ Token::Type::Variable, std::string(1, *c) });
			}
			else {
				Token::Type t = Token::Type::Unknown;
				int pr = -1;
				bool ra = false;
				switch (*c) {
				default:                                    break;
				case '(':   t = Token::Type::LeftParen;     break;
				case ')':   t = Token::Type::RightParen;    break;
				case '_':   t = Token::Type::Operator;      pr = 5; ra = true; break;
				case '^':   t = Token::Type::Operator;      pr = 4; ra = true; break;
				case '*':   t = Token::Type::Operator;      pr = 3; break;
				case '/':   t = Token::Type::Operator;      pr = 3; break;
				case '%':   t = Token::Type::Operator;      pr = 3; break;
				case '+':   t = Token::Type::Operator;      pr = 2; break;
				case '-':   t = Token::Type::Operator;      pr = 2; break;
				}
				const auto s = std::string(1, *c);
				tokens.push_back(Token{ t, s, pr, ra });
			}
		}

		// ShuntingYard

		std::deque<Token> queue;
		std::vector<Token> stack;

		for (Token token : tokens) {
			switch (token.type) {
			case Token::Type::Number:
				queue.push_back(token);
				break;

			case Token::Type::Operator:
			{
				const auto o1 = token;
				while (!stack.empty()) {
					const auto o2 = stack.back();
					if ((!o1.rightAssociative && o1.precedence <= o2.precedence) || (o1.rightAssociative && o1.precedence < o2.precedence)) {
						stack.pop_back();
						queue.push_back(o2);

						continue;
					}
					break;
				}
				stack.push_back(o1);
				break;
			}
			case Token::Type::LeftParen:
				stack.push_back(token);
				break;

			case Token::Type::RightParen:
			{
				bool match = false;
				while (!stack.empty() && stack.back().type != Token::Type::LeftParen) {
					queue.push_back(stack.back());
					stack.pop_back();
					match = true;
				}
				stack.pop_back();

				if (!match && stack.empty()) {
					printf("RightParen error (%s)\n", token.str.c_str());
					return {};
				}
				break;
			}
			default:
				printf("error (%s)\n", token.str.c_str());
				return {};
			}
		}
		while (!stack.empty()) {
			if (stack.back().type == Token::Type::LeftParen) {
				printf("Mismatched parentheses error\n");
				return {};
			}

			queue.push_back(std::move(stack.back()));
			stack.pop_back();
		}
		return queue;
	}

	double EvaluateExpression(std::deque<Token> queue, std::map<char, double> vars) {
		std::vector<int> stack;
		while (!queue.empty()) {
			const auto token = queue.front();
			queue.pop_front();
			switch (token.type) {
			case Token::Type::Number:
				stack.push_back(std::stoi(token.str));
				break;
			case Token::Type::Variable:
				stack.push_back(vars[token.str[0]]);
				break;
			case Token::Type::Operator:
			{
				const auto r = stack.back();
				stack.pop_back();
				if (token.str[0] == '_') {
					stack.push_back(-r);
					break;
				}
				const auto l = stack.back();
				stack.pop_back();

				switch (token.str[0]) {
				default:
					printf("Operator error [%s]\n", token.str.c_str());
					exit(0);
					break;
				case '^':	stack.push_back(static_cast<int>(pow(l, r)));	break;
				case '*':	stack.push_back(l * r);							break;
				case '/':	stack.push_back(l / r);							break;
				case '+':	stack.push_back(l + r);							break;
				case '-':	stack.push_back(l - r);							break;
				case '%':	stack.push_back(l % r);							break;
				}
			}
			break;

			default:
				printf("Token error\n");
				exit(0);
			}
		}
		return stack.back();
	}

}


struct Slope {
	Slope(std::string s) {
		queue = std::make_unique<std::deque<ShntYrd::Token>>(ShntYrd::ParseExpression(s));
	}
	double GetSlope(double x, double y) {
		std::map<char, double> m;
		m['x'] = x;
		m['y'] = y;
		return ShntYrd::EvaluateExpression(*queue, m);
	}
	std::unique_ptr<std::deque<ShntYrd::Token>> queue;
};

double slope(double x, double y) {
	return (pow(x, 2) + pow(y, 2) - 2 * x * y - 3);  // x^2 + y^2 - 2xy - 3
}

struct Point {
	Point() : x(0), y(0) {}
	Point(double XComponent, double YComponent) {
		x = XComponent;
		y = YComponent;
	}
	double x, y;
	void Scale(double scaler) {
		double r = sqrt(pow(x, 2) + pow(y, 2));
		double theta = atan(x * y);
		r *= scaler;
		x = r * cos(theta);
		y = r * sin(theta);
	}
	void ToInt() {
		x = static_cast<int> (x);
		y = static_cast<int> (y);
	}
	bool inRadius(Point p, double r) {
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2)) <= r;
	}
	void Translate(double transSlope[2], Point transVector) {
		x *= transSlope[0];
		x += transVector.x;
		y *= transSlope[1];
		y += transVector.y;
	}

	friend std::ostream& operator << (std::ostream& os, const Point& p);
	friend bool operator < (const Point& l, const Point& r);
	friend bool operator > (const Point& l, const Point& r);
	friend bool operator <= (const Point& l, const Point& r);
	friend bool operator >= (const Point& l, const Point& r);

	Point operator + (const Point& p) {
		return Point(x + p.x, y + p.y);
	}

	bool operator == (const Point& p) {
		return x == p.x && y == p.y;
	}

	bool operator != (const Point& p) {
		return !(x == p.x || y == p.y);
	}

	void operator += (const Point& p) {
		x += p.x;
		y += p.y;
	}
};

bool operator < (const Point& l, const Point& r) {
	return l.x < r.x&& l.y < r.y;
}
bool operator > (const Point& l, const Point& r) {
	return l.x > r.x && l.y > r.y;
}
bool operator <= (const Point& l, const Point& r) {
	return l.x <= r.x && l.y <= r.y;
}
bool operator >= (const Point& l, const Point& r) {
	return l.x >= r.x && l.y >= r.y;
}

std::ostream& operator << (std::ostream& os, const Point& p) {
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}


struct Segment {
	Segment(Point point1 = Point(), Point point2 = Point()) {
		p1 = point1;
		p2 = point2;
	}
	Point p1, p2;

	void Scale(double scaler) {
		p1.Scale(scaler);
		p2.Scale(scaler);
	}
	void Reverse() {
		Segment s(p2, p1);
		p1 = s.p1;
		p2 = s.p2;
	}
	void Order() {
		if (p2 > p1) {
			Reverse();
		}
	}
	void Translate(Point vector) {
		p1 += vector;
		p2 += vector;
	}
	void ToInt() {
		p1.ToInt();
		p2.ToInt();
	}
	friend bool operator < (const Segment& l, const Segment& r);
	Point operator ++ () {
		return p1 + p2;
	}
	void operator += (const Segment& s) {
		p1 += s.p1;
		p2 += s.p1;
	}
};

bool operator < (const Segment& l, const Segment& r) {
	return l.p1 < r.p1&& l.p2 < r.p2;
}


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


double InvSigmoid(double x, double a) {
	return -log(a / (x + 0.5 * a) - 1);
}


struct Particle {
	Particle(Point origin = Point(), double thetaIn = 0) {
		p = origin;
		theta = thetaIn;
		vector = Point(r * cos(theta), r * sin(theta));
	}
	double DeltaThetaGen(int seedIn = 0) {
		return 1 / 3 * InvSigmoid(((rand() + seedIn) % 1000 - 500) / 50, 10); // https://www.desmos.com/calculator/hmxc9uegiz
	}
	void UpdateVector(int seedIn = 0) {
		theta += DeltaThetaGen(seedIn);
		vector = Point(r * cos(theta), r * sin(theta));
	}
	void UpdatePoint(Point vector) {
		p += vector;
	}
	void UpdatePoint() {
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


namespace Mtx
{
	class Matrix {
	public:
		Matrix() = default;
		Matrix(Point pMin, Point pMax, std::vector<int> dimensions) {
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

}


class SlopeField : public Mtx::Matrix {
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
	void GenSegments(std::string string) { // Also gens bounds
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

	// Members
	std::vector<Point> data;
	std::vector<Segment> segments;
	std::vector<Point> bounds;
};


// Template template
/*
template <typename T>
class container{
	T x;
	T y;
};
*/


namespace utl
{
	struct Hex {
		Hex() {
			for (int i = 0; i < key.size(); i++) {
				map[key[i]] = i;
			}
		}
		int HexToInt(std::string in) {
			int out = 0;
			for (int i = 0; i < in.size(); i++) {
				out += in[i] * pow(16, i);
			}
			return out;
		}
		std::string IntToHex(int in) {
			std::string out;
			while (in) {
				out += key[in % 16];
				in /= 16;
			}
			return out;
		}
		std::map<char, int> map;
		std::string key = "0123456789ABCDEF";
	};

	std::vector<std::vector<double>> PtListToXYList(std::vector<Point> pList) {
		std::vector<double> xList, yList;
		for (Point p : pList) {
			xList.push_back(p.x);
			yList.push_back(p.y);
		}
		return { xList, yList };
	}

	template <class Item>
	void Sort(std::vector<Item>& list, std::function<bool(const Item& l, const Item& r)> c) {
		bool done = false;
		while (!done) {
			done = true;
			for (int i = 1; i < list.size(); i++) {
				if (c(list[i - 1], list[i])) {
					std::swap(list[i - 1], list[i]);
					done = false;
				}
			}
		}
	}

	std::string CharVectorToString(std::vector<char> in) {
		std::string out;
		for (char c : in) {
			out.push_back(c);
		}
		return out;
	}

	std::vector<char> StringToCharVector(std::string in) {
		std::vector<char> out;
		for (char c : in) {
			out.push_back(c);
		}
		return out;
	}

	std::deque<bool> AppendBoolDeque(std::vector<std::deque<bool>> in) {
		std::deque<bool> out;
		for (std::deque<bool> deque : in) {
			while (!deque.empty()) {
				out.push_back(deque.front());
				deque.pop_front();
			}
		}
		return out;
	}

	template<typename I, class Hasher = std::hash<I>>
	std::unordered_map<I, int, Hasher> GenFreq(std::vector<I> in) {
		std::unordered_map<I, int, Hasher> out;
		for (I i : in) out[i]++;
		return out;
	}

	struct Hash {
		std::size_t operator () (const Point& p) {
			std::size_t x = std::hash<double>{}(p.x);
			std::size_t y = std::hash<double>{}(p.y);
			return x ^ (y << 1);
		}
		std::size_t operator() (const Segment& s) {
			std::size_t p1 = Hash{}(s.p1);
			std::size_t p2 = Hash{}(s.p2);
			return p1 ^ (p2 << 1);
		}
		std::size_t operator() (const Particle& p) {
			std::size_t origin = Hash{}(p.p);
			std::size_t theta = std::hash<double>{}(p.theta);
			return origin ^ (theta << 1);
		}
		std::size_t operator() (const Pixel& p) {
			std::size_t point = Hash{}(Point(p.x, p.y));
			std::size_t RGB = 0;
			for (int i : p.RGB) {
				RGB ^= (std::hash<int>{}(i) << i);
			}
			return point ^ (RGB << 1);
		}
		std::size_t operator() (PersonalImage image) {
			std::size_t out = 0;
			for (Pixel p : image.GetImage()) {
				out ^= (Hash{}(p) << 1);
			}
		}
		std::size_t operator() (Mtx::Matrix m) {
			std::size_t out = 0;
			for (Point p : m.getMatrix()) {
				out ^= Hash{}(p);
			}
		}
	};

}


// bool capture inside [] to sue within function
// utl::Sort(list, [](l, r) {return false;});		* [&] captures everything by reference and [=] captures an unreferenced copy



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
	PersonalImage(std::vector<Point> xyBounds, std::vector<int> sizeIn, std::vector<int> backgroundRGB) {
		bounds = xyBounds;
		size = sizeIn;
		backRGB = backgroundRGB;
		int areaReal = size[0] * size[1];
		pixels.resize(areaReal);
		int i = 0;
		for (Point p : Mtx::Matrix(Point(0, 0), Point(sizeIn[0], sizeIn[1]), sizeIn).getMatrix()) {
			pixels[i] = Pixel(p.x, p.y, backgroundRGB);
			i++;
		}
		transSlope[0] = -size[0] / (bounds[0].x - bounds[1].x);
		transSlope[1] = -size[1] / (bounds[0].y - bounds[1].y);
		transVector = Point(-transSlope[0] * bounds[0].x, -transSlope[1] * bounds[0].y); // (Min x, Min y) -> (0,0)
	}
	void ChangeBackgroundRGB(std::vector<int> newRGB) {
		for (Pixel p : pixels) {
			if (p.RGB == backRGB) {
				IncludePixel(Pixel(p.x, p.x, newRGB));
			}
		}
	}
	void SetRGBFromStr(std::string s) {
		RGB = HexToRGB(s);
	}
	void SetRGBFromHex(std::string HexIn) {
		RGB = HexToRGB(HexIn);
	}
	void SetRGB(std::vector<int> RGBIn) {
		RGB = RGBIn;
	}
	void SetAxesRGBFromStr(std::string s) {
		axesRGB = HexToRGB(s);
	}
	void SetAxesRGBFromHex(std::string HexIn) {
		axesRGB = HexToRGB(HexIn);
	}
	void SetAxesRGB(std::vector<int> RGBIn) {
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
	void PlotPoint(Point p) {
		p = TranslatePoint(p);
		IncludePixel(Pixel(p.x, p.y, RGB));
	}
	void PlotSegment(Segment s) {
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
	void PlotSegmentVector(std::vector<Segment> segments) {
		for (int i = 0; i < segments.size(); i++) {
			PlotSegment(segments[i]);
		}
	}
	void PlotParticle(Particle p) {
		IncludePixel(Pixel(p.p.x, p.p.y, RGB));
		while (p.p.x > 0 && p.p.y > 0 && p.p.x <= size[0] && p.p.y <= size[1]) {
			p.Update();
			IncludePixel(Pixel(p.p.x, p.p.y, RGB));
		}
	}
	void PlotEXFunction(std::string f, int bounds[2]) {
		std::deque<ShntYrd::Token> queue = ShntYrd::ParseExpression(f);
		int xDist = static_cast<int> ((bounds[1] - bounds[0]) * transSlope[0]);
		for (int i = 0; i <= xDist; i++) {
			int x = (i - transVector.x) / transSlope[0] + bounds[0];
			int y = static_cast<int> (ShntYrd::EvaluateExpression(queue, std::map<char, double> { { 'x', x } }));
			Point p = TranslatePoint(Point(x, y));
			IncludePixel(Pixel(p.x, p.y, RGB));
		}
	}
	void PlotIMPFunction(std::string f) {
		for (int i = 0; i < f.length(); i++) {
			if (f[i] == '=') {
				f[i] = '-';
				break;
			}
		}
		std::deque<ShntYrd::Token> queue = ShntYrd::ParseExpression(f);
		for (Pixel pix : pixels) {
			Pixel p = UntranslatePixel(pix);
			if (abs(ShntYrd::EvaluateExpression(queue, std::map<char, double> { { 'x', p.x }, { 'y', p.y } })) <= 1) {
				IncludePixel(pix);
			}
		}
	}
	void SetNewRGBStr(std::string s, std::vector<int> i) {
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
	void IncludePixel(Pixel p) {
		p.ToInt();
		pixels[abs(p.x) + abs(p.y) * size[0]].RGB = p.RGB;
	}
	void PlotAxes() {
		for (int i = 0; i < size[0]; i++) {
			IncludePixel(Pixel(i, transVector.y, axesRGB));
		}
		for (int i = 0; i < size[1]; i++) {
			IncludePixel(Pixel(transVector.x, i, axesRGB));
		}
	}
	int UniqueStrInt(std::string s) {
		int out = 0;
		for (int i = 0; i < s.length(); i++) {
			out += s[i] * pow(100, i);
		}

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
	std::vector<int> HexToRGB(std::string in) {
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

	utl::Hex hexUtility;
	std::vector<Point> bounds;
	std::vector<int> size, RGB, axesRGB, backRGB;
	std::vector<Pixel> pixels;
	Point transVector;
	double transSlope[2];
};

namespace Huff
{
	template <typename I>
	struct Element {
		bool isSetFlag = false;
		virtual ~Element() = default;
		virtual std::vector<I> GetItemList() = 0;
		virtual I GetItem(std::deque<bool>&) = 0;
		virtual int GetFreq() = 0;
		virtual bool isNode() = 0;
	};

	template <typename I>
	class Item : public Element<I> {
	public:
		Item() : i(I), freq(0) {}
		Item(I in, int f) : i(in), freq(f) {}

		I GetItem(std::deque<bool>& nothing) override {
			return i;
		}
		std::vector<I> GetItemList() override {
			return std::vector<I> {i};
		}
		int GetFreq() override {
			return freq;
		}
		bool isNode() override {
			return false;
		}
	private:
		int freq;
		I i;
	};

	template <typename I>
	struct Node : public Element<I> {
		Node() : _left(nullptr), _right(nullptr) {}
		Node(std::unique_ptr<Element<I>> left, std::unique_ptr<Element<I>> right) {
			_left = std::move(left);
			_right = std::move(right);
		}
		std::vector<I> GetItemList() override {
			std::vector<I> out = _right->GetItemList();
			out.push_back(_left->GetItemList());
			return out;
		}
		I GetItem(std::deque<bool>& index) override {
			bool casheBool = index.front();
			index.pop_front();
			if (casheBool) return _right->GetChar(index);
			else		   return _left->GetChar(index);
		}
		int GetFreq() override {
			int i = 0;
			if (_left != nullptr) i += _left->GetFreq();
			if (_right != nullptr) i += _right->GetFreq();
			return i;
		}
		bool isNode() override {
			return true;
		}
		void operator = (const Node& n) {
			_left = std::make_unique<Node<I>>(n._left);
			_right = std::make_unique<Node<I>>(n._right);
		}

		std::unique_ptr<Element<I>> _left, _right;
	};

	template <typename I, class Hasher = std::hash<I>>
	class Tree {
		Tree(std::vector<I> orderedKey, std::unordered_map<I, int, Hasher> freq) {

			// Initializing Char deque using Ordered key & paired frequency | basically just converting data types
			std::deque<Item<I>> initCharList;
			for (I i : orderedKey) initCharList.push_back(Item<I>(i, freq[i]));


			// Initializing tree -- initial construction always includes a node with the two least frequent items
			Item<I> cashedChar = initCharList.front();
			initCharList.pop_front();
			std::vector<Node<I>> nodes = { Node<I>(std::make_unique<Item<I>>(cashedChar), std::make_unique<Item<I>>(initCharList.front())) };
			_ItemBoolIndex[initCharList.front().GetItemList()[0]].push_back(true);
			_ItemBoolIndex[cashedChar.GetItemList()[0]].push_back(false);
			initCharList.pop_front();

			// Initializing cashes
			Node<I> cashedNode;
			cashedChar.isSetFlag = false;
			cashedChar.isSetFlag = false;

			// Constructing tree
			while (!nodes.empty()) {

				// Setting cashes
				if (!cashedChar.isSetFlag) {
					cashedChar = initCharList.front();
					initCharList.pop_front();
					cashedChar.isSetFlag = true;
				}
				if (!cashedNode.isSetFlag) {
					cashedNode = nodes[0];
					int tempIndex = 0;
					for (int i = 1; i < nodes.size(); i++) {
						if (nodes[i].GetFreq() < cashedNode.GetFreq()) {
							cashedNode = nodes[i];
							tempIndex = i;
						}
					}
					nodes.erase(nodes.begin() + tempIndex);
					cashedNode.isSetFlag = true;
				}

				{	// Combiner logic -- artificial scope so tempNode and tempIndex always get re-initialized
					Node<I> tempNode = nodes[0];
					int tempIndex = 0;
					for (int i = 1; i < nodes.size(); i++) {
						if (nodes[i].GetFreq() < tempNode.GetFreq()) {
							tempNode = nodes[i];
							tempIndex = i;
						}
					}
					if (tempNode.GetFreq() < cashedChar.GetFreq() || initCharList.empty()) {
						nodes.erase(nodes.begin() + tempIndex);
						nodes.emplace_back(Node<I>(std::make_unique<Node<I>>(tempNode), std::make_unique<Node<I>>(cashedNode)));
						cashedNode.isSetFlag = false;
						for (I i : tempNode.GetItemList()) _ItemBoolIndex[i].push_back(false);
						for (I i : cashedNode.GetItemList()) _ItemBoolIndex[i].push_back(true);
					}
					else if (cashedNode.GetFreq() < initCharList.front().GetFreq()) {
						nodes.emplace_back(Node<I>(std::make_unique<Node<I>>(cashedNode), std::make_unique<Item<I>>(cashedChar)));
						for (I i : cashedNode.GetItemList()) _ItemBoolIndex[i].push_back(false);
						_ItemBoolIndex[cashedChar.GetItemList()[0]].push_back(true);
						cashedNode.isSetFlag = false;
						cashedChar.isSetFlag = false;
					}
					else {
						tempNode = Node<I>(std::make_unique<Item<I>>(cashedChar), std::make_unique<Item<I>>(initCharList.front()));
						_ItemBoolIndex[cashedChar.GetItemList()[0]].push_back(false);
						_ItemBoolIndex[initCharList.front().GetItemList()[0]].push_back(true);
						initCharList.pop_front();

						// Check is new node is less frequent than current least frequent
						if (tempNode.GetFreq() < cashedNode.GetFreq()) {
							cashedNode.isSetFlag = false;				// Maintining false isSetFlag initialization for all Nodes in nodes
							nodes.emplace_back(cashedNode);
							cashedNode = tempNode;
							cashedNode.isSetFlag = true;
						}
						else {
							nodes.emplace_back(tempNode);
						}
					}
				}
			}
			_Tree = cashedNode;
		}


		std::vector<std::deque<bool>> EncodeItemVector(std::vector<I> list) {
			std::vector<std::deque<bool>> out;
			for (I i : list) out.push_back(_ItemBoolIndex[i]);
			return out;
		}

		std::vector<I> DecodeBoolVector(std::deque<bool> index) {
			std::vector<I> out;
			while (!index.empty()) out.push_back(_Tree.GetItem(index);
			return out;
		}

		// Bool index
		std::unordered_map<I, std::deque<bool>, Hasher> _ItemBoolIndex;

		// Tree
		Node<I> _Tree;
	};

	// OrderKey (old)
	/*
	std::string OrderKey(std::string& key, std::map<char, int> freq) {
		bool d = false;
		while (!d) {
			d = true;
			for (int i = 1; i < key.size(); i++) {
				if (freq[key[i - 1]] > freq[key[i]]) {
					std::swap(key[i - 1], key[i]);
					d = false;
				}
			}
		}
		return key;
	}
	*/


}



int main()
{
	return 0;
}


int main()
{
	return 0;
}
