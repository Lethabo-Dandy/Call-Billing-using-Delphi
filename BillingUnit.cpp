//---------------------------------------------------------------------------

#include <vcl.h>
#include <DateUtils.hpp>
#pragma hdrstop

#include "BillingUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCallBilling *frmCallBilling;
//---------------------------------------------------------------------------
__fastcall TfrmCallBilling::TfrmCallBilling(TComponent* Owner): TForm(Owner)
{
	//headings for the columns
	HeadingData[0] = "CallDate";
	HeadingData[1] = "StartTime";
	HeadingData[2] = "EndTime";
	HeadingData[3] = "Extension";
	HeadingData[4] = "Called Nr";
	HeadingData[5] = "Provider";
	HeadingData[6] = "Duration";
	HeadingData[7] = "Cost";
	HeadingData[11] = "----------";
}
//---------------------------------------------------------------------------
void __fastcall TfrmCallBilling::btnLoadClick(TObject *Sender)
{
	//load the csv file
	lstOutput->Items->LoadFromFile("MonthCallData.csv");
	AddHeadings();
	Application->ProcessMessages();

	AddData();        //add calculated data of calls and prices
}
//---------------------------------------------------------------------------
AnsiString TfrmCallBilling::getFieldNum(AnsiString OneRecordLine, int fieldNr, char delim)
{
	//get field from a record line
	OneRecordLine += delim;
	AnsiString field;
	for (int i = 1; i <= fieldNr; i++)
	{
		int pos = OneRecordLine.Pos(delim);
		field = OneRecordLine.SubString(1, pos - 1);
		OneRecordLine.Delete(1, pos);
	}
	return field;
}
//---------------------------------------------------------------------------
void TfrmCallBilling::AddHeadings()
{
	//add headings
	AnsiString OneListLine;
	OneListLine = "";
	for (int i = 0; i < 8; i++)            // Headers added
	{
		OneListLine = OneListLine + HeadingData[i] + "\t";
	}
	lstOutput->Items->Insert(0, OneListLine);

	OneListLine = "";
	for (int i = 0; i < 8; i++)            // "-----------" added
	{
		OneListLine = OneListLine + HeadingData[11]+ "\t";
	}
	lstOutput->Items->Insert(1, OneListLine);
}
//---------------------------------------------------------------------------
void TfrmCallBilling::AddData()
{
	//calculate the call duration and price

	int CallDuration;    // in seconds!
	float CallCost;

	AnsiString OneListLine, starT, endT;
	for (int recordNr = 2; recordNr < lstOutput->Count; recordNr++)
	{
		OneListLine = lstOutput->Items->Strings[recordNr];
		starT = getFieldNum(OneListLine, 2, '\t');
		endT  = getFieldNum(OneListLine, 3, '\t');

		CallDuration = calcCallDuration(starT, endT);
		OneListLine = OneListLine + "\t" + IntToStr(CallDuration);

		CallCost = calcCost(CallDuration);
		OneListLine = OneListLine + "\t" +FloatToStrF(CallCost,ffCurrency, 6, 2);

		lstOutput->Items->Strings[recordNr] = OneListLine;// OVERWRITE original line!
	}
}
//---------------------------------------------------------------------------
int TfrmCallBilling::calcCallDuration(AnsiString StartTime, AnsiString EndTime)
{
	//Time example "13:15:20"
	int HH , MM , SS;
	int TotSecStart, TotSecEnd, Duration;

	/* ... the "odd" way
	TotSecStart = StrToInt(StartTime.SubString(1,2)) * 3600 +
				  StrToInt(StartTime.SubString(4,2)) * 60 +
				  StrToInt(StartTime.SubString(7,2));


	TotSecEnd =   StrToInt(EndTime.SubString(1,2)) * 3600 +
				  StrToInt(EndTime.SubString(4,2)) * 60 +
				  StrToInt(EndTime.SubString(7,2));

	Duration = TotSecEnd - TotSecStart;
	return Duration; */

	//... the professional way!
	TDateTime T1 , T2;
	T1 = StrToTime(StartTime);
	T2 = StrToTime(EndTime);

	return SecondsBetween(T2,T1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCallBilling::lstOutputClick(TObject *Sender)
{
	//extract two fields (start and end for calls)
	int indx = lstOutput->ItemIndex;
	edtStartTime->Text = getFieldNum(lstOutput->Items->Strings[indx], 2, '\t'); //call start
	edtendtime->Text = getFieldNum(lstOutput->Items->Strings[indx], 3, '\t');   //call end
}
//---------------------------------------------------------------------------
void __fastcall TfrmCallBilling::btnCalcDurationClick(TObject *Sender)
{
	//show the message for calculated call duration
	ShowMessage("The total seconds between " + edtStartTime->Text +
			  " and " + edtStartTime->Text + " is "  +
			  IntToStr(calcCallDuration(edtStartTime->Text,edtendtime->Text))
			  + " seconds");
}
//---------------------------------------------------------------------------
float TfrmCallBilling::calcCost(int Duration)
{
	//calculate the cost
	return Duration * 0.0115;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCallBilling::btnSortExtensionClick(TObject *Sender)
{
	// sort the data according to extension
	bool swopped = true;
	int arrSize = lstOutput->Count;
	int iEnd = arrSize -1;
	AnsiString Ext1, Ext2;

	lblBusy->Visible = true;
	Application->ProcessMessages();

	while (swopped)
	{
		swopped = false;
		for (int c=2; c < iEnd; c++)       // 2 x lines for headers
		{
			Ext1 = getFieldNum(lstOutput->Items->Strings[c], 4, '\t');
			Ext2 = getFieldNum(lstOutput->Items->Strings[c+1], 4, '\t');
			if (Ext1 > Ext2)
			{
				AnsiString txt = lstOutput->Items->Strings[c];
				lstOutput->Items->Strings[c] = lstOutput->Items->Strings[c+1];
				lstOutput->Items->Strings[c+1] = txt;
				swopped = true;
			}
		}
		Application->ProcessMessages();
		iEnd--;
	}
	lblBusy->Visible = false;
}
//----------------------------------------------------------------------------

