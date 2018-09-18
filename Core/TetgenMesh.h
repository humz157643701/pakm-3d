#pragma once
#include <list>
#include "Mesh.h"
#include "tetgen.h"

namespace TexGen {
	/** Class which tetrahedralizes a textile using Tetgen software http://tetgen.berlios.de/
		The surface mesh and domain are used as tetgen input, the domain boundaries first being 
		triangulated such that opposite boundaries have the same input mesh. (This assumes that
		the domain is specified to be a whole number of textile repeats).
	*/

	using namespace std;

	/// Structure which contains information for transformation from 3D to 2D plane
	struct PLANEPARAMS {
		XYZ XAxis;
		XYZ YAxis;
		XYZ RefPoint;
		XYZ Normal;
	};

	class CLASS_DECLSPEC CTetgenMesh {
	public:
		CTetgenMesh( double Seed );
		virtual ~CTetgenMesh(void);
		/// Save a textile as a tetrahedralized mesh using Tetgen
		/**
		\param Textile Textile to be meshed
		\param Filename for Tetgen output files
		\param Parameters tetgen flags to be applied during tetrahedralization
		\param bPeriodic If set true, opposite faces of mesh will be replicated
		*/
		void SaveTetgenMesh(CTextile &Textile, string OutputFilename, string Parameters, bool bPeriodic );

	protected:
		///	Mesh used to store node points and elements
		CMesh			m_Mesh;
		/// Tetgen input and output structures
		tetgenio		m_in, m_out;
		/// Seed used for calculating boundary edge points
		double			m_Seed;

		/// Triangulate the domain faces
		bool Triangulate( vector< vector<XY> > &PolygonPoints, CMesh& OutputMesh, PLANEPARAMS& ConvertRef );

		/// Convert points on one domain surface to local 2D points
		bool ConvertDomainPointsTo2D( const list<int> &QuadIndices, CMesh& DomainMesh, vector<XY>& Points2D, PLANEPARAMS& ConvertRef );
		/// Convert local 2D coordinates to global 3D coordinates
		void Convert2DTo3DCoordinates( vector<XY>& Points2D, vector<XYZ>& Points3D, PLANEPARAMS& ConvertRef );
		/// Convert global 3D coordinates to local 2D coordinates
		void Convert3DTo2DCoordinates( vector<XYZ>& Points3D, PLANEPARAMS& ConvertRef, vector<XY>& Points2D );
		/// Calculates seed points along domain edge
		void SeedSides( vector<XY>& Points );
		/// Offsets points in mesh by given distance in direction of normal
		void OffsetMeshPoints( CMesh& Mesh, XYZ& Normal, double dDist );
		/// Save tetgenio data to Abaqus export file
		void SaveToAbaqus( string Filename, CTextile &Textile );
	};
}
