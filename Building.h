#pragma once
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class Building {
public:
    Building();
    Building(const int32_t &loc, const string &name, const int32_t &type);
    Building(const int32_t &loc, const string &name, const int32_t &type,
             const int32_t &price, const int32_t &rank, const int32_t &owner,
             const vector<int32_t> &tax);

    const int32_t getLoc() const;
    const string getName() const;
    const int32_t getType() const;
    const int32_t getPrice() const;
    const int32_t getRank() const;
    const int32_t getOwner() const;
    const int32_t getTax() const;
    const int32_t getTax(const int32_t &rk) const;

    void setLoc(const int32_t &loc);
    void setName(const string &name);
    void setType(const int32_t &type);
    void setPrice(const int32_t &price);
    void setRank(const int32_t &rk);
    void setOwner(const int32_t &p);
    void setTax(const vector<int32_t> &tax);

protected:
    int32_t _location;
    string _name;
    int32_t _type;
    int32_t _price;
    int32_t _rank;
    int32_t _owner;
    vector<int32_t> _tax;
};