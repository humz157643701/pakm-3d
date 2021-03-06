#include "stdafx.h"
#include "SectionBezier.h"

using namespace TexGen;

CSectionBezier::CSectionBezier(const vector<XY> &BezierPoints, bool bSingleQuadrant)
: m_BezierPoints(BezierPoints)
{
	if (!bSingleQuadrant)
	{
		if (m_BezierPoints.size() % 3 != 0)
		{
			TGERROR("Unable to create bezier section, number of points must be multiple of 3");
			m_BezierPoints.clear();
		}
	}
	else
	{
		if ((m_BezierPoints.size()-1) % 3 != 0)
		{
			TGERROR("Unable to create bezier section, number of points-1 must be multiple of 3");
			m_BezierPoints.clear();
			return;
		}
		vector<XY>::const_iterator itPoint;
		XY P;
		// Reflect to top left quadrant
		for (itPoint = BezierPoints.end()-2; itPoint != BezierPoints.begin(); --itPoint)
		{
			P = *itPoint;
			P.x *= -1;
			m_BezierPoints.push_back(P);
		}
		// Reflect to bottom left quadrant
		for (itPoint = BezierPoints.begin(); itPoint != BezierPoints.end(); ++itPoint)
		{
			P = *itPoint;
			P.y *= -1;
			P.x *= -1;
			m_BezierPoints.push_back(P);
		}
		// Reflect to bottom right quadrant
		for (itPoint = BezierPoints.end()-2; itPoint != BezierPoints.begin(); --itPoint)
		{
			P = *itPoint;
			P.y *= -1;
			m_BezierPoints.push_back(P);
		}
	}
}

CSectionBezier::~CSectionBezier(void)
{
}

bool CSectionBezier::operator == (const CSection &CompareMe) const
{
	if (CompareMe.GetType() != GetType())
		return false;
	return m_BezierPoints == ((CSectionBezier*)&CompareMe)->m_BezierPoints;
}

CSectionBezier::CSectionBezier(TiXmlElement &Element)
: CSection(Element)
{
	FOR_EACH_TIXMLELEMENT(pPoint, Element, "BezierPoint")
	{
		m_BezierPoints.push_back(valueify<XY>(pPoint->Attribute("value")));
	}
}

void CSectionBezier::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CSection::PopulateTiXmlElement(Element, OutputType);
	vector<XY>::const_iterator itPoint;
	for (itPoint = m_BezierPoints.begin(); itPoint != m_BezierPoints.end(); ++itPoint)
	{
		TiXmlElement Point("BezierPoint");
		Point.SetAttribute("value", stringify(*itPoint));
		Element.InsertEndChild(Point);
	}
}

XY CSectionBezier::GetPoint(double t) const
{
	int iNumSections = m_BezierPoints.size()/3;
	t *= iNumSections;
	int iIndex = int(t);
	t -= iIndex;
	if (iIndex >= iNumSections)
	{
		iIndex = iNumSections-1;
		t = 1;
	}
	XY P1, P2, P3, P4;

	P1 = m_BezierPoints[iIndex*3];
	P2 = m_BezierPoints[iIndex*3+1];
	P3 = m_BezierPoints[iIndex*3+2];
	P4 = m_BezierPoints[(iIndex*3+3)%m_BezierPoints.size()];

	return CalculateBezierPoint(P1, P2, P3, P4, t);
}

string CSectionBezier::GetDefaultName() const {
	return "Bezier(N:" + stringify(m_BezierPoints.size()/3) + ")";
}
