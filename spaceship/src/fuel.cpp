#include <fuel.h>

#include <algorithm>

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

namespace {
std::string name(Chemical const &chemical) { return std::get<0>(chemical); }
int64_t amount(Chemical const &chemical) { return std::get<1>(chemical); }

using Storage = std::map<std::string, int64_t>;

int64_t produce(Chemical const &chemical, PossibleReactions const &reactions,
                Storage &unusedMaterial) {

  auto reaction = reactions.at(name(chemical));
  int64_t needed = amount(chemical);

  if (needed <= unusedMaterial[name(chemical)]) {
    unusedMaterial[name(chemical)] -= needed;
    return 0;
  }

  needed -= unusedMaterial[name(chemical)];
  unusedMaterial[name(chemical)] = 0;

  int numberOfReactions =
      (needed + amount(reaction.output) - 1) / amount(reaction.output);

  int64_t ore = 0;
  std::for_each(std::begin(reaction.inputs), std::end(reaction.inputs),
                [&](auto const &input) {
                  if (name(input) == "ORE") {
                    ore += numberOfReactions * amount(input);
                  } else {
                    auto const neededInput = std::make_pair(
                        name(input), amount(input) * numberOfReactions);
                    ore += produce(neededInput, reactions, unusedMaterial);
                  }
                });

  unusedMaterial[name(chemical)] =
      numberOfReactions * amount(reaction.output) - needed;

  return ore;
}

} // namespace

void NanoFactory::addReaction(std::string const &reaction) {
  using namespace ranges;
  auto splitted =
      reaction | views::split(' ') | ranges::to<std::vector<std::string>>();

  Reaction chemical;
  for (int i = 0; i < static_cast<int>(splitted.size()) - 3; i += 2) {
    int64_t quantity = std::stol(splitted[i]);
    std::string input = splitted[i + 1];
    input.erase(std::remove(std::begin(input), std::end(input), ','),
                std::end(input));

    chemical.inputs.push_back(std::make_pair(input, quantity));
  }

  chemical.output = std::make_pair(splitted[splitted.size() - 1],
                                   std::stol(splitted[splitted.size() - 2]));

  reactions_[name(chemical.output)] = chemical;
}

int64_t NanoFactory::necessaryORE() const {
  Storage unusedMaterial;
  return produce(std::make_pair("FUEL", 1), reactions_, unusedMaterial);
}

int64_t NanoFactory::maximumFuel(int64_t ore) const {
  Storage unusedMaterial;
  auto const fuel = std::make_pair("FUEL", 1);
  int produced = 0;
  while (ore > 0) {
    ore -= produce(fuel, reactions_, unusedMaterial);
    produced++;
  }

  return produced - 1;
}