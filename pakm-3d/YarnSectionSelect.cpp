#include "YarnSectionSelect.h"

BEGIN_EVENT_TABLE(CYarnSectionSelect, wxDialog)
	EVT_INIT_DIALOG(CYarnSectionSelect::OnInit)
	EVT_BUTTON(wxID_OK, CYarnSectionSelect::OnOK)
	EVT_BUTTON(XRCID("ConstantSection"), CYarnSectionSelect::OnConstantSection)
	EVT_BUTTON(XRCID("InterpPositionInsert"), CYarnSectionSelect::OnInterpPositionInsert)
	EVT_BUTTON(XRCID("InterpPositionRemove"), CYarnSectionSelect::OnInterpPositionRemove)
	EVT_BUTTON(XRCID("InterpPositionSection"), CYarnSectionSelect::OnInterpPositionSection)
	EVT_COMMAND_SCROLL(XRCID("InterpPositionSlider"), CYarnSectionSelect::OnInterpPositionSlider)
	EVT_TEXT(XRCID("InterpPositionTextBox"), CYarnSectionSelect::OnInterpPositionText)
	EVT_LISTBOX(XRCID("InterpPositionListBox"), CYarnSectionSelect::OnInterpPositionSelectionChanged)
	EVT_BUTTON(XRCID("InterpNodesInsert"), CYarnSectionSelect::OnInterpNodesInsert)
	EVT_BUTTON(XRCID("InterpNodesRemove"), CYarnSectionSelect::OnInterpNodesRemove)
	EVT_BUTTON(XRCID("InterpNodesSection"), CYarnSectionSelect::OnInterpNodesSection)
//	EVT_CHECKBOX(XRCID("Rotate"), CYarnSectionSelect::OnRotate)
//	EVT_CHECKBOX(XRCID("Scale"), CYarnSectionSelect::OnScale)
END_EVENT_TABLE()

CYarnSectionSelect::CYarnSectionSelect()
: m_Position(wxT("0.0"))
, m_iSliderPosition(0)
, m_ConstactSection(CSectionEllipse(1, 1))
, m_bPolar(false)
, m_bRamped(true)
, m_bConstMesh(true)
{
	BuildDialog();
}

CYarnSectionSelect::~CYarnSectionSelect(void)
{
}

void CYarnSectionSelect::BuildDialog()
{
	if (wxXmlResource::Get()->LoadDialog(this, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("YarnSection")))
	{
		XRCCTRL(*this, "InterpPositionTextBox", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m_Position));
		
		XRCCTRL(*this, "InterpNodesRamped", wxCheckBox)->SetValidator(wxGenericValidator(&m_bRamped));
		XRCCTRL(*this, "InterpNodesPolar", wxCheckBox)->SetValidator(wxGenericValidator(&m_bPolar));
		XRCCTRL(*this, "InterpNodesConstMesh", wxCheckBox)->SetValidator(wxGenericValidator(&m_bConstMesh));

		XRCCTRL(*this, "InterpPositionRamped", wxCheckBox)->SetValidator(wxGenericValidator(&m_bRamped));
		XRCCTRL(*this, "InterpPositionPolar", wxCheckBox)->SetValidator(wxGenericValidator(&m_bPolar));
		XRCCTRL(*this, "InterpPositionConstMesh", wxCheckBox)->SetValidator(wxGenericValidator(&m_bConstMesh));

		wxSlider* pSlider = XRCCTRL(*this, "InterpPositionSlider", wxSlider);
		pSlider->SetValidator(wxGenericValidator(&m_iSliderPosition));
		pSlider->SetTickFreq(10, 0);

		wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
		if (pListBox->GetCount() == 0)
		{
			int iPos = pListBox->Append(wxT("Section"), new CInterpPositionItem());
			UpdateInterpPositionItemText(iPos);
		}
	}
}

void CYarnSectionSelect::OnOK(wxCommandEvent& event)
{
	if (!Validate())
		return;
	if (!TransferDataFromWindow())
		return;
	wxChoicebook* pChoicebook = XRCCTRL(*this, "YarnSectionType", wxChoicebook);
	wxNotebookPage* pPage = pChoicebook->GetCurrentPage();
	if (pPage)
	{
		if (!pPage->Validate())
			return;
		if (!pPage->TransferDataFromWindow())
			return;
	}
	if (IsModal())
	{
		EndModal(wxID_OK);
	}
	else
	{
		SetReturnCode(wxID_OK);
		this->Show(false);
	}
}

