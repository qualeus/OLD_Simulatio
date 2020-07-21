#include <iostream>

int main()
{
	std::vector<std::shared_ptr<phy::Corpse>> vect = std::vector<std::shared_ptr<phy::Corpse>>();
	vect.emplace_back(std::move(std::make_shared<phy::Circle>(phy::Circle())));
	vect.emplace_back(std::move(std::make_shared<phy::Circle>(phy::Circle())));
	vect.emplace_back(std::move(std::make_shared<phy::Circle>(phy::Circle())));

	for (int i=0; i < vect.size(); i++) { std::cout << vect.at(i)->get_id() << std::endl; } // 0 1 2

	std::cout <<"remove: " << vtr::remove(1, vect)->get_id() << std::endl; // remove: 1

	for (int i=0; i < vect.size(); i++) { std::cout << vect.at(i)->get_id() << std::endl; } // 0 2
}