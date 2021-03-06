#include "stdafx.h"
#include "FibreDistribution1DQuad.h"

using namespace TexGen;

CFibreDistribution1DQuad::CFibreDistribution1DQuad(double dDropOff)
: m_dDropOff(dDropOff)
{
}

CFibreDistribution1DQuad::CFibreDistribution1DQuad(TiXmlElement &Element)
: CFibreDistribution(Element)
{
	Element.Attribute("DropOff", &m_dDropOff);
}

void CFibreDistribution1DQuad::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CFibreDistribution::PopulateTiXmlElement(Element, OutputType);
	Element.SetDoubleAttribute("DropOff", m_dDropOff);
}

double CFibreDistribution1DQuad::ComputeMaxX(const vector<XY> &Section) const
{
	double dMaxX = 0;
	for(unsigned int i=0;i<Section.size();++i)
	{
		dMaxX = max(Section[i].x, dMaxX);
	}
	return dMaxX;
}

double CFibreDistribution1DQuad::IntegrateDistribution(const vector<XY> &Section, double dMaxX) const
{
	double Integral = 0;
	for(unsigned int i=0;i<Section.size();++i)
	{
		double x_0 = Section[i].x, x_1 = Section[(i+1)%Section.size()].x;
		double dx = abs(x_1 - x_0);	// TODO: Probably shouldn't be taking the absoulte value here
		double avy = (abs(Section[(i+1)%Section.size()].y) + abs(Section[i].y))*0.5;
		double dA = dx*avy;
		double avf = (Distribution(1, m_dDropOff, x_1, dMaxX) + Distribution(1, m_dDropOff, x_0, dMaxX)) * 0.5;
		Integral += dA*avf;	
	}
	// TODO: Take the absolute value at the end
	return Integral;
}

double CFibreDistribution1DQuad::GetVolumeFraction(const vector<XY> &Section, double dFibreArea, XY Location) const
{
	// compute the max x direction
	double dMaxX = ComputeMaxX(Section);
	double dScale = IntegrateDistribution(Section, dMaxX);
	dScale = dFibreArea/dScale;
	if(dScale>0.86||dScale<0)
		TGERROR("Warning: Volume fraction is not realistic " << dScale);

	return Distribution(dScale, m_dDropOff*dScale, Location.x, dMaxX);
}

double CFibreDistribution1DQuad::Distribution(double max, double min, double x, double dMaxX) const
{
	double xScaled = x / dMaxX;
	return min + (max - min)*(1-xScaled*xScaled);
}
