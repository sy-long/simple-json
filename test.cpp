#include <iostream>
#include "simple_json.h"
using std::cout;
using std::cin;
int pass = 0;
int count = 0;
#define expect_actual( expect, actual) do{ count++;\
if (expect == actual) pass++;\
else fprintf(stderr, "%s:%d: expect: %d actual: %d \n", __FILE__, __LINE__, expect, actual);\
}while(0)\

void test_parse_null(){
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("null"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());
}

void test_parse_boolean() {
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value( "true"));
	expect_actual(simple_json::SIM_BOOLEAN, s.sim_get_parse_type());
	expect_actual("true", s.sim_get_parse_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("false"));
	expect_actual(simple_json::SIM_BOOLEAN, s.sim_get_parse_type());
	expect_actual("false", s.sim_get_parse_value());
}

static void test_parse_expect_value() {
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_EXPECE_VALUE, s.sim_parse_value(""));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_EXPECE_VALUE, s.sim_parse_value(" "));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

}

static void test_parse_invalid_value() {

	simple_json s;
	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("nul"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("?"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());
}
static void test_parse_root_not_singular() {

	simple_json s;
	expect_actual(simple_json::SIM_PARSE_ROOT_NOT_SINGULAR, s.sim_parse_value("null x"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());
}
void test_parse(){
	test_parse_null();
	test_parse_boolean();
	test_parse_expect_value();
	test_parse_invalid_value();
	test_parse_root_not_singular();
}

int main(){
	test_parse();
	cout << pass << '/' << count;
	cin.get();
}