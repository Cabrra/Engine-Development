#include "precompiled.h"
#include "RenderMesh.h"
//#include "Renderer.h"
#include "VertexBufferManager.h"
#include "IndexBuffer.h"
#include "../EDMemoryManager/MMAllocator.h"
#include "../EDUtilities/GDMesh.h"
#include "../EDUtilities/InternalOutput.h"
#include "../EDUtilities/ContentManager.h"
#include "../EDUtilities/GDMeshStream.h"

using namespace EDUtilities;
#include "atlstr.h"

using std::nothrow;
using std::string;
using namespace DirectX;

namespace EDRendererD3D
{
	RenderMesh::RenderMesh()
	{
		numVertices = -1;
		startVertex = 0;
		startIndex = 0;
	}

	RenderMesh::~RenderMesh()
	{
	}

	RenderMesh &RenderMesh::operator=(const RenderMesh & rhs)
	{
		if(this != &rhs)
		{
			numPrimitives = rhs.numPrimitives;
			//			primitiveType = rhs.primitiveType;
			startVertex = rhs.startVertex;
			startIndex = rhs.startIndex;
			numVertices = rhs.numVertices;

			boundingSphere = rhs.boundingSphere;
			boundingAabb = rhs.boundingAabb;
		}
		return *this;
	}

	RenderMesh &RenderMesh::operator=(RenderMesh &&rhs)
	{
		if(this != &rhs)
		{
			numPrimitives = std::move(rhs.numPrimitives);
			//			primitiveType = std::move(rhs.primitiveType);
			startVertex = std::move(rhs.startVertex);
			startIndex = std::move(rhs.startIndex);
			numVertices = std::move(rhs.numVertices);

			boundingSphere = std::move(rhs.boundingSphere);
			boundingAabb = std::move(rhs.boundingAabb);

		}
		return *this;
	}

	RenderMesh *RenderMesh::Load(const char* pInFileName, const char* pVertexFormat)
	{
		string temp = ContentManager::theContentPath;
		temp += pInFileName;
		const char* fileName = temp.c_str();

		if( pVertexFormat == 0 )
			return 0;

		if( fileName == 0 )
			return 0;

		InternalOutput::GetReference() << "Loading " << fileName << "...\n";

		ContentHandle<GDMeshStreamBinary> meshHandle = ContentManager::Load<GDMeshStreamBinary>(fileName);
		RenderMesh *resultPtr = new RenderMesh;;
		if(!LoadContent(resultPtr, fileName, meshHandle, pVertexFormat))
		{
			delete resultPtr;
			resultPtr = 0;
		}
		return resultPtr;

	}

	bool RenderMesh::LoadXML(RenderMesh* resultPtr, const char *xmlFileName, 
		const char *pVertexFormat)
	{
		string temp = ContentManager::theContentPath;
		temp += xmlFileName;
		const char* xmlFileNamePtr = temp.c_str();

		if( pVertexFormat == 0 )
			return 0;

		if( xmlFileNamePtr == 0 )
			return 0;

		InternalOutput::GetReference() << "Loading " << xmlFileNamePtr << "...\n";

		ContentHandle<GDMeshStreamXML> meshHandle = ContentManager::LoadXML<GDMeshStreamXML>(xmlFileName);

		return LoadContent(resultPtr, xmlFileNamePtr, meshHandle, pVertexFormat);
	}

	RenderMesh *RenderMesh::LoadXML(const char *xmlFileName, 
		const char *pVertexFormat)
	{
		RenderMesh* resultPtr = new RenderMesh;
		if(!LoadXML(resultPtr, xmlFileName, pVertexFormat))
		{
			delete resultPtr;
			resultPtr = 0;
		}

		return resultPtr;
	}

	RenderMesh RenderMesh::LoadXMLObject(const char *xmlFileName, 
		const char *pVertexFormat)
	{
		RenderMesh result;
		LoadXML(&result, xmlFileName, pVertexFormat);

		return result;
	}

