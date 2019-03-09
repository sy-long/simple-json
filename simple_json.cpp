#include "simple_json.h"
#include <sstream>
using std::istringstream;

simple_json::simple_json(){
	sim_value.type = SIM_NULL;
	sim_value.boolean_value = "false";
	sim_value.number_value = 0.0;
	sim_value.string_value = "";
}

void simple_json::sim_clear() {
	simple_json();
}

void simple_json::sim_parse_whitespace(char **handle) {
	while (**handle == ' ' || **handle == '\t' || **handle == '\n' || **handle == '\r')
		(*handle)++;
}

void simple_json::sim_set_parse_type(simple_json::sim_type type) {
	sim_value.type = type;
}
void simple_json::sim_set_parse_boolean_valuse(string &boolean_value){
	sim_value.boolean_value = boolean_value;
}
void simple_json::sim_set_parse_number_value(double value) {
	sim_value.number_value = value;
}

int simple_json::sim_parse_literal(char **handle, string literal, simple_json::sim_type type) {
	for (size_t i = 0; i < literal.size(); i++) {
		if ((*handle)[i] != literal[i])
			return simple_json::SIM_PARSE_INVALID_VALUE;
	}
	(*handle) += literal.size();
	(*this).sim_set_parse_type(type);
	if (type == simple_json::SIM_BOOLEAN) (*this).sim_set_parse_boolean_valuse(literal);
	return simple_json::SIM_PARSE_OK;
}

int simple_json::sim_parse_number(char **handle) {
	char *p = *handle;
	if ((**handle) == '-') (*handle)++;
	if ((**handle) == '0')(*handle)++;
	else{
		if (!((**handle) >= '1' && (**handle) <= '9')) return simple_json::SIM_PARSE_INVALID_VALUE;
		for ((*handle)++; ((**handle) >= '0' && (**handle) <= '9'); (*handle)++);
	}
	if ((**handle) == '.') {
		(*handle)++;
		if (!((**handle) >= '0' && (**handle) <= '9')) return simple_json::SIM_PARSE_INVALID_VALUE;
		for ((*handle)++; ((**handle) >= '0' && (**handle) <= '9'); (*handle)++) { int count=0; count++; }
	}
	if (**handle == 'e' || **handle == 'E') {
		(*handle)++;
		if (**handle == '+' || **handle == '-') (*handle)++;
		if (!((**handle) >= '0' && (**handle) <= '9')) return simple_json::SIM_PARSE_INVALID_VALUE;
		for ((*handle)++; ((**handle) >= '0' && (**handle) <= '9'); (*handle)++);
	}
	if((**handle)!=' '&&(**handle)!='\0') return simple_json::SIM_PARSE_INVALID_VALUE;
	string tjson = p;
	istringstream str(tjson);
	(*this).sim_set_parse_type(simple_json::SIM_NUMBER);
	double value;
	str >> value;
	(*this).sim_set_parse_number_value(value);
	return simple_json::SIM_PARSE_OK;
}

int simple_json::sim_parse_string(char **handle) {
	(*handle)++;
	string t_str = "";
	while ((**handle) != '\0') {
		if ((**handle) != '\\') t_str += (**handle);
		else {
			switch (*(++(*handle)))
			{
			case '\"':t_str += '\"'; break;
			case '\\':t_str += '\\'; break;
			case '/':t_str += '/'; break;
			case 'b':t_str += '\b'; break;
			case 'f':t_str += '\f'; break;
			case 'n':t_str += '\n'; break;
			case 'r':t_str += '\r'; break;
			case 't':t_str += '\t'; break;
			default:
				return SIM_PARSE_INVALID_VALUE;
				break;
			}
		}
		if((**handle)<0x20) return SIM_PARSE_INVALID_VALUE;
		(*handle)++;
	}
	size_t len = t_str.size() - 1;
	if(t_str[len]!='\"') return SIM_PARSE_INVALID_VALUE;
	t_str.pop_back();
	(*this).sim_value.type = simple_json::SIM_STRING;
	(*this).sim_value.string_value = t_str;
	return simple_json::SIM_PARSE_OK;
}

int simple_json::sim_classify_parse__value(char **handle) {
	switch (**handle) {
		case 'n':return sim_parse_literal(handle, "null", simple_json::SIM_NULL);
		case 't':return sim_parse_literal(handle, "true", simple_json::SIM_BOOLEAN);
		case 'f':return sim_parse_literal(handle, "false", simple_json::SIM_BOOLEAN);
		case '\"':return sim_parse_string(handle);
		default:return sim_parse_number(handle);
	}
}

int simple_json::sim_parse_value(const string &json){
	sim_clear();
	int ret;
	string tjson = json;
	char* handle = &tjson[0];
	sim_parse_whitespace(&handle);
	if (handle==&tjson[json.size()]) return simple_json::SIM_PARSE_EXPECE_VALUE;
	if ((ret=sim_classify_parse__value(&handle)) == simple_json::SIM_PARSE_OK){
		sim_parse_whitespace(&handle);
		if (handle != &tjson[json.size()]) {
			(*this).sim_set_parse_type(SIM_NULL);
			ret = simple_json::SIM_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	return ret;
}
simple_json::sim_type simple_json::sim_get_parse_type() {
	return (*this).sim_value.type;
}
string simple_json::sim_get_parse_boolean_value() {
	if((*this).sim_value.type==simple_json::SIM_BOOLEAN)
		return (*this).sim_value.boolean_value;
	else return "false";
}
double simple_json::sim_get_parse_number_value() {
	if ((*this).sim_value.type == simple_json::SIM_NUMBER)
		return (*this).sim_value.number_value;
	else return 0.0;
}
string simple_json::sim_get_parse_string_value() {
	if ((*this).sim_value.type == simple_json::SIM_STRING)
		return (*this).sim_value.string_value;
	else return "";
}