#ifndef SIMPLE_JSON_H__
#define SIMPLE_JSON_H__
#include<string>
#include <vector>
#include <map>
using std::map;
using std::vector;
using std::string;
using std::pair;
class simple_json{
public:
	static enum sim_type {SIM_NULL,SIM_BOOLEAN,SIM_NUMBER,SIM_STRING,SIM_ARRAY,SIM_OBJECT};
	static enum inf{ SIM_PARSE_OK, SIM_PARSE_EXPECE_VALUE, SIM_PARSE_INVALID_VALUE, SIM_PARSE_ROOT_NOT_SINGULAR };
private:
	struct value
	{
		vector<map<simple_json, simple_json>> object_e;
		vector<simple_json> array_e;
		double number_value;
		string boolean_value;
		string string_value;
		sim_type type;
	};
	value sim_value;
public:
	simple_json();
	int sim_parse_value(const string&);
	sim_type sim_get_parse_type();
	string sim_get_parse_boolean_value();
	double sim_get_parse_number_value();
	string sim_get_parse_string_value();
	int sim_get_parse_array_size();
	simple_json* sim_get_parse_array_e(int index);
	bool operator<(const simple_json &p) const{return false;}
	int sim_get_parse_object_size();
	pair<simple_json,simple_json> sim_get_parse_object_e(int);
private:
	void sim_parse_whitespace(char **);
	int sim_classify_parse__value(char **);
	int sim_parse_literal(char **, string, simple_json::sim_type);
	int sim_parse_number(char **);
	void sim_set_parse_type(sim_type);
	void sim_set_parse_boolean_valuse(string &);
	void sim_set_parse_number_value(double);
	int sim_parse_string(char **);
	void sim_clear();
	int sim_parse_array(char **);
	int sim_parse_object(char **);
};
class simple_json_stringify {
private:
	string result;
public:
	enum { SIM_STRINGIFY_FAIL, SIM_STRINGIFY_OK };
private:
	int sim_stringify_null();
	int sim_stringify_boolean(simple_json &);
	int sim_stringify_number(simple_json &);
	int sim_stringify_string(simple_json &);
	int sim_stringify_array(simple_json &);
	int sim_stringify_object(simple_json &);
public:
	simple_json_stringify() { result = ""; }
	void clear() { result = ""; }
	int sim_stringify_value(simple_json &);
	string & sim_get_stringify_value();
};
#endif
