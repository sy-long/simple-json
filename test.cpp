#include <iostream>
#include "simple_json.h"
using std::cout;
using std::cin;
using std::endl;
int pass = 0;
int count = 0;
#define expect_actual( expect, actual) do{ count++;\
if (expect == actual) pass++;\
else cout<<__FILE__<<"  "<<__LINE__<<"  "<<"expect: "<<expect<<"  actual:  "<< actual<< endl;\
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
	expect_actual("true", s.sim_get_parse_boolean_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("false"));
	expect_actual(simple_json::SIM_BOOLEAN, s.sim_get_parse_type());
	expect_actual("false", s.sim_get_parse_boolean_value());
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

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("+0"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value(".123"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("INF"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("NAN"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("0123"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("0x123"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("\"\\v\""));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("[1x3,\"test\"]"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("[123,\"test\""));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("[[]"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("[1 2"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("{:1,"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("{1:1,"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("{1:1,"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value("{false:1,"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_INVALID_VALUE, s.sim_parse_value(
		"\"n\" : null , "
		"\"f\" : , "
		"\"t\" : true , "
	));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());
}
static void test_parse_root_not_singular() {

	simple_json s;
	expect_actual(simple_json::SIM_PARSE_ROOT_NOT_SINGULAR, s.sim_parse_value("null x"));
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_type());
}

#define TEST_NUMBER(expect, json)\
    do {\
        simple_json s;\
        expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value(json));\
        expect_actual(simple_json::SIM_NUMBER, s.sim_get_parse_type());\
        expect_actual(expect, s.sim_get_parse_number_value());\
    } while(0)

void test_parse_number() {
	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
	TEST_NUMBER(0.0, "1e-10000");

	TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); 
	TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); 
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");  
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");  
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");  
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}



void test_parse_string() {
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("\"\""));
	expect_actual(simple_json::SIM_STRING, s.sim_get_parse_type());
	expect_actual("", s.sim_get_parse_string_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("\"Hello\""));
	expect_actual(simple_json::SIM_STRING, s.sim_get_parse_type());
	expect_actual("Hello", s.sim_get_parse_string_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("\"Hello\\nWorld\""));
	expect_actual(simple_json::SIM_STRING, s.sim_get_parse_type());
	expect_actual("Hello\nWorld", s.sim_get_parse_string_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\""));
	expect_actual(simple_json::SIM_STRING, s.sim_get_parse_type());
	expect_actual("\" \\ / \b \f \n \r \t", s.sim_get_parse_string_value());

}

void test_parse_array() {
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[123,\"test\"]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(2, s.sim_get_parse_array_size());
	expect_actual(123, s.sim_get_parse_array_e(0)->sim_get_parse_number_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[123,[123,456],\"test\"]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(3, s.sim_get_parse_array_size());
	expect_actual("test", s.sim_get_parse_array_e(2)->sim_get_parse_string_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[123,[789,[456]]]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(2, s.sim_get_parse_array_size());
	expect_actual(789, s.sim_get_parse_array_e(1)->sim_get_parse_array_e(0)->sim_get_parse_number_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[   123    ,   [789,[456]]]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(2, s.sim_get_parse_array_size());
	expect_actual(789, s.sim_get_parse_array_e(1)->sim_get_parse_array_e(0)->sim_get_parse_number_value());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[ null , false , true , 123 , \"abc\" ]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(5, s.sim_get_parse_array_size());
	expect_actual(simple_json::SIM_NULL, s.sim_get_parse_array_e(0)->sim_get_parse_type());

	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]"));
	expect_actual(simple_json::SIM_ARRAY, s.sim_get_parse_type());
	expect_actual(4, s.sim_get_parse_array_size());
	expect_actual(0.0, s.sim_get_parse_array_e(1)->sim_get_parse_array_e(0)->sim_get_parse_number_value());

}

void test_parse_object(){
	simple_json s;
	expect_actual(simple_json::SIM_PARSE_OK, s.sim_parse_value(
		" { "
		"\"n\" : null , "
		"\"f\" : false , "
		"\"t\" : true , "
		"\"i\" : 123 , "
		"\"s\" : \"abc\" , "
		"\"a\" : [ 1, 2, 3 ],"
		"\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
		" } "
	));
	expect_actual(simple_json::SIM_OBJECT, s.sim_get_parse_type());
	expect_actual(7, s.sim_get_parse_object_size());
	simple_json t;
	t.sim_parse_value("\"i\"");
	expect_actual(123, s.sim_get_parse_object_e(3)[t].sim_get_parse_number_value());
}

void test_parse(){
	test_parse_null();
	test_parse_boolean();
	test_parse_expect_value();
	test_parse_invalid_value();
	test_parse_root_not_singular();
	test_parse_number();
	test_parse_array();
	test_parse_string();
	test_parse_object();
}

int main(){
	test_parse();
	cout << pass << '/' << count;
	cin.get();
}