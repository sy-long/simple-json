#include "simple_json.h"
#include <sstream>
using std::istringstream;

simple_json::simple_json(){
	sim_value.type = SIM_NULL;
	sim_value.boolean_value = "false";
	sim_value.number_value = 0.0;
	sim_value.string_value = "";
	sim_value.array_e.clear();
	sim_value.object_e.clear();
}

void simple_json::sim_clear() {
	sim_value.type = SIM_NULL;
	sim_value.boolean_value = "false";
	sim_value.number_value = 0.0;
	sim_value.string_value = "";
	sim_value.array_e.clear();
	sim_value.object_e.clear();
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

int simple_json::sim_parse_array(char **handle) {
	(*handle)++;
	sim_parse_whitespace(handle);
	if ((**handle) == ']'){
		(*handle)++;
		sim_set_parse_type(simple_json::SIM_ARRAY);
		return SIM_PARSE_OK;
	}
	string t_str;
	int state = 0;
	int count = 0;
	while ((**handle) != '\0' && (**handle) != ']') {
		for (; (**handle) == ',';) (*handle)++;
		sim_parse_whitespace(handle);
		if ((**handle) == '[') state = 1;
		for (;(**handle != '\0'); (*handle)++) {
			if (**handle == '[') count++;
			if (**handle == ',')
				if (state != 1)
					break;
			if (**handle == ']')
				if (state != 1)
					break;
			if (**handle == ']'&&count != 0) count--;
			if (**handle == ']'&&count == 0) state = 0;
			t_str.push_back((**handle));
		}
		if((**handle)=='\0') return SIM_PARSE_INVALID_VALUE;
		while (t_str[t_str.size() - 1] == ' ') t_str.pop_back();
		simple_json s;
		int ret;
		if ((ret = s.sim_parse_value(t_str)) != SIM_PARSE_OK)
			return ret;
		sim_value.array_e.push_back(s);
		t_str = "";
		state = 0;
		count = 0;
	}
	if (**handle != ']') return SIM_PARSE_INVALID_VALUE;
	(*handle)++;
	sim_set_parse_type(simple_json::SIM_ARRAY);
	return SIM_PARSE_OK;
}

int simple_json::sim_parse_object(char **handle) {
	(*handle)++;
	sim_parse_whitespace(handle);
	if ((**handle) == '}') {
		(*handle)++;
		sim_set_parse_type(simple_json::SIM_OBJECT);
		return SIM_PARSE_OK;
	}
	string t_str_k="";
	string t_str_v="";
	simple_json k;
	simple_json v;
	int count = 0;
	int state = 0;
	int count_arr = 0;
	int state_arr = 0;
	map<simple_json, simple_json> t_value;
	int ret;
	while ((**handle) != '}'&&(**handle)!='\0') {

		while ((**handle) != ':' && (**handle) != '\0') {
			t_str_k.push_back(**handle);
			(*handle)++;
		}
		if((**handle)=='\0') return simple_json::SIM_PARSE_INVALID_VALUE;
		if (t_str_k.size() == 0) return simple_json::SIM_PARSE_INVALID_VALUE;
		while (t_str_k[t_str_k.size() - 1] == ' ') t_str_k.pop_back();
		if (t_str_k.size() == 0) return simple_json::SIM_PARSE_INVALID_VALUE;
		if ((ret = k.sim_parse_value(t_str_k)) != simple_json::SIM_PARSE_OK) return simple_json::SIM_PARSE_INVALID_VALUE;
		if (k.sim_get_parse_type() != simple_json::SIM_STRING) return simple_json::SIM_PARSE_INVALID_VALUE;
		(*handle)++;

		sim_parse_whitespace(handle);
		if ((**handle) == '{')  state = 1;
		if ((**handle) == '[') 
			state_arr = 1;
		while ((**handle) != '\0') {

			if ((**handle) == '{') count++;
			if ((**handle) == '[') count_arr++;

			if ((**handle) == ',')
				if (state == 0)
					if (state_arr == 0)
						break;
			if ((**handle) == '}')
				if (state == 0)
					if (state_arr == 0)
						if (state_arr == 0)
					break;

			if (**handle == '}'&&count != 0) count--;
			if (**handle == '}'&&count == 0) state = 0;

			if (**handle == ']'&&count_arr != 0) count_arr--;
			if (**handle == ']'&&count_arr == 0) state_arr = 0;

			t_str_v.push_back(**handle);
			(*handle)++;
		}
		if (t_str_v.size() == 0) return simple_json::SIM_PARSE_INVALID_VALUE;
		if ((**handle) == '\0') return simple_json::SIM_PARSE_INVALID_VALUE;
		while (t_str_v[t_str_v.size() - 1] == ' ') t_str_v.pop_back();

		if (t_str_v.size() == 0) return simple_json::SIM_PARSE_INVALID_VALUE;

		if ((ret = v.sim_parse_value(t_str_v)) != simple_json::SIM_PARSE_OK) return simple_json::SIM_PARSE_INVALID_VALUE;
		t_value.insert(pair<simple_json, simple_json>(k, v));
		sim_value.object_e.push_back(t_value);
		if((**handle)==',')(*handle)++;
		t_str_k = "";
		t_str_v = "";
		int count = 0;
		int state = 0;
		t_value.clear();
	}
	if ((**handle) == '}') (*handle)++;
	sim_set_parse_type(simple_json::SIM_OBJECT);
	return SIM_PARSE_OK;
}


int simple_json::sim_classify_parse__value(char **handle) {
	switch (**handle) {
		case 'n':return sim_parse_literal(handle, "null", simple_json::SIM_NULL);
		case 't':return sim_parse_literal(handle, "true", simple_json::SIM_BOOLEAN);
		case 'f':return sim_parse_literal(handle, "false", simple_json::SIM_BOOLEAN);
		case '[':return sim_parse_array(handle);
		case '\"':return sim_parse_string(handle);
		case '{':return sim_parse_object(handle);
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
int simple_json::sim_get_parse_array_size() {
	if ((*this).sim_value.type == simple_json::SIM_ARRAY)
		return (*this).sim_value.array_e.size();
	else return 0;
}
simple_json* simple_json::sim_get_parse_array_e(int index) {
	if ((*this).sim_value.type == simple_json::SIM_ARRAY&&index< (*this).sim_value.array_e.size())
		return &(*this).sim_value.array_e[index];
	else return NULL;
}
int simple_json::sim_get_parse_object_size() {
	if ((*this).sim_value.type == simple_json::SIM_OBJECT)
		return (*this).sim_value.object_e.size();
	else return 0;
}
pair<simple_json, simple_json> simple_json::sim_get_parse_object_e(int index) {
	map<simple_json, simple_json>::iterator iter = (*this).sim_value.object_e[index].begin();
	return *iter;
}