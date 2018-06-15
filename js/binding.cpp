#include "duk/duktape.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
boost::asio::io_service* gio=NULL;
void testshow(std::string info) {
	std::cout << info << std::endl;
}
int test(duk_context* ctx) {
	duk_int_t len = duk_get_top(ctx);
	if (duk_is_string(ctx, 0)) {
		printf("%s\r\n", duk_get_string(ctx, 0));
		gio->post(boost::bind(testshow,duk_to_string(ctx,0)));
	}
	duk_push_string(ctx, "i love u!!!");
	return 1;
}

static const duk_function_list_entry my_module_funcs[] = {
	{ "test", test, DUK_VARARGS /*nargs*/ },
	{ NULL, NULL, 0 }
};
int passoa_init(duk_context* ctx,boost::asio::io_service* io_) {
	gio = io_;
	duk_put_function_list(ctx, -1, my_module_funcs);
	return 0;
}