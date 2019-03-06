#ifndef SIMPLE_JSON_H__
#define SIMPLE_JSON_H__
#include<string>
using std::string;
class simple_json
{
public:
	static enum sim_type {SIM_NULL,SIM_BOOLEAN,SIM_NUMBER,SIM_STRING,SIM_ARRAY,SIM_OBJECT};
	static enum inf{ SIM_PARSE_OK, SIM_PARSE_EXPECE_VALUE, SIM_PARSE_INVALID_VALUE, SIM_PARSE_ROOT_NOT_SINGULAR };
private:
	struct
	{
		string boolean_value;
		sim_type type;
	}sim_value;
public:
	simple_json();
	int sim_parse_value(const string&);
	sim_type sim_get_parse_type();
	string sim_get_parse_value();
private:
	void sim_parse_whitespace(char **);
	int sim_classify_parse__value(const char *);
	int sim_parse_literal(const char *, string, simple_json::sim_type);
	void sim_set_parse_type(sim_type);
	void sim_set_parse_boolean_valuse(string &);
	void sim_clear();
};

#endif