void CYarnSectionSelect::OnConstantSection(wxCommandEvent& event)
{
	CSectionSelect::UpdateSection(m_ConstactSection);
}

void CYarnSectionSelect::OnInit(wxInitDialogEvent& event)
{
	XRCCTRL(*this, "Constant", wxPanel)->TransferDataToWindow();
	XRCCTRL(*this, "InterpNodes", wxPanel)->TransferDataToWindow();
	XRCCTRL(*this, "InterpPosition", wxPanel)->TransferDataToWindow();
}

void CYarnSectionSelect::OnInterpPositionInsert(wxCommandEvent& event)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
	int iPos = pListBox->Append(wxT("Section"), new CInterpPositionItem(*m_ConstactSection));
	UpdateInterpPositionItemText(iPos);
}

void CYarnSectionSelect::OnInterpPositionRemove(wxCommandEvent& event)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
	wxArrayInt Selections;
	int iNumSelection;
	while (iNumSelection = pListBox->GetSelections(Selections))
	{
		pListBox->Delete(Selections[0]);
	}
//	pListBox->InsertItems(1, &wxString(wxT("testing")), 0);
}

void CYarnSectionSelect::OnInterpPositionSection(wxCommandEvent& event)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
	wxArrayInt Selections;
	int iNumSelections = pListBox->GetSelections(Selections);
	if (iNumSelections == 0)
		return;
	CSectionSelect Dialog;
	CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(Selections[0]));
	if (pItem && pItem->m_Section)
		Dialog.LoadSettings(*pItem->m_Section);
	if (Dialog.ShowModal() == wxID_OK)
	{
		int i;
		for (i=0; i<iNumSelections; ++i)
		{
			CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(Selections[i]));
			if (pItem)
				pItem->m_Section = Dialog.GetSection();
		}
	}
}

void CYarnSectionSelect::OnInterpPositionSlider(wxScrollEvent &event)
{
	int iPos = event.GetPosition();
	float mu = iPos/100.0f;
	m_Position.clear();
	m_Position << mu;
	UpdateInterpPositionText();
	UpdateInterpPositionListboxText();
}

void CYarnSectionSelect::OnInterpPositionText(wxCommandEvent &event)
{
	wxValidator* pValidator = XRCCTRL(*this, "InterpPositionTextBox", wxTextCtrl)->GetValidator();
	if (pValidator)
		pValidator->TransferFromWindow();
	UpdateInterpPositionSlider();
	UpdateInterpPositionListboxText();
}

void CYarnSectionSelect::OnInterpPositionSelectionChanged(wxCommandEvent &event)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
	wxArrayInt Selections;
	int iNumSelection = pListBox->GetSelections(Selections);
	if (iNumSelection > 0)
	{
		CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(Selections[0]));
		if (pItem)
		{
			m_Position.clear();
			m_Position << pItem->m_dPosition;
			UpdateInterpPositionSlider();
			UpdateInterpPositionText();
		}
	}
}

void CYarnSectionSelect::UpdateInterpPositionText()
{
	wxValidator* pValidator = XRCCTRL(*this, "InterpPositionTextBox", wxTextCtrl)->GetValidator();
	if (pValidator)
		pValidator->TransferToWindow();
}

void CYarnSectionSelect::UpdateInterpPositionSlider()
{
	double dValue;
	if (m_Position.ToDouble(&dValue))
	{
		m_iSliderPosition = int(dValue*100.0);
		wxValidator* pValidator = XRCCTRL(*this, "InterpPositionSlider", wxSlider)->GetValidator();
		if (pValidator)
			pValidator->TransferToWindow();
	}
}

void CYarnSectionSelect::UpdateInterpPositionListboxText()
{
	double dValue;
	if (m_Position.ToDouble(&dValue))
	{
		if (dValue < 0)
			dValue = 0;
		else if (dValue > 1)
			dValue = 1;
		wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
		wxArrayInt Selections;
		int iNumSelection = pListBox->GetSelections(Selections);
		if (iNumSelection > 0)
		{
			CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(Selections[0]));
			if (pItem)
			{
				pItem->m_dPosition = dValue;
				UpdateInterpPositionItemText(Selections[0]);
			}
		}
	}
}

void CYarnSectionSelect::UpdateInterpPositionItemText(int iSelection)
{
	if (iSelection != wxNOT_FOUND)
	{
		wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
		CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(iSelection));
		if (pItem)
		{
			wxString Name = wxT("Section at Position ");
			Name << pItem->m_dPosition;
			pListBox->SetString(iSelection, Name);
		}
	}
}




