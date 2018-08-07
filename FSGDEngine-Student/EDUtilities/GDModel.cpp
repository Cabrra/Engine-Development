
#include "GDModel.h"
#include "../tinyxml/tinyxml.h"
#include "../EDMath/Float4x4.h"
#include "../EDUtilities/ContentManager.h"
using namespace EDUtilities;

using EDMath::Float4x4;
using std::string;
using std::ios_base;
using std::ifstream;
using std::ofstream;
using std::vector;

namespace EDUtilities
{
    GDModelPart::GDModelPart(void)
    {
        parentIndex = -1;
		localTransform.makeIdentity();
    }

    GDModelPart::GDModelPart(ifstream &fin)
    {
        parentIndex = -1;
        localTransform.makeIdentity();
        Read( fin );
    }

    const char* GDModelPart::GetName(void) const
    {
        return name.c_str();
    }

    void GDModelPart::SetName(const char* _name)
    {
        name = _name;
    }

    const char* GDModelPart::GetFormName(void) const
    {
        return formName.c_str();
    }

    void GDModelPart::SetFormName(const char* _name)
    {
        formName = _name;
    }

    void GDModelPart::Read(ifstream &fin)
    {
        int count = 0;
        fin.read( (char*)&count, sizeof(count) );
		char* tempName = new char[count+1];
        tempName[count] = 0;
        fin.read( tempName, sizeof(char)*count );
        name = tempName;
		delete [] tempName;

        fin.read( (char*)&parentIndex, sizeof(parentIndex) );
        //fin.read( (char*)localTransform.ma, sizeof(localTransform) );
        fin.read( (char*)localTransform.m, sizeof(localTransform) );

        fin.read( (char*)&count, sizeof(count) );
		tempName = new char[count+1];
        tempName[count] = 0;
        fin.read( tempName, sizeof(char)*count );
        formName = tempName;
		delete []tempName;
    }

    void GDModelPart::Write(ofstream &fout) const
    {
        unsigned int count = (unsigned int)name.length();
        fout.write( (char*)&count, sizeof(count) );
        fout.write( name.c_str(), count );
        fout.write( (char*)&parentIndex, sizeof(parentIndex) );
//        fout.write( (char*)localTransform.ma, sizeof(localTransform) );
        fout.write( (char*)localTransform.m, sizeof(localTransform) );
        count = (unsigned int)formName.length();
        fout.write( (char*)&count, sizeof(count) );
        fout.write( formName.c_str(), count );		
    }

    // GDModel

    GDModel::GDModel( const char* fileName )
    {
        ifstream fin( fileName, ios_base::in | ios_base::binary );

        if( fin.is_open() )
        {
            Read( fin );
            fin.close();
        }
    }

    GDModel::GDModel( ifstream &fin )
    {
        Read( fin );
    }

    const char* GDModel::GetName(void) const
    {
        return name.c_str();
    }

    void GDModel::SetName(const char* _name)
    {
        name = _name;
    }

    void GDModel::SetModelParts(const vector<GDModelPart> &_modelParts)
    {
        modelParts = _modelParts;
    }

    void GDModel::Save(const char* fileName, bool bOverWrite) const
    {
        ifstream fin(fileName, ios_base::in | ios_base::binary);

        if( fin.bad() || bOverWrite == true )
        {
            fin.close();
            fin.clear();

            ofstream fout( fileName, ios_base::out | ios_base::binary | ios_base::trunc );
            Write( fout );
            fout.close();
        }
    }

    void GDModel::Load(const char* fileName)
    {
        ifstream fin(fileName, ios_base::in | ios_base::binary);

        if( fin.is_open() )
        {
            Read( fin );
            fin.close();
        }
    }

    void GDModel::Read(ifstream &fin)
    {
        int count = 0;
        fin.read( (char*)&count, sizeof(count) );
		char* tempName = new char[count+1];
        tempName[count] = 0;
        fin.read( tempName, sizeof(char)*count );
        name = tempName;
		delete []tempName;

        fin.read( (char*)&count, sizeof(count) );

        modelParts.resize(count);
        for( int index = 0; index < count; ++index )
            modelParts[index].Read( fin );
	}

    void GDModel::Write(ofstream &fout) const
    {
        size_t count = name.length();
        fout.write( (char*)&count, sizeof(count) );
        fout.write( name.c_str(), count );

        count = modelParts.size();
        fout.write( (char*)&count, sizeof(count) );

        for( size_t index = 0; index < count; ++index )
            modelParts[index].Write(fout);
    }

