#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <boost/array.hpp>

int test1(boost::shared_ptr<std::vector<char>> x) {
	const char* p1 = x->data();
	std::cout << p1 << x->size() << std::endl;
	return 0;
}
int test(boost::asio::io_service& io) {
	char x[6] = "12345";
	boost::shared_ptr<std::vector<char>> data(new std::vector<char>(x,x+6));
	for(int i=0;i<100;i++){
		io.post(boost::bind(test1, data));
	}
	return 0;
}
int main() {
	boost::asio::io_service io;
	test(io);
	io.run();
	return 0;
}