void CYarnSectionSelect::SetNumberOfNodes(int iNumNodes)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
	pListBox->Clear();
	int i;
	for (i=0; i<iNumNodes; ++i)
	{
		int iPos = pListBox->Append(wxT("Section"), new CInterpNodesItem(i, *m_ConstactSection));
		UpdateInterpNodesItemText(iPos);
	}
}

void CYarnSectionSelect::OnInterpNodesInsert(wxCommandEvent& event)
{
/*	wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
	int iPos = pListBox->Append(wxT("Section"), new CInterpPositionItem());
	UpdateInterpPositionItemText(iPos);*/
}

void CYarnSectionSelect::OnInterpNodesRemove(wxCommandEvent& event)
{
/*	wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
	int iSelection = pListBox->GetSelection();
	if (iSelection != wxNOT_FOUND)
	{
		pListBox->Delete(iSelection);
	}*/
}

void CYarnSectionSelect::OnInterpNodesSection(wxCommandEvent& event)
{
	wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
	wxArrayInt Selections;
	int iNumSelections = pListBox->GetSelections(Selections);
	if (iNumSelections == 0)
		return;
	CSectionSelect Dialog;
	CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(Selections[0]));
	if (pItem && pItem->m_Section)
		Dialog.LoadSettings(*pItem->m_Section);
	if (Dialog.ShowModal() == wxID_OK)
	{
		int i;
		for (i=0; i<iNumSelections; ++i)
		{
			CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(Selections[i]));
			if (pItem)
				pItem->m_Section = Dialog.GetSection();
		}
	}
}

void CYarnSectionSelect::UpdateInterpNodesListboxText()
{
/*	wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
	int iSelection = pListBox->GetSelection();
	if (iSelection != wxNOT_FOUND)
	{
		CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(iSelection));
		if (pItem)
		{
			// May need to include some stuff here
		}
	}*/
}

void CYarnSectionSelect::UpdateInterpNodesItemText(int iSelection)
{
	if (iSelection != wxNOT_FOUND)
	{
		wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
		CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(iSelection));
		if (pItem)
		{
			wxString Name = wxT("Section at Node ");
			Name << pItem->m_iNode;
			pListBox->SetString(iSelection, Name);
		}
	}
}

void CYarnSectionSelect::SetDefaultSection(const CSection &Section)
{
	// Constant section
	m_ConstactSection = Section;

	// Interp Nodes
	{
		wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
		int i;
		CInterpNodesItem* pItem;
		for (i=0; i<(int)pListBox->GetCount(); ++i)
		{
			pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(i));
			if (pItem)
				pItem->m_Section = Section;
		}
	}

	// Interp position
	{
		wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
		int i;
		CInterpPositionItem* pItem;
		for (i=0; i<(int)pListBox->GetCount(); ++i)
		{
			pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(i));
			if (pItem)
				pItem->m_Section = Section;
		}
	}
}

void CYarnSectionSelect::LoadSettings(const CYarnSection &YarnSection)
{
	wxChoicebook* pChoicebook = XRCCTRL(*this, "YarnSectionType", wxChoicebook);
	string Type = YarnSection.GetType();
	if (Type == "CYarnSectionConstant")
	{
		CYarnSectionConstant &Constant = (CYarnSectionConstant&)YarnSection;
		SetDefaultSection(Constant.GetSection());
		m_ConstactSection = Constant.GetSection();
		pChoicebook->SetSelection(0);
	}
	else if (Type == "CYarnSectionInterpNode")
	{
		CYarnSectionInterpNode &InterpNode = (CYarnSectionInterpNode&)YarnSection;
		int iNumNodes = InterpNode.GetNumNodeSections();
		if (iNumNodes > 0)
			SetDefaultSection(InterpNode.GetNodeSection(0));
		SetNumberOfNodes(iNumNodes);
		wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
		int i;
		for (i=0; i<iNumNodes; ++i)
		{
			CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(i));
			if (pItem)
				pItem->m_Section = InterpNode.GetNodeSection(i);
		}
		m_bRamped = InterpNode.GetRamped();
		m_bPolar = InterpNode.GetPolar();
		m_bConstMesh = InterpNode.GetForceMeshLayers();
		pChoicebook->SetSelection(1);
	}
	else if (Type == "CYarnSectionInterpPosition")
	{
		CYarnSectionInterpPosition &InterpPosition = (CYarnSectionInterpPosition&)YarnSection;
		int iNumSections = InterpPosition.GetNumNodeSections();
		if (iNumSections > 0)
			SetDefaultSection(InterpPosition.GetSection(0));
		wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
		pListBox->Clear();
		int i;
		CInterpPositionItem* pItem;
		for (i=0; i<iNumSections; ++i)
		{
			pItem = new CInterpPositionItem();
			pItem->m_dPosition = InterpPosition.GetSectionPosition(i);
			pItem->m_Section = InterpPosition.GetSection(i);
			int iPos = pListBox->Append(wxT("Section"), pItem);
			UpdateInterpPositionItemText(iPos);
		}
//		m_ConstactSection.LoadSettings(*Constant.GetSection());
		m_bRamped = InterpPosition.GetRamped();
		m_bPolar = InterpPosition.GetPolar();
		m_bConstMesh = InterpPosition.GetForceMeshLayers();
		pChoicebook->SetSelection(2);
	}
}