	void GDModel::WriteXML(ofstream &fout) const
	{
		char buffer[128];

		fout << "<GDModel name=\"" << name.c_str() << "\">\n";
		sprintf_s( buffer, "%d", modelParts.size() );
		fout << "\t<GDModelPart_List count=\"" << buffer << "\">\n";
		size_t vSize = modelParts.size();
		for( unsigned int i = 0; i < vSize; ++i)
		{
			fout << "\t\t<GDModelPart name=\"" << modelParts[i].GetName() << "\">\n";
			sprintf_s( buffer, "%d", modelParts[i].GetParentIndex() );
			fout << "\t\t\t<ParentIndex>" << buffer << "</ParentIndex>\n";
			fout << "\t\t\t<RenderFormName>" << modelParts[i].GetFormName() << "</RenderFormName>\n";
			fout << "\t\t\t<LocalTransform>\n";
			fout << "\t\t\t\t<XAxis>\n";
			sprintf_s( buffer, "<x>%f</x><y>%f</y><z>%f</z><w>%f</w>\n", 
					 modelParts[i].GetLocalTransform().m[0][0],
					 modelParts[i].GetLocalTransform().m[0][1],
					 modelParts[i].GetLocalTransform().m[0][2],
					 modelParts[i].GetLocalTransform().m[0][3] );
			fout << buffer;
			fout << "\t\t\t\t</XAxis>\n";
			fout << "\t\t\t\t<YAxis>\n";
			sprintf_s( buffer, "<x>%f</x><y>%f</y><z>%f</z><w>%f</w>\n", 
					 modelParts[i].GetLocalTransform().m[1][0],
					 modelParts[i].GetLocalTransform().m[1][1],
					 modelParts[i].GetLocalTransform().m[1][2],
					 modelParts[i].GetLocalTransform().m[1][3] );
			fout << buffer;
			fout << "\t\t\t\t</YAxis>\n";
			fout << "\t\t\t\t<ZAxis>\n";
			sprintf_s( buffer, "<x>%f</x><y>%f</y><z>%f</z><w>%f</w>\n", 
					 modelParts[i].GetLocalTransform().m[2][0],
					 modelParts[i].GetLocalTransform().m[2][1],
					 modelParts[i].GetLocalTransform().m[2][2],
					 modelParts[i].GetLocalTransform().m[2][3]);
			fout << buffer;
			fout << "\t\t\t\t</ZAxis>\n";
			fout << "\t\t\t\t<WAxis>\n";
			sprintf_s( buffer, "<x>%f</x><y>%f</y><z>%f</z><w>%f</w>\n", 
					 modelParts[i].GetLocalTransform().m[3][0],
					 modelParts[i].GetLocalTransform().m[3][1],
					 modelParts[i].GetLocalTransform().m[3][2],
					 modelParts[i].GetLocalTransform().m[3][3] );
			fout << buffer;
			fout << "\t\t\t\t</WAxis>\n";
			fout << "\t\t\t</LocalTransform>\n";
			fout << "\t\t</GDModelPart>\n";
		}
		fout << "\t</GDModelPart_List>\n";
		fout << "</GDModel>\n";
	}

	void GDModel::OpenXML(const char* fileName)
	{
		TiXmlDocument doc(fileName);

		if(!doc.LoadFile())
			return;

		TiXmlHandle hDoc(&doc);
		TiXmlElement* elementPtr;
		TiXmlHandle hRoot(0);
		
		elementPtr = hDoc.FirstChildElement().Element();
		if( !elementPtr )
			return;

		string rootName = elementPtr->Value();

		if( strcmp(rootName.c_str(), "GDModel") != 0 )
			return;

		hRoot = TiXmlHandle(elementPtr);

		name = elementPtr->Attribute("name");

		elementPtr = hRoot.FirstChild( "GDModelPart_List" ).Element();

		if( elementPtr != 0 )
		{
			unsigned int index = 0;
			modelParts.resize( atoi(elementPtr->Attribute("count")) );

			TiXmlElement* childPtr = elementPtr->FirstChildElement("GDModelPart");

			while( childPtr != NULL )
			{
				if( index >= modelParts.size() )
					break;

				modelParts[index].SetName( childPtr->Attribute("name") );

				elementPtr = childPtr->FirstChildElement("ParentIndex");
				if( elementPtr != 0 )
					modelParts[index].SetParentIndex( atoi( elementPtr->GetText() ) );

				elementPtr = childPtr->FirstChildElement("RenderFormName");

				if( elementPtr != 0 )
					modelParts[index].SetFormName( elementPtr->GetText() );

				elementPtr = childPtr->FirstChildElement("LocalTransform");

				TiXmlElement* pAxis;
				TiXmlElement* pComponent;
				
				Float4x4 localTransform;
				localTransform.makeIdentity();

				if( elementPtr != 0 )
				{
					const char* axes[] = { "XAxis", "YAxis", "ZAxis", "WAxis" };
					const char* components[] = { "x", "y", "z", "w" };

					for(int axis = 0; axis < 4; ++axis)
					{
						pAxis = elementPtr->FirstChildElement(axes[axis]);
						if( pAxis != NULL )
						{
							for( int component = 0; component < 4; ++component )
							{
								pComponent = pAxis->FirstChildElement(components[component]);
								if( pComponent != NULL )
								{
									localTransform.m[axis][component] = (float)atof(pComponent->GetText());
								}
							}
						}
					}
				}
				
				modelParts[index].SetLocalTransform(localTransform);

				++index;
				childPtr = childPtr->NextSiblingElement("GDModelPart");
			}
		}
	}

	void GDModel::SaveXML(void) const
	{
		string fileName = ContentManager::theContentPath + "/gdmdl_";
		fileName += name;
		fileName += ".xml";

		ofstream fout( fileName.c_str(), ios_base::out | ios_base::binary | ios_base::trunc );
		
		fout << "<?xml version=\"1.0\"?>\n";
		
		WriteXML( fout );

		fout.close();
	}
}