#include "simple_json.h"
#include <cstdio>
using std::sprintf;
int simple_json_stringify::sim_stringify_null() {
	(*this).result = "null";
	return SIM_STRINGIFY_OK;
}

int simple_json_stringify::sim_stringify_boolean(simple_json &json) {
	if (json.sim_get_parse_boolean_value() == "true") (*this).result = "true";
	else (*this).result = "false";
	return SIM_STRINGIFY_OK;
}

int simple_json_stringify::sim_stringify_number(simple_json &json) {
	char buffer[32];
	int length = sprintf_s(buffer, "%.17g", json.sim_get_parse_number_value());
	(*this).result = buffer;
	return SIM_STRINGIFY_OK;
}

int simple_json_stringify::sim_stringify_string(simple_json &json) {
	string json_str = json.sim_get_parse_string_value();
	string value;
	value.push_back('\"');
	for (size_t i = 0; i < json_str.size(); i++) {
		if (json_str[i] != '\n'&&json_str[i] != '\"'&&json_str[i] != '\\'&&json_str[i] != '/'&&json_str[i] != '\b'&&json_str[i] != '\f'&&json_str[i] != '\r'&&json_str[i] != '\t')
			value.push_back(json_str[i]);
		else{
			switch (json_str[i])
			{
			case '\n': {value.push_back('\\'); value.push_back('n'); } break;
			case '\"': {value.push_back('\\'); value.push_back('\"'); } break;
			case '\\': {value.push_back('\\'); value.push_back('\\'); } break;
			case '/': {value.push_back('\\'); value.push_back('/'); } break;
			case '\b': {value.push_back('\\'); value.push_back('b'); } break;
			case '\f': {value.push_back('\\'); value.push_back('f'); } break;
			case '\r': {value.push_back('\\'); value.push_back('r'); } break;
			case '\t': {value.push_back('\\'); value.push_back('t'); } break;
			default:
				break;
			}
		}
	}
	value.push_back('\"');
	(*this).result = value;
	return SIM_STRINGIFY_OK;
}

int simple_json_stringify::sim_stringify_array(simple_json &json) {
	string value = "";
	value += '[';
	simple_json_stringify s_j_s;
	for (size_t i = 0; i < json.sim_get_parse_array_size(); i++) {
		s_j_s.sim_stringify_value(*(json.sim_get_parse_array_e(i)));
		value += s_j_s.sim_get_stringify_value();
		if (i != json.sim_get_parse_array_size() - 1) {
			value += ',';
		}
	}
	value += ']';
	(*this).result = value;
	return SIM_STRINGIFY_OK;
}




int simple_json_stringify::sim_stringify_object(simple_json &json) {
	string value = "";
	value += '{';
	simple_json t;
	simple_json_stringify s_j_s;
	for (size_t i = 0; i < json.sim_get_parse_object_size(); i++) {
		t = json.sim_get_parse_object_e(i).first;
		s_j_s.sim_stringify_value(t);
		value += s_j_s.sim_get_stringify_value();
		value += ':';
		t = json.sim_get_parse_object_e(i).second;
		s_j_s.sim_stringify_value(t);
		value += s_j_s.sim_get_stringify_value();
		if (i != json.sim_get_parse_object_size() - 1) {
			value += ',';
		}
	}
	value += '}';
	(*this).result = value;
	return SIM_STRINGIFY_OK;
}
int simple_json_stringify::sim_stringify_value(simple_json &json) {
	(*this).clear();
	switch (json.sim_get_parse_type())
	{
		case simple_json::SIM_NULL: return sim_stringify_null();
		case simple_json::SIM_BOOLEAN:return sim_stringify_boolean(json);
		case simple_json::SIM_NUMBER:return sim_stringify_number(json);
		case simple_json::SIM_STRING:return sim_stringify_string(json);
		case simple_json::SIM_ARRAY:return sim_stringify_array(json);
		case simple_json::SIM_OBJECT:return sim_stringify_object(json);
	default:
		break;
	}
}

string & simple_json_stringify::sim_get_stringify_value() {
	return result;
}