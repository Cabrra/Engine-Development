#include "PakFile.h"

namespace EDUtilities
{
	PakFile::PakFile()
	{
	}

	PakFile::~PakFile()
	{
		ClearPak();
	}

	PakFile* PakFile::GetInstance()
	{
		static PakFile pak_file;
		return &pak_file;
	}

	void PakFile::FindAssetName(const char* _raw_name, std::string& _final_name)
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//FindAssetNameSolution(_raw_name, _final_name);
		int i, index;
		i = index = 0;
		while (_raw_name[i] != '\0')
		{
			if (_raw_name[i] == '/')
				index = i;
			i++;
		}
		_final_name.append("\\");
		_final_name.append(_raw_name+index);
	
	}

	bool PakFile::LoadAsset(const char* _asset_name, char** _buffer, unsigned int& _file_size)
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//return LoadAssetSolution(_asset_name, _buffer, _file_size);

		if (IsInPak(_asset_name))
		{
			std::string help;
			FindAssetName(_asset_name, help);
			_file_size = pak_directory[help].file_length;

			*_buffer = new char[_file_size];

			pak_file.seekg(pak_directory[help].file_offset, std::ios_base::beg);
			pak_file.read((char*)*_buffer, pak_directory[help].file_length);

			return true;
		}
		else
			return false;
	    
		return false;
	}

	void PakFile::ClearPak()
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//ClearPakSolution();
		pak_directory.clear();

		//close stream
		pak_file.clear();
		pak_file.close();
	
	}

	bool PakFile::IsInPak(const char* _file_name)
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//return IsInPakSolution(_file_name);

		std::string help;
		FindAssetName(_file_name, help);
		if (pak_directory.find(help) != pak_directory.end())
			return true;
		else
			return false;
	}

	void PakFile::LoadDirectory(const char* _pak_file)
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//LoadDirectorySolution(_pak_file);
		pak_file.open(_pak_file, std::ifstream::binary);
		if (pak_file.is_open())
		{
			pak_file.seekg(0, std::ios_base::beg);
			
			MainHeaderPakf main;
			pak_file.read((char*)&main, sizeof(MainHeaderPakf));

			pak_file.seekg(main.directory_offset, std::ios_base::beg);

			int index = main.directory_length / sizeof(FileHeaderPak);

			for (int i = 0; i < index; i++)
			{
				FileHeaderPak help;	
				pak_file.read((char*)&help, sizeof(FileHeaderPak));
				pak_directory[help.filename] = help;
			}
		}
	}

	void PakFile::ExtractAsset(const char* _asset_name)
	{
		// TODO-STUDENT - comment out solution version and write your own implementation
		//ExtractAssetSolution(_asset_name);

		std::ofstream bufferB;
		bufferB.open(_asset_name, std::ios_base::binary | std::ios_base::trunc);

		std::string mine;
		if (IsInPak(_asset_name))
		{
			pak_file.seekg(0, std::ios_base::beg);
			FindAssetName(_asset_name, mine);
			pak_file.seekg(pak_directory[mine].file_offset, std::ios_base::beg);
			int size = pak_directory[mine].file_length / 64000;
	
			char helpbuffer[64000];
			for (int i = 0; i < size; i++)
			{
				pak_file.read((char*)&helpbuffer, 64000);
				bufferB.write((char*)&helpbuffer, 64000);
			}

			size = pak_directory[mine].file_length % 64000;

			pak_file.read((char*)&helpbuffer, size);
			bufferB.write((char*)&helpbuffer, size);

			bufferB.close();
		}
	}
}
