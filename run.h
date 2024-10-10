#ifndef __PARSER_H
#define __PARSER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <algorithm>

using namespace std;

using ParseResult = bool;

class Token {
public:
  Token() {}
  Token(const std::string& str):literal(str){}
  std::string literal;
};

class Parser {
public:
  void tokenize(const string& input);
  bool expect(const std::string& literal);
  ParseResult parse_command();
  ParseResult parse_lighting_command();
  ParseResult parse_thermal_device_command();
  ParseResult parse_barrier_command();
  ParseResult parse_appliance_command();
  ParseResult parse_barrier_action();
  ParseResult parse_state();
  ParseResult parse_temperature();
  ParseResult parse_light_source();
  ParseResult parse_barrier();
  ParseResult parse_thermal_device();
  ParseResult parse_appliance();
  ParseResult parse_number();
  ParseResult parse_location();
  ParseResult parse_augmented_command();
  ParseResult parse_when_condition();
  ParseResult parse_until_condition();
  ParseResult parse_condition();
  ParseResult parse_temperature_condition();
  ParseResult parse_comparator();
  ParseResult parse_time_condition();

  Token getCurrentToken() const;
protected:
  std::vector<Token> tokens;
  int pos = 0;
};

#endif