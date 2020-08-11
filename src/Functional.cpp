#include "../include/Functional.hpp"

/* Dot product: [Ax Ay].[Bx By] = (Ax)(Bx)+(Ay)(By) = Amag*Bmag*cos(theta) */
float ftn::Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b) { return vect_a.x*vect_b.x + vect_a.y*vect_b.y; }
float ftn::Dot(float x1, float y1, float x2, float y2) { return x1*x2 + y1*y2; }

/* Perp/Cross Dot product: [Ax Ay].[Bx By] = (Ax)(Bx)-(Ay)(By) = Amag*Bmag*sin(theta) */
float ftn::Perp_Dot(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b) { return vect_a.x*vect_b.x - vect_a.y*vect_b.y; }
float ftn::Perp_Dot(float x1, float y1, float x2, float y2) { return x1*x2 - y1*y2; }

/* Vector length: c^2=a^2+b^2 <=> c = sqrt(a*a+b*b) */
float ftn::Length(const sf::Vector2f &vect) { return std::sqrt(Dot(vect, vect)); }
float ftn::Length(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B) { return std::sqrt(Dot(vect_B-vect_A, vect_B-vect_A)); }
float ftn::Length(float x, float y) { return std::sqrt(Dot(x, y, x, y)); }
float ftn::Length(float x1, float y1, float x2, float y2) { return std::sqrt(Dot(x1-x2, y1-y2, x1-x2, y1-y2)); }

/* Convert deg rad: (PI/2) * (180/PI) = 90 */
float ftn::degree_to_radian(float degree) { return (degree / 180) * PI; }
float ftn::radian_to_degree(float radian) { return (radian / PI) * 180; }

/* Normalize vect = [Ax Ay].(1/size) = [Ax/size Ay/size] */
sf::Vector2f ftn::Normalize(const sf::Vector2f &vect) {
	sf::Vector2f temp_vect = {vect.x, vect.y};
	float length = ftn::Length(temp_vect);
	if (length != 0 ) { temp_vect /= length; }
	return temp_vect;
}
/* Return the Norme = [Ay - By, Bx - Ax] */
sf::Vector2f ftn::Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B) {
	return sf::Vector2f(vect_A.y - vect_B.y, vect_B.x - vect_A.x);
}

/* Return the inverse Norme = [Ay - By, Bx - Ax] */
sf::Vector2f ftn::inverse_Norme(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B) {
	return sf::Vector2f(vect_B.y - vect_A.y, vect_A.x - vect_B.x);
}

/* Rotate vector: [Ax Ay].scale = [Ax*scale Ay*scale] */
void ftn::Rotate(sf::Vector2f &vect, int angle) { 
	float theta = degree_to_radian(angle);
	float cos_theta = std::cos(theta);
	float sin_theta = std::sin(theta);
	vect = sf::Vector2f(vect.x * cos_theta - vect.y * sin_theta, vect.x * sin_theta + vect.y * cos_theta); 
}
/* Scale vector: [Ax Ay].scale = [Ax*scale Ay*scale] */
void ftn::Scale(sf::Vector2f &vect, float scale) { vect=sf::Vector2f(vect.x * scale, vect.y * scale); }

/* Search if the lines AB and CD intersect */
bool ftn::Lines_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D) {
	float ABx = vect_A.x - vect_B.x;
	float ABy = vect_B.y - vect_A.y;
	float AB = ABy*vect_A.x + ABx*vect_A.y;

	float CDx = vect_C.x - vect_D.x;
	float CDy = vect_D.y - vect_C.y;
	float CD = CDy*vect_C.x + CDx*vect_C.y;

	float det = ABy*CDx - CDy*ABx;
	
	// Parallel Lines
	if (det==0) { return false; } 
	return true;
}

/* Given the collinears points A,B and C, check if the point C lies on the line segment [AB] */
bool ftn::on_segment(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C) {
    if (vect_C.x > std::max(vect_A.x, vect_B.x)) { return false; }
    if (vect_C.x < std::min(vect_A.x, vect_B.x)) { return false; }
    if (vect_C.y > std::max(vect_A.y, vect_B.y)) { return false; }
    if (vect_C.y < std::min(vect_A.y, vect_B.y)) { return false; }

    return true; 
}

