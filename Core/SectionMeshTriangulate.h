#pragma once
#include "SectionMesh.h"

namespace TexGen { 
	using namespace std;

	/// Creates a mesh of a section using open source package triangle http://www.cs.cmu.edu/~quake/triangle.html
	class CLASS_DECLSPEC CSectionMeshTriangulate : public CSectionMesh {
	public:
		/// Set the mesh quality parameters
		/**
		\param dMinAngle Minimum angle between two triangle edges generated by the triangulator.
						Increasing is value will result in a higher quality mesh but more elements.
						If this value is too high the mesh generator will hang.
		\param dMaxArea Maximum size of triangle generated by the triangulator. Decreasing this value
						will result in more elements in the mesh. The value is normalised with the area
						of the total section. Thus a value of 0.1 will not create any triangles with an
						area greater than 0.1 times the the area of the section.
		*/
		CSectionMeshTriangulate(double dMinAngle = 20, double dMaxArea = 1);
		CSectionMeshTriangulate(TiXmlElement &Element);
		~CSectionMeshTriangulate(void) {}

		CSectionMesh* Copy() const { return new CSectionMeshTriangulate(*this); }
		string GetType() const { return "CSectionMeshTriangulate"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Function to mesh 2d list of points that form a closed loop. This function does not use triangle
		/// instead it used homegrown code.
		static CMesh GetSimpleMesh(const vector<XY> &Section);

	protected:
		bool CreateMesh(const vector<XY> &Section) const;

		double m_dMinAngle;
		double m_dMaxArea;
	};
}
