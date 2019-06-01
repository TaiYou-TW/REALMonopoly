#include "Building.h"
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

// start point
Building::Building() {
    _location = 0;
    _name = "";
    _type = 0;
    _price = 0;
    _rank = 0;
    _owner = -1;
    _tax.clear();
}
// fate or chance
Building::Building(const int32_t &loc, const string &name, const int32_t &type) {
    _location = loc;
    _name = name;
    _type = type;
    _price = 0;
    _rank = 0;
    _owner = -1;
    _tax.clear();
}
// std building
Building::Building(const int32_t &loc, const string &name, const int32_t &type,
                   const int32_t &price, const int32_t &rank, const int32_t &owner,
                   const vector<int32_t> &tax) {
    _location = loc;
    _name = name;
    _type = type;
    _price = price;
    _rank = rank;
    _owner = owner;
    _tax = tax;
}

const int32_t Building::getLoc() const { return _location; }
const string Building::getName() const { return _name; }
const int32_t Building::getType() const { return _type; }
const int32_t Building::getPrice() const { return _price; }
const int32_t Building::getRank() const { return _rank; }
const int32_t Building::getOwner() const { return _owner; }
const int32_t Building::getTax() const { return _tax[_rank]; }
const int32_t Building::getTax(const int32_t &rk) const { return _tax[rk]; }

void Building::setLoc(const int32_t &loc) { _location = loc; }
void Building::setName(const string &name) { _name = name; }
void Building::setType(const int32_t &type) { _type = type; }
void Building::setPrice(const int32_t &price) { _price = price; }
void Building::setRank(const int32_t &rk) { _rank = rk; }
void Building::setOwner(const int32_t &p) { _owner = p; }
void Building::setTax(const vector<int32_t> &tax) { _tax = tax; }