CYarnSection* CYarnSectionSelect::GetCreateSection()
{
	stringstream StringStream;
	wxChoicebook* pChoicebook = XRCCTRL(*this, "YarnSectionType", wxChoicebook);
	wxNotebookPage* pPage = pChoicebook->GetCurrentPage();
	if (pPage)
	{
		if (pPage == XRCCTRL(*this, "Constant", wxPanel))
		{
			auto yarnsection = new CYarnSectionConstant(*m_ConstactSection);
			return yarnsection;
		}
		else if (pPage == XRCCTRL(*this, "InterpNodes", wxPanel))
		{
			auto yarnsection = new CYarnSectionInterpNode(m_bRamped, m_bPolar, m_bConstMesh);
			wxListBox* pListBox = XRCCTRL(*this, "InterpNodesListBox", wxListBox);
			bool bRectangle = CheckRectangleSections( pListBox, "InterpNodes" );
			for (int i = 0; i < pListBox->GetCount(); i++)
			{
				auto pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(i));
				if (pItem)
				{
					yarnsection->AddSection(*pItem->m_Section);
				}
			}
			return yarnsection;
		}
		else if (pPage == XRCCTRL(*this, "InterpPosition", wxPanel))
		{
			auto yarnsection = new CYarnSectionInterpPosition(m_bRamped, m_bPolar, m_bConstMesh);
			wxListBox* pListBox = XRCCTRL(*this, "InterpPositionListBox", wxListBox);
			bool bRectangle = CheckRectangleSections( pListBox, "InterpPosition" );
			for (int i = 0; i < pListBox->GetCount(); i++)
			{
				auto pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(i));
				if (pItem)
				{
					yarnsection->AddSection(pItem->m_dPosition, *pItem->m_Section);
				}
			}
			return yarnsection;
		}
	}
	return nullptr;
}

bool CYarnSectionSelect::CheckRectangleSections( wxListBox* pListBox, string str )
{
	int i, iCount = pListBox->GetCount();
	int iNumRects = 0;
	for (i=0; i<iCount; ++i)
	{
		string Type;
		bool bHybridRect = false;
		
		if ( str == "InterpNodes" )
		{
			CInterpNodesItem* pItem = dynamic_cast<CInterpNodesItem*>(pListBox->GetClientObject(i));
			const CSection& Section = *pItem->m_Section;
			Type = Section.GetType();
			if ( Type == "CSectionHybrid" )
				bHybridRect = CheckHybridSection( Section );
			else if ( Type == "CSectionRotated" && Section.GetBaseType() == "CSectionRectangle" )
				bHybridRect = true;
		}
		else if ( str == "InterpPosition" )
		{
			CInterpPositionItem* pItem = dynamic_cast<CInterpPositionItem*>(pListBox->GetClientObject(i));	
			const CSection& Section = *pItem->m_Section;
			Type = Section.GetType();
			if ( Type == "CSectionHybrid" )
				bHybridRect = CheckHybridSection( Section );
			else if ( Type == "CSectionRotated" && Section.GetBaseType() == "CSectionRectangle" )
				bHybridRect = true;
		}
				
		if ( Type == "CSectionRectangle" || bHybridRect )
			iNumRects++;
	}
	if ( iNumRects > 0 && iNumRects != iCount )
		return true;
	return false;
}

bool CYarnSectionSelect::CheckHybridSection( const CSection& Section )
{
	const CSectionHybrid &Hybrid = (const CSectionHybrid&)Section;
	for ( int i = 0; i < Hybrid.GetNumDivisions() ; i++ )
	{
		const CSection& HybridSection = Hybrid.GetSection(i);
		if ( HybridSection.GetType() == "CSectionRectangle" )
			return true;
	}
	return false;
}
