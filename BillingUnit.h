//---------------------------------------------------------------------------

#ifndef BillingUnitH
#define BillingUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmCallBilling : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TPanel *pnlTop;
	TImage *imgPnlTop;
	TEdit *edtStartTime;
	TEdit *edtendtime;
	TButton *btnCalcDuration;
	TListBox *lstOutput;
	TButton *btnSortExtension;
	TButton *btnLoad;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *lblBusy;
	void __fastcall btnCalcDurationClick(TObject *Sender);
	void __fastcall btnSortExtensionClick(TObject *Sender);
	void __fastcall btnLoadClick(TObject *Sender);
	void __fastcall lstOutputClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmCallBilling(TComponent* Owner);
	TStringList *myFileDataList;
	//AnsiString ExtractField(AnsiString OneRecordLine, int FieldInd, int FieldCount);
  AnsiString getFieldNum(AnsiString OneRecordLine, int fieldNr, char delim);
	AnsiString HeadingData[12];
	void AddHeadings();

	float calcCost(int Duration);
	void AddData();

	// 1.1
	int calcCallDuration(AnsiString StartTime, AnsiString EndTime);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCallBilling *frmCallBilling;
//---------------------------------------------------------------------------
#endif
