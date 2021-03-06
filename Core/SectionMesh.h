#pragma once
#include "Mesh.h"
#include "ObjectContainer.h"

namespace TexGen {
	using namespace std;

	/// Abstract base class to create a 2D mesh of a section
	/**
	The resulting mesh is stored in a CMesh where the z coordinate is always 0.
	The derived section mesh should be assigned to a section for meshing.
	*/
	class CLASS_DECLSPEC CSectionMesh {
	public:
		CSectionMesh(void);
		CSectionMesh(TiXmlElement &Element);
		virtual ~CSectionMesh(void);

		/// This is a function to allow copying of derived classes correctly
		/**
		Derived classes should implement this as CSectionMesh* Copy() const { return new CSectionMeshDerived(*this); }
		where CSectionMeshDerived should be replaced by the name of the derived class
		*/
		virtual CSectionMesh* Copy() const = 0;

		/// Derived class should return the class name
		virtual string GetType() const = 0;

		/// Used for saving data to XML
		virtual void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Create a section from TiXmlElement
		static CObjectContainer<CSectionMesh> CreateSectionMesh(TiXmlElement &Element);

		/// Accessor methods
		const CMesh &GetMesh(const vector<XY> &Section) const;
		virtual void SetNumLayers( int iNumLayers ){;}
		virtual int GetNumLayers() {return -1;}

	protected:
		/// Create a mesh out of given list of points representing the edge of the section
		virtual bool CreateMesh(const vector<XY> &Section) const = 0;

		/// Create the mesh if it has not already been created
		bool CreateMeshIfNeeded(const vector<XY> &Section) const;

		/// Used to cache the result of the previous mesh for efficiency
		mutable CMesh m_Mesh;

		/// Store the section used to request a mesh the last time
		/**
		If the given section is the same as given last time then no need to
		remesh, simply re-used the result of last time.
		*/
		mutable vector<XY> m_Section;
	};
}
