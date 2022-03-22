#include "fileexportel.h"


FileExportEl::FileExportEl()
{

}


//______________________________________________________________________________
//
void FileExportEl::RunExportEl(ElossReco *er)
{
    if (Option=="root")
    {
        const std::string filename = exportPath.toStdString() + "Energy loss Profile." + Option;
        TFile f (filename.c_str(), "new") ;
        er->Write();
        f.Close();
    }

    else
    {
        TCanvas *ExpC = new TCanvas ("ExpC","Energy loss Profile", wc1s, hc1s);
        ExpC->cd();
        er->Draw("");
        gPad->Update();

        std::string exportName = exportPath.toStdString() + "Energy loss Profile." + Option;

        ExpC->Print(exportName.c_str());

        delete ExpC;
    }
}

//______________________________________________________________________________
//
void FileExportEl::SetOpt(const std::string opt)
{
    Option=opt;
}

//______________________________________________________________________________
//
std::string FileExportEl::GetOpt()
{
    return Option;
}
