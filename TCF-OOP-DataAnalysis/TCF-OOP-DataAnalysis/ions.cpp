#include "ions.h"

Ions::Ions()
{

}

//______________________________________________________________________________
void Ions::initializeAllIons(std::vector<ionDisplay> &id)
{
    ionDisplay H = {.name = "H", .A = 1, .Z = 1, .Q = +1};
    id.push_back(H);

    ionDisplay C = {.name = "C", .A = 12, .Z = 6, .Q = +6};
    id.push_back(C);
}
