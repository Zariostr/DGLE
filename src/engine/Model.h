/**
\author		Korotkov Andrey aka DRON
\date		20.04.2013 (c)Korotkov Andrey

This file is a part of DGLE project and is distributed
under the terms of the GNU Lesser General Public License.
See "DGLE.h" for more details.
*/

#pragma once

#include "Common.h"
#include "ResourceManager.h"

class CModel: public CInstancedObj, public IModel
{
	struct TMeshWithMat
	{
		IMesh *pMesh;
		IMaterial *pMaterial;

		TMeshWithMat(IMesh *mesh) : pMesh(mesh), pMaterial(NULL) {}
	};

	class CMatSort
	{
		bool _bModelHaveBlendMat;

	public:
		
		CMatSort(bool bModelHaveBlendMat) : _bModelHaveBlendMat(bModelHaveBlendMat) {}

		inline bool operator() (const TMeshWithMat &a, const TMeshWithMat &b) const
		{
			bool is_blend_a = _bModelHaveBlendMat, is_blend_b = _bModelHaveBlendMat;
			E_BLENDING_EFFECT effect;
			if (a.pMaterial) a.pMaterial->GetBlending(is_blend_a, effect);
			if (b.pMaterial) b.pMaterial->GetBlending(is_blend_b, effect);
			return !is_blend_a && is_blend_b;
		}
	};

	TPoint3 _center;
	TVector3 _extents;
	IMaterial *_mat;
	std::vector<TMeshWithMat> _meshes;

	bool _HaveMaterialWithBlending() const;
	void _RecalculateBounds();
	bool _SaveToFile(IFile *pFile);

public:

	CModel(uint uiInstIdx);
	~CModel();

	void AddMesh(IMesh *pMesh, const TPoint3 &stCenter, const TVector3 &stExtents);

	DGLE_RESULT DGLE_API Draw();
	DGLE_RESULT DGLE_API DrawMesh(uint uiMeshIdx);
	DGLE_RESULT DGLE_API GetCenter(TPoint3 &stCenter);
	DGLE_RESULT DGLE_API GetExtents(TVector3 &stExtents);
	DGLE_RESULT DGLE_API MeshesCount(uint &uiCount);
	DGLE_RESULT DGLE_API GetMesh(uint uiMeshIdx, IMesh *&prMesh);
	DGLE_RESULT DGLE_API SetModelMaterial(IMaterial *pMaterial);
	DGLE_RESULT DGLE_API GetModelMaterial(IMaterial *&prMaterial);
	DGLE_RESULT DGLE_API SetMeshMaterial(uint uiMeshIdx, IMaterial *pMaterial);
	DGLE_RESULT DGLE_API GetMeshMaterial(uint uiMeshIdx, IMaterial *&prMaterial);
	DGLE_RESULT DGLE_API AddMesh(IMesh *pMesh);
	DGLE_RESULT DGLE_API RemoveMesh(IMesh *pMesh);
	DGLE_RESULT DGLE_API ReplaceMesh(uint uiMeshIdx, IMesh *pMesh);

	DGLE_RESULT DGLE_API ExecuteCommand(uint uiCmd, TVariant &stVar); // Command with uiCmd == 1 will save mesh to IFile, stVar should contain a pointer to IFile interface.
	DGLE_RESULT DGLE_API ExecuteTextCommand(const char *pcCommand, TVariant &stVar);
	DGLE_RESULT DGLE_API ExecuteTextCommandEx(const char *pcCommand, char *pcResult, uint &uiCharsCount);

	IENGINE_BASE_OBJECT_IMPLEMENTATION(EOT_MODEL)
	IDGLE_BASE_GUID_IMPL(IModel)
	IUNKNOWN_IMPL(INTERFACE_IMPL(IDGLE_Base, INTERFACE_IMPL(IModel, INTERFACE_IMPL(IEngineBaseObject, INTERFACE_IMPL_END))))
};