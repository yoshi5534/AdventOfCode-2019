#include <fuel.h>

#include <algorithm>
#include <iostream>

#include <range/v3/view.hpp>

using namespace AdventOfCode;

int AdventOfCode::fuel_calculation(int mass) {
  return static_cast<int>(std::floor(static_cast<double>(mass) / 3.0) - 2.0);
}

int AdventOfCode::fuel_for_module(int mass) {
  int fuel = 0;
  while ((mass = fuel_calculation(mass)) > 0)
    fuel += mass;

  return fuel;
}

void NanoFactory::addReaction(std::string const &reaction) {
  using namespace ranges;
  auto splitted =
      reaction | views::split(' ') | ranges::to<std::vector<std::string>>();

  Reaction chemical;
  for (int i = 0; i < splitted.size() - 3; i += 2) {
    int quantity = std::stoi(splitted[i]);
    std::string name = splitted[i + 1];
    name.erase(std::remove(std::begin(name), std::end(name), ','),
               std::end(name));

    chemical.inputs.push_back(std::make_pair(name, quantity));
  }

  chemical.output = std::make_pair(splitted[splitted.size() - 1],
                                   std::stoi(splitted[splitted.size() - 2]));

  reactions_.push_back(chemical);
}

namespace {
std::string name(Chemical const &chemical) { return std::get<0>(chemical); }
int amount(Chemical const &chemical) { return std::get<1>(chemical); }

using Storage = std::map<std::string, int>;

auto reactionForChemical(PossibleReactions const &reactions,
                         Chemical const &chemical) {
  return std::find_if(std::begin(reactions), std::end(reactions),
                      [&chemical](auto reaction) {
                        return name(reaction.output) == name(chemical);
                      });
}

int produce(Chemical const &chemical, PossibleReactions const &reactions,
            Storage &unusedMaterial) {

  auto reaction = reactionForChemical(reactions, chemical);
  int needed = amount(chemical);

  if (needed <= unusedMaterial[name(chemical)]) {
    unusedMaterial[name(chemical)] -= needed;
    return 0;
  }

  needed -= unusedMaterial[name(chemical)];
  unusedMaterial[name(chemical)] = 0;

  int numberOfReactions =
      (needed + amount(reaction->output) - 1) / amount(reaction->output);

  int ore = 0;
  std::for_each(std::begin(reaction->inputs), std::end(reaction->inputs),
                [&](auto const& input) {
                  std::cout << needed << " of " << name(chemical) << " needs "
                            << amount(input) * numberOfReactions << " of " << name(input) << "\n";
                  if (name(input) == "ORE") {
                    ore += numberOfReactions * amount(input);
                  } else {
                    auto const neededInput = std::make_pair (name(input), amount(input) * numberOfReactions);
                    ore += produce(neededInput, reactions, unusedMaterial);
                  }
                });

  unusedMaterial[name(chemical)] =
      numberOfReactions * amount(reaction->output) - needed;

  return ore;
}

} // namespace

int NanoFactory::necessaryORE() const {
  Storage unusedMaterial;

  auto fuel = reactionForChemical(reactions_, std::make_pair("FUEL", 1));

  int ore = 0;

  std::for_each(std::begin(fuel->inputs), std::end(fuel->inputs),
                [&](auto chemical) {
                  std::cout << "FUEL needs " << amount(chemical) << " of "
                            << name(chemical) << "\n";

                  ore += produce(chemical, reactions_, unusedMaterial);
                });

  return ore;
}