/* Find the orientation of the ordered triplet (A, B, C) */
/*
	return 0 => A,B and C Colinear: [AB] // [BC]
	return 1 => Clockwise Orientation: [BA] => [AC] => [CB]
	return 2 => Counter Clockwise: [AB] => [BC] => [CA]
*/
int ftn::line_orientation(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C) {
	/* 
		Using the segments slope:
		- if the two slopes are equals, the segments are colinears
		- if slope[AB] > slope[BC], the orientation is clockwise
		- if slope[AB] < slope[BC], the orientation is counter clockwise
	*/
    int sigma = static_cast<int>((vect_B.y - vect_A.y) * (vect_C.x - vect_B.x) - (vect_B.x - vect_A.x) * (vect_C.y - vect_B.y));
    if (sigma == 0) { return 0; }
    if (sigma > 0) { return 1; }
    return 2;
}

/* Search if the segments AB and CD intersect */
bool ftn::Segments_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D) {
	
	int orientationA = ftn::line_orientation(vect_A, vect_B, vect_C);
	int orientationB = ftn::line_orientation(vect_A, vect_B, vect_D);
	int orientationC = ftn::line_orientation(vect_C, vect_D, vect_A);
	int orientationD = ftn::line_orientation(vect_C, vect_D, vect_B);

	// General case 
    if (orientationA != orientationB && orientationC != orientationD) { return true; }
    if (orientationA == 0 && ftn::on_segment(vect_A, vect_B, vect_C)) { return true; }
    if (orientationB == 0 && ftn::on_segment(vect_A, vect_B, vect_D)) { return true; } 
    if (orientationC == 0 && ftn::on_segment(vect_C, vect_D, vect_A)) { return true; } 
    if (orientationD == 0 && ftn::on_segment(vect_C, vect_D, vect_B)) { return true; } 
    return false;
}

/* NOT WORKING YET */
bool ftn::Test_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const sf::Vector2f &vect_D) {
	float ABx = vect_A.x - vect_B.x;
	float ABy = vect_B.y - vect_A.y;
	float AB = ABy*vect_A.x + ABx*vect_A.y;

	float CDx = vect_C.x - vect_D.x;
	float CDy = vect_D.y - vect_C.y;
	float CD = CDy*vect_C.x + CDx*vect_C.y;

	float det = ABy*CDx - CDy*ABx;
	
	// Parallel Lines
	if (det==0) { return false; } 
	
	float intersect_x = (CDx*AB - ABx*CD)/det;
    float intersect_y = (ABy*CD - CDy*AB)/det;

    // Check that the segments overlap one other (X/Y overlap)
    if (std::max(vect_A.x, vect_B.x) < std::min(vect_C.x, vect_D.x) || std::min(vect_A.x, vect_B.x) > std::max(vect_C.x, vect_D.x)) { return false; }
    if (std::max(vect_A.y, vect_B.y) < std::min(vect_C.y, vect_D.y) || std::min(vect_A.y, vect_B.y) > std::max(vect_C.y, vect_D.y)) { return false; }

    //Check that the point is on bounds AB (X/Y in bounds)
    if (intersect_x < std::min(vect_A.x, vect_B.x) || intersect_x > std::max(vect_A.x, vect_B.x)) { return false; }
    if (intersect_y < std::min(vect_A.y, vect_B.y) || intersect_y > std::max(vect_A.y, vect_B.y)) { return false; }

    //Check that the point is on bounds CD (X/Y in bounds)
    if (intersect_x < std::min(vect_C.x, vect_D.x) || intersect_x > std::max(vect_C.x, vect_D.x)) { return false; }
    if (intersect_y < std::min(vect_C.y, vect_D.y) || intersect_y > std::max(vect_C.y, vect_D.y)) { return false; }

	return true;
}


/* Project point A onto the line B */
sf::Vector2f ftn::Line_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b) {
	float dp = ftn::Dot(vect_a, vect_b); // dot product of A and B
	float bd = ftn::Dot(vect_b, vect_b); // length of B squared
	if (bd == 0) { return sf::Vector2f(); }
	return vect_b*(dp/bd); // proj = [(dp/bd)*vect_b.x, (dp/bd)*vect_b.y]
}

/* Projected point C onto the line AB */
sf::Vector2f ftn::Segment_Projection(const sf::Vector2f &vect_a, const sf::Vector2f &vect_b, const sf::Vector2f &vect_c) {
	sf::Vector2f vect_ab = vect_b-vect_a; //v2
	sf::Vector2f vect_ac = vect_c-vect_a; //v1

	float dp = Dot(vect_ab, vect_ac); // dot product of A and B
	float bd = Dot(vect_ab, vect_ab); // length of B squared
	if (bd == 0) { return sf::Vector2f(); }
	return vect_a + (dp/bd)*vect_ab; // proj = [(dp/bd)*vect_b.x, (dp/bd)*vect_b.y]
}

