#pragma once
#include <cstdint>
#include <vector>
#include "Building.h"

using namespace std;

class Player {
public:
    const int32_t getID() const;
    const int32_t getLoc() const;
    const int64_t getDeposit() const;
    const vector<Building> getEstate() const;

    void setID(const int32_t &id);
    void setLoc(const int32_t &loc);
    void setDeposit(const int64_t &d);
    void setEstate(const vector<Building> &newEstateList);

protected:
    int32_t _ID;
    int32_t _location;
    int64_t _deposit;
    vector<Building> _estate;
};