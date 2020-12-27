#include <iostream>
#include "../../include/Functional.hpp"

template <typename TA, typename TB>
void functionnal_Message(const TA& test_result, const TB& true_result) {
	std::cout << "[Functional] " << ftn::to_string(test_result) << " == "<< ftn::to_string(true_result) << std::endl;
}

template <typename TA, typename TB>
void error_Message(const TA& test_result, const TB& true_result) {
	std::string delimiter = "\n[-------------------------------[Error]-------------------------------]\n";
	std::cout << delimiter << "Computation Error: " << ftn::to_string(test_result) << " != "<< ftn::to_string(true_result) << delimiter << std::endl;
}

template <typename TA, typename TB>
void assert_Equals(const TA& test_result, const TB& true_result) {
	if (test_result == true_result) {
		functionnal_Message(test_result, true_result);
	} else {
		error_Message(test_result, true_result);
	}
}

void assert_Equals(const float& test_result, const float& true_result) {
	if (ftn::decimal_equals(test_result, true_result, 0.05f)) {
		functionnal_Message(test_result, true_result);
	} else {
		error_Message(test_result, true_result);
	}
}

void assert_Equals(const sf::Vector2f& test_result, const sf::Vector2f& true_result) {
	if (ftn::decimal_equals(test_result.x, true_result.x, 0.05f) && ftn::decimal_equals(test_result.y, true_result.y, 0.05f)) {
		functionnal_Message(test_result, true_result);
	} else {
		error_Message(test_result, true_result);
	}
}

/* Test the ftn::Dot(vect2f, vect2f) function */
void test_Dot_vect() {
	std::cout << "\nTesting Dot_vect: [test_value == true_value ?]" << std::endl;

	std::cout << "# Dot_vect empty: ";
	sf::Vector2f vect_a = sf::Vector2f();
	sf::Vector2f vect_b = sf::Vector2f();
	float test_result = ftn::Dot(vect_a, vect_b);
	float true_result = 0.0f;
	assert_Equals(test_result, true_result);

	std::cout << "# Dot_vect integers: ";
	vect_a = sf::Vector2f(15, 7);
	vect_b = sf::Vector2f(4, 78);
	test_result = ftn::Dot(vect_a, vect_b);
	true_result = 606.0f;
	assert_Equals(test_result, true_result);

	std::cout << "# Dot_vect floats: ";
	vect_a = sf::Vector2f(84.26f, 14.1101f);
	vect_b = sf::Vector2f(878.26f, 1105.1f);
	test_result = ftn::Dot(vect_a, vect_b);
	true_result = 89595.3f;
	assert_Equals(test_result, true_result);

	std::cout << "# Dot_vect negative: ";
	vect_a = sf::Vector2f(-15.12, -45);
	vect_b = sf::Vector2f(-12, 0);
	test_result = ftn::Dot(vect_a, vect_b);
	true_result = 181.44f;
	assert_Equals(test_result, true_result);
}

/* Test the ftn::remove(int, vect) function */
void test_Remove() {
	std::cout << "\nTesting remove from vector: [test_value == true_value ?]" << std::endl;
	
	std::cout << "# Remove pos {2} from int vector {0,1,2,3,4}: ";
	std::vector<std::shared_ptr<int>> vect = std::vector<std::shared_ptr<int>>();
	for(int i=0; i<5; i++) { vect.emplace_back(std::move(std::make_shared<int>(i))); }
	ftn::remove(2, vect);
	std::vector<int> test_result{};
	for (int i=0; i < vect.size(); i++) { test_result.push_back((int) *vect.at(i).get()); }
	std::vector<int> true_result{0,1,3,4};
	assert_Equals(test_result, true_result);

	std::cout << "# Remove pos {0,1,2,3} from int vector {0,1,2,3,4}: ";
	vect = std::vector<std::shared_ptr<int>>();
	for(int i=0; i<5; i++) { vect.emplace_back(std::move(std::make_shared<int>(i))); }
	for(int i=0; i<4; i++) { ftn::remove(0, vect); }

	test_result = {};
	for (int i=0; i < vect.size(); i++) { test_result.push_back((int) *vect.at(i).get()); }
	
	true_result = {4};
	assert_Equals(test_result, true_result);

	std::cout << "# Remove pos {1,3} from int vector {0,1,2,3,4}: ";
	vect = std::vector<std::shared_ptr<int>>();
	for(int i=0; i<5; i++) { vect.emplace_back(std::move(std::make_shared<int>(i))); }
	ftn::remove(1, vect); // remove pos 1 => {0,2,3,4}
	ftn::remove(3, vect); // remove pos 3 => {0,2,3}

	test_result = {};
	for (int i=0; i < vect.size(); i++) { test_result.push_back((int) *vect.at(i).get()); }
	
	true_result = {0,2,3};
	assert_Equals(test_result, true_result);
}

void test_Segment_Projection() {

	std::cout << "\nTesting Point projection on a Line: [test_value == true_value ?]" << std::endl;
	
	std::cout << "# Project point [5,3] on a line [0,0] [4,0]: ";
	sf::Vector2f test_result = ftn::Segment_Projection({0,0}, {4,0}, {5,3});
	sf::Vector2f true_result = {5, 0};
	assert_Equals(test_result, true_result);

	std::cout << "# Project point [452,-691] on a line [49,-727] [375,-274]: ";
	test_result = ftn::Segment_Projection({49,-727}, {375,-274}, {452,-691});
	true_result = {203.568,-512.217};
	assert_Equals(test_result, true_result);
}

int main() {
	std::cout << "unit test launching..." << std::endl;

	test_Dot_vect();
	test_Remove();
	test_Segment_Projection();

	system("pause");
}

