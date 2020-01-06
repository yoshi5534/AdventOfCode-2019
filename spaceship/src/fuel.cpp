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

    chemical.inputs.push_back(std::make_pair(quantity, name));
  }

  chemical.output = std::make_pair(std::stoi(splitted[splitted.size() - 2]),
                                   splitted[splitted.size() - 1]);

  reactions_.push_back(chemical);
}

namespace {

auto reactionForChemical(PossibleReactions const &reactions,
                         std::string const &name) {
  return std::find_if(
      std::begin(reactions), std::end(reactions),
      [&name](auto reaction) { return std::get<1>(reaction.output) == name; });
}

void chemicalsForFuel(std::map<std::string, int> &baseChemicals,
                      PossibleReactions const &reactions,
                      std::string const &name, int amount) {
  auto chemical = reactionForChemical(reactions, name);

  std::for_each(std::begin(chemical->inputs), std::end(chemical->inputs),
                [&](auto const &input) {
                  std::cout << amount << " of " << name << " needs "
                            << std::get<0>(input) << " of "
                            << std::get<1>(input) << "\n";
                  if (std::get<1>(input) == "ORE") {
                    baseChemicals[name] += amount;
                  } else
                    chemicalsForFuel(baseChemicals, reactions,
                                     std::get<1>(input),
                                     (std::get<0>(input) * amount +
                                      std::get<0>(chemical->output) - 1) /
                                         std::get<0>(chemical->output));
                });
}
} // namespace

int NanoFactory::necessaryORE() const {
  std::map<std::string, int> baseChemicals;
  auto fuel = reactionForChemical(reactions_, "FUEL");

  std::for_each(
      std::begin(fuel->inputs), std::end(fuel->inputs), [&](auto chemical) {
        std::cout << "FUEL needs " << std::get<0>(chemical) << " of "
                  << std::get<1>(chemical) << "\n";
        chemicalsForFuel(baseChemicals, reactions_, std::get<1>(chemical),
                         std::get<0>(chemical));
      });

  int ore = 0;
  std::for_each(
      std::begin(baseChemicals), std::end(baseChemicals), [&](auto chemical) {
        auto const reaction =
            reactionForChemical(reactions_, std::get<0>(chemical));
        auto const createdPerReaction = std::get<0>(reaction->output);
        auto const neededReactions =
            (std::get<1>(chemical) + createdPerReaction - 1) /
            createdPerReaction;
        auto const necessaryORE =
            neededReactions * std::get<0>(reaction->inputs[0]);
        ore += necessaryORE;
        std::cout << "Needed amount of chemical " << std::get<0>(chemical)
                  << ": " << std::get<1>(chemical) << " => consuming "
                  << necessaryORE << " ORE\n";
      });
  return ore;
}