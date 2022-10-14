# DOCS: Calorimeter-Monte-Carlo - Tecniche di Calcolo per la Fisica (3/4)
### <a name="description"></a>4. Architecture and design patterns

- <b> Factory pattern: simulation materials </b>

The <i> factory pattern </i> is used to provide an abstract interface that enable the use of different kind of materials for the calorimeter simulation. Each material class inherits from the `Absorber` class and reimplement the member functions that are specfic of the material. This pattern allows the implementation to be referred to an abstract material with no need of any change if new materials have to be added. At the same time, the specific material can implement inside the concrete classes advanced functionalities: for example the scintillator material foresee pixelization schematics for the simulation of scintillator matrices.

- <b> Visitor pattern: export class </b>

The visitor pattern is used for exporting data to file from different kind of classes that composes the experiment istance allowing each class to export in a different format, but providing to the C++ client code an agnostic, abstract, high-level interface.

- <b> Other inheritance and abstract interfaces </b>

The entire software is designed with a strong use of polymorphism and inheritance to allow a significant flexibility and code reusabilty. The base objects are the `MeasPoint` instances that provide tools for manipulating each run of the calorimeter in a fixed position. The `MeasPoint` class inherits both from the `TH1D` class of the `CERN ROOT Framework` and from the `FileParser` class that provides storage and methods for parsing the experiment output files. A collection of `MeasPoint` can be used to construct a `ElossReco` istance (energy loss reconstruction), that inherits from ROOT `TGraphErrors`. 
<p>
The simulation tools use a similar tree structure: the `ElossMonteCarlo` provides method for interfacing with the <b>Geant4</b> backend and need a `DetectorModel` object to simulate. `DetectorModel` is an abstract class, while `Calorimeter` is the specific concrete implementation used in this software. This allows more code reusability. The `DetectorModel` contains a `std::vector<Absorber *>` called `layers` where the geometry produced by the factory pattern in stored.


### <a name="description"></a>5. Class diagram

<p align="left" width="40">
  <img src="./svg/classDiag1.svg" width="100%">
</p>
  
<p align="left" width="40">
  <img src="./svg/classDiag2.svg" width="100%">
</p>
  
<p align="left" width="40">
  <img src="./svg/classDiag3.svg" width="100%">
</p>


[Previous page](./useCaseDiagram.md) | 
[Next page](./classes.md)






