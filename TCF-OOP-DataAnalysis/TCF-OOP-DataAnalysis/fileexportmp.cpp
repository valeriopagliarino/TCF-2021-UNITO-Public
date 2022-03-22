/*
e#include "fileexportmp.h"*/

#include"fileexportmp.h"

FileExportMp::FileExportMp()
{

}

//______________________________________________________________________________
//
void FileExportMp::RunExportMp(MeasPoint *mp)
{
    if (Option=="root")
    {
        const std::string filename =  exportPath.toStdString() + "Single run analysis." + Option;
        TFile f (filename.c_str(), "new") ;
        mp->Write();
        f.Close();
    }

    else
    {
        TCanvas *ExpC = new TCanvas ("ExpC", mp->getFilename().toStdString().c_str(), wc1s, hc1s);
        ExpC->cd();
        mp->Draw("");
        gPad->Update();

        std::string exportName = exportPath.toStdString() + "Single run analysis." + Option;

        ExpC->Print(exportName.c_str());

        delete ExpC;
    }
}

//______________________________________________________________________________
//
void FileExportMp::SetOpt(const std::string opt)
{
    Option=opt;
}

//______________________________________________________________________________
//
std::string FileExportMp::GetOpt()
{
    return Option;
}

