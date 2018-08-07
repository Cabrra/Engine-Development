#pragma once


#include "../EDMath/Float4x4.h"

namespace EDUtilities
{

    class GDModelPart
    {
        std::string name;
        int parentIndex;
        EDMath::Float4x4 localTransform;
        std::string formName;

    public:
        GDModelPart(void);
        GDModelPart(std::ifstream &fin);

        const char* GetName(void) const;
        void SetName(const char* _name);

        int GetParentIndex(void) const { return parentIndex; }
        void SetParentIndex(int index){ parentIndex = index; }

        const EDMath::Float4x4 &GetLocalTransform(void) const { return localTransform; }
        void SetLocalTransform(const EDMath::Float4x4 &transform){ localTransform = transform; }

        const char* GetFormName(void) const;
        void SetFormName(const char* _name);

        void Read(std::ifstream &fin);
        void Write(std::ofstream &fout) const;
    };

    class GDModel
    {
        std::string name;
        std::vector<GDModelPart> modelParts;

    public:
        GDModel(void){}
        GDModel( const char* fileName );
        GDModel( std::ifstream &fin );

        const char* GetName(void) const;
        void SetName(const char* _name);

        const std::vector<GDModelPart> &GetModelParts(void) const { return modelParts; }
        void SetModelParts(const std::vector<GDModelPart> &_modelParts);

        void Save(const char* fileName, bool bOverWrite = false) const;
        void Load(const char* fileName);

        void Read(std::ifstream &fin);
        void Write(std::ofstream &fout) const;

		void WriteXML(std::ofstream &fout) const;
		void SaveXML(void) const;

		void OpenXML(const char* fileName);
        //static GDModel* Load(const char* fileName);
        //static GDModel* Load(std::ifstream &fin);
    };
}