/* Compute the centroid of the polygon with its edges */
/*
	X = SUM[(Xi + Xi+1) * (Xi * Yi+1 - Xi+1 * Yi)] / 6 / A
	Y = SUM[(Yi + Yi+1) * (Xi * Yi+1 - Xi+1 * Yi)] / 6 / A
*/
sf::Vector2f ftn::Centroid(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> edges) {
	sf::Vector2f centroid = sf::Vector2f();
	float signed_area = 0.0f;
	float A = 0.0f;

	for (int i=0; i<edges.size(); i++) {
		sf::Vector2f pA = edges.at(i).first;
		sf::Vector2f pB = edges.at(i).second;
		A = pA.x*pB.y - pB.x*pA.y;
		signed_area += A;
		centroid += (pA+pB)*A;
	}

	if (signed_area == 0) { return sf::Vector2f(); }
	signed_area*=0.5f;
	centroid /= (6.0f*signed_area);
	return centroid;
}

/* Compute the averages of the points */
sf::Vector2f ftn::Points_Average(std::vector<sf::Vector2f> points) {
	sf::Vector2f points_average = sf::Vector2f();
	if (points.size() == 0) { return points_average; }

	for (int i=0; i<points.size(); i++) { points_average = points_average + points.at(i); }
	return points_average / (float)points.size();
}

/* Test if the line segment [AB] intersect with the circle of center C and of radius size. Return true if collide and the collision point */
std::pair<bool, sf::Vector2f> ftn::Line_Circle_Intersect(const sf::Vector2f &vect_A, const sf::Vector2f &vect_B, const sf::Vector2f &vect_C, const float &size) {
	
	sf::Vector2f closest = ftn::Segment_Projection(vect_A, vect_B, vect_C);

	// Check if one of the ends of the line segment (side) is inside the circle
	if (ftn::Length(vect_A,vect_C) <= size) { return {true, closest}; }
	if (ftn::Length(vect_B,vect_C) <= size) { return {true, closest}; }

	// Check if the closest point on the line is inside the circle
	/*
	float side_len = ftn::Length(vect_A, vect_B);
	float dot = ftn::Dot(vect_C-vect_A, vect_B-vect_A) / (side_len*side_len);
	sf::Vector2f closest = vect_A + dot * (vect_B-vect_A);
	*/

	if (!ftn::on_segment(vect_A, vect_B, closest)) { return {false, sf::Vector2f()}; }
	if (ftn::Length(closest,vect_C) <= size) { return {true, closest}; }

	return {false, sf::Vector2f()};
}

/* Return the float with a designed number of digits behind the comma */
float ftn::digits_comma(float number, int digits) { 
	int d_limit = std::pow(10, digits);
	return round(number * d_limit)/d_limit; 
}

/* Angle from a line: atan2(y, x) = Arg(x+iy) rad <=> atan2(y, x)*(180/3.1415)= Arg(x+iy) deg */
float ftn::bearing(float x1, float y1, float x2, float y2) { return atan2(y1 - y2, x1 - x2) * (180 / PI); }

bool ftn::rect_in_bounds(const ftn::Rectangle &object, const ftn::Rectangle &limits) {
	// One point in bounds
	// p1 (x,y)
	if (object.pos.x > limits.pos.x && object.pos.x < limits.pos.x + limits.size.x && object.pos.y > limits.pos.y && object.pos.y < limits.pos.y + limits.size.y) { return true; }
	// p2 (x+w,y)
	if (object.pos.x + object.size.x > limits.pos.x && object.pos.x + object.size.x < limits.pos.x + limits.size.x && object.pos.y > limits.pos.y && object.pos.y < limits.pos.y + limits.size.y) { return true; }
	// p3 (x,y+h)
	if (object.pos.x > limits.pos.x && object.pos.x < limits.pos.x + limits.size.x && object.pos.y + object.size.y > limits.pos.y && object.pos.y + object.size.y <  limits.pos.y + limits.size.y) { return true; }
	// p4 (x+w,y+h)
	if (object.pos.x + object.size.x > limits.pos.x && object.pos.x + object.size.x < limits.pos.x + limits.size.x  && object.pos.y + object.size.y > limits.pos.y && object.pos.y + object.size.y <  limits.pos.y + limits.size.y) { return true; }

	return false;
}

bool ftn::rect_out_bounds(const ftn::Rectangle &object, const ftn::Rectangle &limits) {
	// One face out bounds
	// f1
	if (object.pos.x < limits.pos.x) { return true; }
	// f2
	if (object.pos.x + object.size.x > limits.pos.x + limits.size.x) { return true; }
	// f3
	if (object.pos.y < limits.pos.y) { return true; }
	// f4 
	if (object.pos.y + object.size.y > limits.pos.y + limits.size.y) { return true; }

	return false;
}