	template<typename T>
	bool RenderMesh::LoadContent(RenderMesh* resultPtr, const char* xmlFileNamePtr, 
		ContentHandle<T> &meshHandle, const char* pVertexFormat)
	{
		if( meshHandle.GetContent() == 0 )
		{
			InternalOutput::GetReference().Error(
				"Failed to load %s, does the file exist?\n", xmlFileNamePtr);
			return 0;
		}

		ContentHandle<GDAttribute<Point>> pointHandle = meshHandle.GetContent()->ReadAttribute<Point>();
		if( pointHandle.GetContent() == 0 )
		{
			InternalOutput::GetReference().Error(
				"Failed to load %s, does the file define points properly?\n",
				xmlFileNamePtr);
			return 0;
		}

		size_t numVerts = pointHandle.GetContent()->size();
		if( numVerts == 0 )
		{
			InternalOutput::GetReference().Error(
				"Failed to load %s, does the file define points properly?\n",
				xmlFileNamePtr);
			return 0;
		}

		ContentHandle<GDAttribute<TriVertIndices>> indicesHandle = meshHandle.GetContent()->GetIndicesHandle();
		if( indicesHandle.GetContent() == 0 )
		{
			InternalOutput::GetReference().Error(
				"Failed to load %s, does the file define indices properly?\n",
				xmlFileNamePtr);
			return 0;
		}

		// Load vertices for shadow casting only
		VERTEX_POS *pShadowVerts = new VERTEX_POS[numVerts];

		int interIndex = 0;
		for(size_t i = 0; i < numVerts; ++i)
		{
			pShadowVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
			pShadowVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
			pShadowVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;
		}

		if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POS] ) == 0 )
		{
			VERTEX_POS *pInterVerts = new VERTEX_POS[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;
			}


			resultPtr->CreateIndexedMesh( pInterVerts, pShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));

			delete [] pInterVerts;
			delete [] pShadowVerts;
			return true;
		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSCOLOR] ) == 0 )
		{
			//assert(0);
			ContentHandle<GDAttribute<Color>> colorHandle = meshHandle.GetContent()->ReadAttribute<Color>();

			VERTEX_POSCOLOR *pInterVerts = new VERTEX_POSCOLOR[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;

				if( colorHandle.GetContent() == 0 )
					pInterVerts[i].color = XMFLOAT4( 1, 1, 1, 1 );
				else
				{
					pInterVerts[i].color.x = colorHandle.GetContent()->operator[](i).Red;
					pInterVerts[i].color.y = colorHandle.GetContent()->operator[](i).Green;
					pInterVerts[i].color.z = colorHandle.GetContent()->operator[](i).Blue;
					pInterVerts[i].color.w = colorHandle.GetContent()->operator[](i).Alpha;
				}
			}


			resultPtr->CreateIndexedMesh( pInterVerts, pShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));

			delete []pInterVerts;
			delete [] pShadowVerts;

			return true;

		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSTEX] ) == 0 )
		{
			ContentHandle<GDAttribute<UV>> texCoordHandle = meshHandle.GetContent()->ReadAttribute<UV>("map1");

			if( texCoordHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing texture coordinates.\n",
					xmlFileNamePtr);
				return 0;
			}

			VERTEX_POSTEX *pInterVerts = new VERTEX_POSTEX[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;

				pInterVerts[i].texcoord.x = texCoordHandle.GetContent()->operator[](i).u;
				pInterVerts[i].texcoord.y = texCoordHandle.GetContent()->operator[](i).v;
			}

			// CreateIndexedMesh()...

			resultPtr->CreateIndexedMesh( pInterVerts, pShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));

			delete[] pInterVerts;
			delete [] pShadowVerts;

			return true;
		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSNORMTEX] ) == 0 )
		{
			ContentHandle<GDAttribute<Normal>> normalHandle = meshHandle.GetContent()->ReadAttribute<Normal>();

			if( normalHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing normals.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<UV>> texCoordHandle = meshHandle.GetContent()->ReadAttribute<UV>("map1");

			if( texCoordHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing texture coordinates.\n",
					xmlFileNamePtr);
				return 0;
			}

			VERTEX_POSNORMTEX *pInterVerts = new VERTEX_POSNORMTEX[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;

				pInterVerts[i].normal.x = normalHandle.GetContent()->operator[](i).x;
				pInterVerts[i].normal.y = normalHandle.GetContent()->operator[](i).y;
				pInterVerts[i].normal.z = normalHandle.GetContent()->operator[](i).z;

				pInterVerts[i].texcoord.x = texCoordHandle.GetContent()->operator[](i).u;
				pInterVerts[i].texcoord.y = 1.0f - texCoordHandle.GetContent()->operator[](i).v;
			}


			resultPtr->CreateIndexedMesh( pInterVerts, pShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));

			delete []pInterVerts;
			delete [] pShadowVerts;

			return true;
		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSNORMTANTEX] ) == 0 )
		{
			ContentHandle<GDAttribute<Normal>> normalHandle = meshHandle.GetContent()->ReadAttribute<Normal>();

			if( normalHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing normals.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Tangent>> tangentHandle = meshHandle.GetContent()->ReadAttribute<Tangent>();

			if( tangentHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing tangents.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Bitangent>> bitangentHandle = meshHandle.GetContent()->ReadAttribute<Bitangent>();

			if( bitangentHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing bitangents.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<UV>> texCoordHandle = meshHandle.GetContent()->ReadAttribute<UV>("map1");

			if( texCoordHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing texture coordinates.\n",
					xmlFileNamePtr);
				return 0;
			}

			VERTEX_POSNORMTANTEX *pInterVerts = new VERTEX_POSNORMTANTEX[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;

				pInterVerts[i].normal.x = normalHandle.GetContent()->operator[](i).x;
				pInterVerts[i].normal.y = normalHandle.GetContent()->operator[](i).y;
				pInterVerts[i].normal.z = normalHandle.GetContent()->operator[](i).z;

				pInterVerts[i].tangent.x = tangentHandle.GetContent()->operator[](i).x;
				pInterVerts[i].tangent.y = tangentHandle.GetContent()->operator[](i).y;
				pInterVerts[i].tangent.z = tangentHandle.GetContent()->operator[](i).z;

				EDMath::Float3 perp;
				CrossProduct(perp, tangentHandle.GetContent()->operator[](i), 
					bitangentHandle.GetContent()->operator[](i));

				pInterVerts[i].determinant = DotProduct( perp, 
					normalHandle.GetContent()->operator[](i) );

				pInterVerts[i].texcoord.x = texCoordHandle.GetContent()->operator[](i).u;
				pInterVerts[i].texcoord.y = 1.0f - texCoordHandle.GetContent()->operator[](i).v;
			}

			resultPtr->CreateIndexedMesh( pInterVerts, pShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));
			delete []pInterVerts;
			delete [] pShadowVerts;

			return true;
		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSBONEWEIGHTNORMTEX] ) == 0 )
		{
			ContentHandle<GDAttribute<Normal>> normalHandle = 
				meshHandle.GetContent()->ReadAttribute<Normal>();

			if( normalHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing normals.\n",
					xmlFileNamePtr);
				return 0;
			}

			//ContentHandle<GDAttribute<Tangent>> tangentHandle =
			//	meshHandle.GetContent()->ReadAttribute<Tangent>();

			//if( tangentHandle.GetContent() == 0 )
			//{
			//	InternalOutput::GetReference().Error(
			//		"Failed to load %s, missing tangents.\n",
			//		xmlFileNamePtr);
			//	return 0;
			//}

			ContentHandle<GDAttribute<UV>> texCoordHandle = 
				meshHandle.GetContent()->ReadAttribute<UV>("map1");

			if( texCoordHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing texture coordinates.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<JointIndices>> jointsHandle = 
				meshHandle.GetContent()->ReadAttribute<JointIndices>();

			if( jointsHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing joint indices.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Weights>> weightsHandle = 
				meshHandle.GetContent()->ReadAttribute<Weights>();

			if( weightsHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing vertex weights.\n",
					xmlFileNamePtr);
				return 0;
			}

			VERTEX_POSBONEWEIGHTNORMTEX *pInterVerts = new VERTEX_POSBONEWEIGHTNORMTEX[numVerts];
			VERTEX_POSBONEWEIGHT *animatedShadowVerts = new VERTEX_POSBONEWEIGHT[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				animatedShadowVerts[i].position.x = pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				animatedShadowVerts[i].position.y = pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				animatedShadowVerts[i].position.z = pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;
												    						  
				animatedShadowVerts[i].bone.x	  = pInterVerts[i].bone.x	 = jointsHandle.GetContent()->operator[](i).joint0;
				animatedShadowVerts[i].bone.y	  = pInterVerts[i].bone.y	 = jointsHandle.GetContent()->operator[](i).joint1;
				animatedShadowVerts[i].bone.z	  = pInterVerts[i].bone.z	 = jointsHandle.GetContent()->operator[](i).joint2;
				animatedShadowVerts[i].bone.w	  = pInterVerts[i].bone.w	 = jointsHandle.GetContent()->operator[](i).joint3;
												    						  
				animatedShadowVerts[i].weights.x  = pInterVerts[i].weights.x = weightsHandle.GetContent()->operator[](i).weight0;
				animatedShadowVerts[i].weights.y  = pInterVerts[i].weights.y = weightsHandle.GetContent()->operator[](i).weight1;
				animatedShadowVerts[i].weights.z  = pInterVerts[i].weights.z = weightsHandle.GetContent()->operator[](i).weight2;
				animatedShadowVerts[i].weights.w  = pInterVerts[i].weights.w = weightsHandle.GetContent()->operator[](i).weight3;

				pInterVerts[i].normal.x = normalHandle.GetContent()->operator[](i).x;
				pInterVerts[i].normal.y = normalHandle.GetContent()->operator[](i).y;
				pInterVerts[i].normal.z = normalHandle.GetContent()->operator[](i).z;

				pInterVerts[i].texcoord.x = texCoordHandle.GetContent()->operator[](i).u;
				pInterVerts[i].texcoord.y = 1.0f - texCoordHandle.GetContent()->operator[](i).v;
			}

			resultPtr->CreateIndexedMesh( pInterVerts, animatedShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), 
				static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));
			delete [] pInterVerts;
			delete [] pShadowVerts;
			delete [] animatedShadowVerts;

			return true;
		}
		else if( strcmp( pVertexFormat, VertexFormatString[eVERTEX_POSBONEWEIGHTNORMTANTEX] ) == 0 )
		{
			ContentHandle<GDAttribute<Normal>> normalHandle = 
				meshHandle.GetContent()->ReadAttribute<Normal>();

			if( normalHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing normals.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Tangent>> tangentHandle =
				meshHandle.GetContent()->ReadAttribute<Tangent>();

			if( tangentHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing tangents.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Bitangent>> bitangentHandle =
				meshHandle.GetContent()->ReadAttribute<Bitangent>();

			if( bitangentHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing bitangents.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<UV>> texCoordHandle = 
				meshHandle.GetContent()->ReadAttribute<UV>("map1");

			if( texCoordHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing texture coordinates.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<JointIndices>> jointsHandle = 
				meshHandle.GetContent()->ReadAttribute<JointIndices>();

			if( jointsHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing joint indices.\n",
					xmlFileNamePtr);
				return 0;
			}

			ContentHandle<GDAttribute<Weights>> weightsHandle = 
				meshHandle.GetContent()->ReadAttribute<Weights>();

			if( weightsHandle.GetContent() == 0 )
			{
				InternalOutput::GetReference().Error(
					"Failed to load %s, missing vertex weights.\n",
					xmlFileNamePtr);
				return 0;
			}

			VERTEX_POSBONEWEIGHTNORMTANTEX *pInterVerts = new VERTEX_POSBONEWEIGHTNORMTANTEX[numVerts];
			VERTEX_POSBONEWEIGHT *animatedShadowVerts = new VERTEX_POSBONEWEIGHT[numVerts];

			int interIndex = 0;
			for(size_t i = 0; i < numVerts; ++i)
			{
				animatedShadowVerts[i].position.x = pInterVerts[i].position.x = pointHandle.GetContent()->operator[](i).x;
				animatedShadowVerts[i].position.y = pInterVerts[i].position.y = pointHandle.GetContent()->operator[](i).y;
				animatedShadowVerts[i].position.z = pInterVerts[i].position.z = pointHandle.GetContent()->operator[](i).z;
												    
				animatedShadowVerts[i].bone.x	  = pInterVerts[i].bone.x = jointsHandle.GetContent()->operator[](i).joint0;
				animatedShadowVerts[i].bone.y	  = pInterVerts[i].bone.y = jointsHandle.GetContent()->operator[](i).joint1;
				animatedShadowVerts[i].bone.z	  = pInterVerts[i].bone.z = jointsHandle.GetContent()->operator[](i).joint2;
				animatedShadowVerts[i].bone.w	  = pInterVerts[i].bone.w = jointsHandle.GetContent()->operator[](i).joint3;
												    
				animatedShadowVerts[i].weights.x  = pInterVerts[i].weights.x = weightsHandle.GetContent()->operator[](i).weight0;
				animatedShadowVerts[i].weights.y  = pInterVerts[i].weights.y = weightsHandle.GetContent()->operator[](i).weight1;
				animatedShadowVerts[i].weights.z  = pInterVerts[i].weights.z = weightsHandle.GetContent()->operator[](i).weight2;
				animatedShadowVerts[i].weights.w  = pInterVerts[i].weights.w = weightsHandle.GetContent()->operator[](i).weight3;

				pInterVerts[i].normal.x = normalHandle.GetContent()->operator[](i).x;
				pInterVerts[i].normal.y = normalHandle.GetContent()->operator[](i).y;
				pInterVerts[i].normal.z = normalHandle.GetContent()->operator[](i).z;

				pInterVerts[i].tangent.x = tangentHandle.GetContent()->operator[](i).x;
				pInterVerts[i].tangent.y = tangentHandle.GetContent()->operator[](i).y;
				pInterVerts[i].tangent.z = tangentHandle.GetContent()->operator[](i).z;

				/// Calculate determinant of tangent space matrix to be used to flip mirrored normals
				EDMath::Float3 perp;
				CrossProduct(perp, tangentHandle.GetContent()->operator[](i), 
					bitangentHandle.GetContent()->operator[](i));

				pInterVerts[i].determinant = DotProduct( perp, 
					normalHandle.GetContent()->operator[](i) );

				pInterVerts[i].texcoord.x = texCoordHandle.GetContent()->operator[](i).u;
				pInterVerts[i].texcoord.y = 1.0f - texCoordHandle.GetContent()->operator[](i).v;
			}

			resultPtr->CreateIndexedMesh( pInterVerts, animatedShadowVerts, static_cast<unsigned int >(numVerts), 
				static_cast<unsigned int *>(indicesHandle.GetContent()->operator[](0).indices), 
				static_cast<unsigned int >(indicesHandle.GetContent()->size()*3 ));
			delete []pInterVerts;
			delete [] pShadowVerts;
			delete [] animatedShadowVerts;
			return true;
		}
		delete [] pShadowVerts;

		return false;
	}

}