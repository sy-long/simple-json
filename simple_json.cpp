#include "simple_json.h"

simple_json::simple_json(){
	sim_value.type = SIM_NULL;
	sim_value.boolean_value = "false";
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

int simple_json::sim_parse_literal(const char *handle, string literal, simple_json::sim_type type) {
	for (size_t i = 0; i < literal.size(); i++) {
		if (handle[i] != literal[i])
			return simple_json::SIM_PARSE_INVALID_VALUE;
	}
	handle += literal.size();
	(*this).sim_set_parse_type(type);
	if (type == simple_json::SIM_BOOLEAN) (*this).sim_set_parse_boolean_valuse(literal);
	return simple_json::SIM_PARSE_OK;
}

int simple_json::sim_classify_parse__value(const char *handle) {
	switch (*handle) {
		case 'n':return sim_parse_literal(handle, "null", simple_json::SIM_NULL);
		case 't':return sim_parse_literal(handle, "true", simple_json::SIM_BOOLEAN);
		case 'f':return sim_parse_literal(handle, "false", simple_json::SIM_BOOLEAN);
		default:return simple_json::SIM_PARSE_INVALID_VALUE;
	}
}

int simple_json::sim_parse_value(const string &json){
	sim_clear();
	string tjson = json;
	char* handle = &tjson[0];
	sim_parse_whitespace(&handle);
	if (handle==&tjson[json.size()]) return simple_json::SIM_PARSE_EXPECE_VALUE;
	return sim_classify_parse__value(handle);
	
}
simple_json::sim_type simple_json::sim_get_parse_type() {
	return (*this).sim_value.type;
}
string simple_json::sim_get_parse_value() {
	if((*this).sim_value.type==simple_json::SIM_BOOLEAN)
		return (*this).sim_value.boolean_value;
}