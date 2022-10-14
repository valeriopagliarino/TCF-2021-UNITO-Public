# DOCS: Calorimeter-Monte-Carlo - Tecniche di Calcolo per la Fisica (4/4)
### <a name="description"></a>6. Class reference

- [TABLE OF CONTENTS](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/TOC.html)
- [AbsAbsorber](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/absabsorber_h/index.html)
- [Absorber](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/absorber_h/index.html)
- [Calorimeter](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/calorimeter_h/index.html)
- [CodeEditor](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/codeeditor_h/index.html)
- [DetectorModel](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/detectormodel_h/index.html)
- [Element](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/element_h/index.html)
- [ElossMonteCarlo](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/elossmontecarlo_h/index.html)
- [ElossReco](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/elossreco_h/index.html)
- [FileExport](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fileexport_h/index.html)
- [FileExportEl](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fileexportel_h/index.html)
- [FileExportMp](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fileexportmp_h/index.html)
- [FileExportTr](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fileexporttr_h/index.html)
- [FileParser](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fileparser_h/index.html)
- [Fitterclass](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/fitterclass_h/index.html)
- [Gui](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/ui_gui_h/Classes/Ui_gui/index.html#//apple_ref/cpp/cl/Ui_gui)
- [Ions](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/ions_h/index.html)
- [LiquidCaloAbsorber](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/liquidcaloabsorber_h/index.html)
- [MeasPoint](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/measpoint_h/index.html)
- [PmmaAbsorber](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/pmmaabsorber_h/index.html)
- [QRootCanvas](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/qrootcanvas_h/index.html)
- [ScintAbsorber](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/scintabsorber_h/index.html)
- [StaticToolbox](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/statictoolbox_h/index.html)
- [Transverse](https://valeriopagliarino.github.io/Tecniche-di-calcolo-2021-UNITO/classRef/transverse_h/index.html)


### <a name="description"></a>7. Build and deployment information
This software has been designed using `Qt Creator 4.5.1` with the `Qt 5 framework` following the C++11 standard. The builing procedure is started running `qmake` on the project file with the makefile generation. At this point the compilations is started with `make`. This software has been tested using the LLVM compiler suite on Mac Os X 10.13.4 (High Sierra). This software uses a resource management based on qt `.qrc` file. This software requires some specific environment variables to be set in order to call the required external processes. (Starting the Geant4 backend etc...). The list of the environment variables is inside the `.user` configuration file. The backend project `Calorimeter`, instead, is compiled using CMake + LLVM and is linked to the frontend using executable paths in environment variables.


### <a name="description"></a>8. Further development
Further development includes a smoother installation procedure with automatic path detection and configuration of system env. vars. The integration with the Geant4 backend can be significantly upgraded by merging the frontend app with the backend gui in the same C++ project and replacing the external process call with an internal multithreading architecture. From the physical point of view, a more flexibile interface for detector modelization would be a nice upgrade, as well as a better integration of external CERN ROOT macros. Finally, a vector-based graphics rendering should look better with respect to the current raster-based visualization. 



[Previous page](./architectureClassDiagram.md)






