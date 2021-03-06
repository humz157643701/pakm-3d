#pragma once
#include "wx/wx.h"
#include <wx/dcbuffer.h>
#include "wx\settings.h"
#include "WeaveWizard3D.h"
#include "TexGen.h"
using namespace TexGen;

/// Weave pattern control used for setting the weave pattern
class wxWeavePatternCtrl3D : public wxControl
{
	DECLARE_DYNAMIC_CLASS(wxWeavePatternCtrl3D)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	wxWeavePatternCtrl3D();
	wxWeavePatternCtrl3D(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE,
		const wxValidator& validator = wxDefaultValidator);
	~wxWeavePatternCtrl3D();
	// Creation
	bool Create(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE,
		const wxValidator& validator = wxDefaultValidator);

	void SetWeaveSize(int iWeaveType, int iNumXYarns, int iNumYYarns, YARNDATA& Warp, YARNDATA& Weft, YARNDATA& Binder, int iNumBinderLayers, bool bOffsetWeft );
	void SetWeaveSize(const CTextile3DWeave &Weave);
	int GetWeaveWidth();
	int GetWeaveHeight();
	int GetWeaveNumBinderLayers();
	int GetWeaveNumWarpLayers();
	int GetWeaveNumWeftLayers();
	int GetCellStatus(int i, int j);
	int GetCellBottom( int i, int j );
	bool IsBinderYarn( int j );
	bool bHasWeave() { return m_WeavePattern?true:false; }
	CTextile3DWeave &GetWeave() { return *m_WeavePattern; }
	void SetCrossSectionIndex( int Index ) { m_iCrossSectionIndex = Index; }

protected:
	enum
	{
		MENUID_SETYARNWIDTH,
		MENUID_SETYARNHEIGHT,
		MENUID_SETYARNSPACING,
	};

	struct YARN_POS_DATA
	{
		int iCellMin;
		int iYarnMin;
		int iYarnCenter;
		int iYarnMax;
		int iCellMax;
	};

	// Overrides
	wxSize DoGetBestSize() const { return wxSize(600, 400); }
	// Event handlers
	void OnPaint(wxPaintEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMenu(wxCommandEvent& event);

	void Init();
	pair<int, int> GetCellIndices(wxPoint Position);
	pair<int, int> GetCellHeight(wxPoint Position);
	bool HandleIconSelect(wxPoint Position);
	void HandleIconSelect(wxRect MarqueeSelect);
	wxRect GetLeftIconsRegion();
	wxRect GetTopIconsRegion();
	wxRect GetTopIconRect(int i);
	wxRect GetLeftIconRect(int i);
	wxRect GetWeaveRegion();
	wxRect GetCrossSectionRegion();
	wxRect GetCellRegion(int x, int y);
	wxPoint GetCrossSectionPos(int iIndex, double dZValue);
	wxRect GetMarqueeRect();
	YARN_POS_DATA GetXCoordinate(int iIndex);
	YARN_POS_DATA GetYCoordinate(int iIndex);
	int GetNumberOfYarns(int i, int j);
	void DrawTopDownView(wxDC &dc);
	void DrawCell(wxDC &dc, int i, int j, bool bDirection);
	void DrawCrossSection(wxDC &dc, int iIndex);
	void DrawHorizontalYarn(wxDC &dc, vector<double> &YarnZValues);
	void DrawVerticalYarn(wxDC &dc, int iIndex, double dZValue);
	void DrawLeftIcons(wxDC &dc);
	void DrawTopIcons(wxDC &dc);
	int GetNumSelected();
	void CalculatePixelRatio();

	double m_dPixelRatio;
	int m_iFreeWidth;
	int m_iFreeHeight;
	double m_dLastSelectedWidth;
	double m_dLastSelectedHeight;
	double m_dLastSelectedSpacing;
	CObjectContainer<CTextile3DWeave> m_WeavePattern;
	// The amount of space left for the icons
	int m_iIconSpace;
	int m_iBorderSpace;
	// These refers to the cross section index along the y axis
	int m_iCrossSectionIndex;
	// These refer to indices in the cross section
	// the first is the index along the length of the yarn in the x axis
	// the second is the height index
	pair<int, int> m_SelectedIndices;
	vector<bool> m_SelectedYYarns;
	vector<bool> m_SelectedXYarns;
	wxPoint m_MarqueeStart;
	wxPoint m_MarqueeEnd;
};



/// Circle range control event class
class wxWeavePatternCtrl3DEvent : public wxCommandEvent
{
friend class wxWeavePatternCtrl3D;
public:
	wxWeavePatternCtrl3DEvent(wxEventType commandType = wxEVT_NULL, int id = 0)
	: wxCommandEvent(commandType, id)
	{}
	wxWeavePatternCtrl3DEvent(const wxWeavePatternCtrl3DEvent& event)
	: wxCommandEvent(event)
	{}
	virtual wxEvent *Clone() const
	{
		return new wxWeavePatternCtrl3DEvent(*this);
	}

protected:
	DECLARE_DYNAMIC_CLASS(wxWeavePatternCtrl3DEvent);
};
