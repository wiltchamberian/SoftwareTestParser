#include "run.h"

void Parser::tokenize(const std::string& input) {
  tokens.clear();
  pos = 0;

  int i = 0;
  int j = 0;
  for (j = 0; j < input.size(); ++j) {
    if (input[j] == ' ') {
      std::string word = input.substr(i, j - i);
      tokens.push_back(Token(word));
      for (; j < input.size(); ++j) {
        if (input[j] != ' ') {
          break;
        }
      }
      i = j;
      j--;
    }
  }
  if (i < j) {
    std::string word = input.substr(i, j - i);
    tokens.push_back(Token(word));
  }

}

bool Parser::expect(const std::string& literal) {
  if (pos >= tokens.size()) {
    return false;
  }
  if (tokens[pos].literal == literal) {
    pos += 1;
    return true;
  }
  else {
    return false;
  }
  return false;
}

ParseResult Parser::parse_command() {
  parse_location();

  if (parse_lighting_command()
    || parse_barrier_command()
    || parse_appliance_command()
    || parse_thermal_device_command()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_lighting_command() {
  bool ok = expect("turn");
  if (!ok) {
    return false;
  }
  if (parse_light_source() && parse_state()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_thermal_device_command() {
  if (!expect("set")) {
    return false;
  }
  if (!parse_thermal_device()) {
    return false;
  }
  if (!expect("to")) {
    return false;
  }
  if (!parse_temperature()) {
    return false;
  }
  return true;
}

ParseResult Parser::parse_barrier_command() {
  if (parse_barrier_action() && parse_barrier()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_appliance_command() {
  if (expect("turn") && parse_appliance() && parse_state()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_barrier_action() {
  if (expect("lock")
    || expect("unlock")
    || expect("open")
    || expect("close")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_state() {
  if (expect("on") || expect("off")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_temperature() {
  if (parse_number() && expect("K")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_light_source() {
  if (expect("lamp") || expect("bulb") || expect("neon") || expect("sconce") || expect("brazier")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_barrier() {
  if (expect("gate")
    || expect("curtains")
    || expect("garage-door")
    || expect("blinds")
    || expect("window")
    || expect("shutter")
    || expect("trapdoor")
    || expect("portcullis")
    || expect("drawbridge")
    || expect("blast-door")
    || expect("airlock")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_thermal_device() {
  if (expect("oven")
    || expect("thermostat")
    || expect("electric-blanket")
    || expect("incinerator")
    || expect("reactor-core")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_appliance() {
  if (expect("coffee-maker")
    || expect("oven")
    || expect("air-conditioner")
    || expect("centrifuge")
    || expect("synchrotron")
    || expect("laser-cannon")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_number() {
  if (pos >= tokens.size()) {
    return false;
  }
  const std::string& str = tokens[pos].literal;
  if (str.empty()) {
    return false;
  }
  for (int i = 0; i < str.size(); ++i) {
    if (!(str[i] >= '0' && str[i] <= '9')) {
      return false;
    }
  }
  pos += 1;
  return true;
}

ParseResult Parser::parse_location() {
  if (expect("main-bedroom")
    || expect("kitchen")
    || expect("laboratory")
    || expect("armory")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_augmented_command() {
  if (!parse_command()) {
    return false;
  }
  parse_when_condition();
  parse_until_condition();
  return true;
}

ParseResult Parser::parse_when_condition() {
  if (expect("when") && parse_condition()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_until_condition() {
  if (expect("until") && parse_condition()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_condition() {
  if (parse_temperature_condition() || parse_time_condition()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_temperature_condition() {
  if (expect("current-temperature")
    && parse_comparator()
    && parse_temperature()) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_comparator() {
  if (expect("less-than")
    || expect("greater-than")
    || expect("equal-to")) {
    return true;
  }
  return false;
}

ParseResult Parser::parse_time_condition() {
  if (pos >= tokens.size()) {
    return false;
  }
  const std::string& time = tokens[pos].literal;
  if (time.size() == 5) {
    if (time[0] >= '0' && time[0] <= '9'
      && time[1] >= '0' && time[1] <= '9'
      && time[2] == ':'
      && time[3] >= '0' && time[3] <= '9'
      && time[4] >= '0' && time[4] <= '9') {
      pos += 1;
      if (pos >= tokens.size()) {
        return false;
      }
      if (tokens[pos].literal == "am"
        || tokens[pos].literal == "pm") {
        pos += 1;
        return true;
      }
    }
  }
  return false;
}

Token Parser::getCurrentToken() const {
  if (pos >= 0 && pos < tokens.size()) {
    return tokens[pos];
  }
  return Token("");
}

// REPL function to handle user inputs
void repl() {
  string input;
  cout << "Welcome to the Command Parser REPL!\n";

  while (true) {
    cout << "> ";
    getline(cin, input);

    if (input == "exit") break;

    Parser parser;
    parser.tokenize(input);
    bool ok = parser.parse_augmented_command();
    if (ok) {
      cout << "Command recognized successfully." << std::endl;
    }
    else {
      Token tk = parser.getCurrentToken();

      cout << "Command syntax error detected at:"
        <<tk.literal <<"\n";
    }
  }
}

int main() {

    repl();
    return 0;
}
