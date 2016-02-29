#include "server_ctrl.hpp"


int main() {

	std::cout << "CPU number: " << server_ctrl::get().get_number_of_process() << std::endl;

	return 0;
}