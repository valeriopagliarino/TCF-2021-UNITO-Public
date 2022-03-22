#include "fileexporttr.h"


FileExportTr::FileExportTr()
{

}

//______________________________________________________________________________
//
void FileExportTr::RunExportTr(Transverse *tr)
{
    if (Option=="root")
    {
        const std::string filename = exportPath.toStdString() + "XY_beam_slice." + Option;
        TFile f (filename.c_str(), "new") ;
        tr->Write();
        f.Close();
    }

    else
    {
        TCanvas *ExpC = new TCanvas ("ExpC", "XY beam slice", wc1s, hc1s);
        ExpC->cd();
        tr->getAt(Index)->Draw("lego2");
        ExpC->Draw();
        gPad->Update();

        std::string exportName = exportPath.toStdString() + "XY_beam_slice." + Option;

        ExpC->Print(exportName.c_str());

        delete ExpC;
    }
}

//______________________________________________________________________________
//
void FileExportTr::SetOpt(const std::string opt)
{
    Option=opt;
}

//______________________________________________________________________________
//
std::string FileExportTr::GetOpt()
{
    return Option;
}

//______________________________________________________________________________
//
void FileExportTr::setIndex(const int index)
{
    Index = index;
}
