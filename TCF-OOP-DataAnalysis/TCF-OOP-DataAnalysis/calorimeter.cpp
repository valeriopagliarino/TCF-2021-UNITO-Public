#include "calorimeter.h"

//______________________________________________________________________________
Calorimeter::Calorimeter()
{

}

//______________________________________________________________________________
void Calorimeter::buildBeamDeliveryPMT(double D0mm, double TicknessMm, Absorber * mat, double &zeroRef, int index)
{
    mat->setTallyIndex(index);
    mat->setHeigh(100.);
    mat->setWidth(100.);
    mat->setTickness(TicknessMm);
    mat->setXpos(0);
    mat->setYpos(0);
    mat->setZpos(D0mm);
    zeroRef = zeroRef + D0mm + TicknessMm;
    addLayer(mat);
}

//______________________________________________________________________________
void Calorimeter::buildCalorimeterWall(double D1mm, double TicknessMm, Absorber * mat, double &zeroRef, int index)
{
    mat->setTallyIndex(index);
    mat->setHeigh(300.);
    mat->setWidth(400.);
    mat->setTickness(TicknessMm);
    mat->setXpos(0);
    mat->setYpos(0);
    mat->setZpos(zeroRef + D1mm);
    zeroRef = zeroRef + D1mm + TicknessMm;
    addLayer(mat);
}

//______________________________________________________________________________
void Calorimeter::buildCalorimeterAbso(double TicknessMm, Absorber * mat, double &zeroRef, int index)
{
    mat->setTallyIndex(index);
    mat->setHeigh(300.);
    mat->setWidth(400.);
    mat->setTickness(TicknessMm);
    mat->setXpos(0);
    mat->setYpos(0);
    mat->setZpos(zeroRef);
    zeroRef = zeroRef + TicknessMm;
    addLayer(mat);
}

//______________________________________________________________________________
void Calorimeter::buildSelaedNacelWall(double TicknessMm, Absorber * mat, double &zeroRef, int index)
{
    mat->setTallyIndex(index);
    mat->setHeigh(70.);
    mat->setWidth(40.);
    mat->setTickness(TicknessMm);
    mat->setXpos(0);
    mat->setYpos(0);
    mat->setZpos(zeroRef);
    zeroRef = zeroRef + TicknessMm;
    addLayer(mat);
}

//______________________________________________________________________________
void Calorimeter::buildScintillatorXtl(double D3mm, double TicknessMm, Absorber * mat, double &zeroRef, int nx, int ny, double sizeYZ, int index)
{
    int localIndex = 0;

    if ((nx == 1) && (ny == 1))
    {
        mat->setTallyIndex(index);
        mat->setHeigh(sizeYZ);
        mat->setWidth(sizeYZ);
        mat->setTickness(TicknessMm);
        mat->setXpos(0);
        mat->setYpos(0);
        mat->setZpos(zeroRef + D3mm);
        zeroRef = zeroRef + TicknessMm;
        addLayer(mat);
    }

    /*
    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            mat->setTallyIndex(index + localIndex);
            localIndex++;
            mat->setHeigh(sizeYZ / (double)nx);
            mat->setWidth(sizeYZ / (double)ny);
            mat->setXpos((sizeYZ / (double)nx) * i);
            mat->setYpos((sizeYZ / (double)ny) * j);
            mat->setTickness(TicknessMm);
            mat->setZpos(zeroRef + D3mm);
            addLayer(mat);
        }
    }
    zeroRef = zeroRef + TicknessMm;
    */
}
