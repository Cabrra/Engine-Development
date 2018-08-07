#include "PythonModule.h"
#include "InternalOutput.h"

namespace EDUtilities
{
	void PythonModule::Initialize()
	{
		// internal output must be created before python inits
		InternalOutput::GetReference();

		// tells python where to look for its 'lib' files
		Py_NoSiteFlag=1;
		Py_SetPythonHome(L"../Python/.");
		Py_Initialize();
	
		// tell python where to search for modules
		std::wstring new_path = Py_GetPath();
		new_path += L";../Scripts";
		PySys_SetPath(new_path.c_str());
	}

	void PythonModule::Shutdown()
	{
		Py_Finalize();
	}

	PythonModule::PythonModule(string _name)
	{
		// TODO - comment this out and write your own solution
		LoadModule(_name);


	}

	bool PythonModule::LoadModule(string _filename)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//bool success = false;
		//success = LoadModuleSolution(_filename);
		//return success;

		module_name = _filename;
		module = PyImport_ImportModule(_filename.c_str());
		if (module != nullptr)
		{
			dictionary = PyModule_GetDict(module);
			return true;
		}
		else
		{
			PyErr_Print();
			return false;
		}
	}

	void PythonModule::UnloadModule()
	{
		// TODO-STUDENT - comment this out and write your own solution
		//UnloadModuleSolution();

		if (module != nullptr)
			Py_DECREF(module);
		module = nullptr;
		dictionary = nullptr;
		module_name.clear();
	}

	void PythonModule::CallFunction(string _name)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//CallFunctionSolution(_name);

		PyObject* aux = PyDict_GetItemString(dictionary, _name.c_str());
		if (aux != nullptr)
			PyObject_CallObject(aux, nullptr);
	}

void PythonModule::SetValue(string _name, int _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyDict_SetItemString(dictionary, _name.c_str(), PyLong_FromLong((long)_value));
	}
	
	void PythonModule::SetValue(string _name, float _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyObject* real = PyFloat_FromDouble(_value);
		if (real)
			PyDict_SetItemString(dictionary, _name.c_str(), real);
	}

	void PythonModule::SetValue(string _name, string _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyDict_SetItemString(dictionary, _name.c_str(), PyUnicode_FromString(_value.c_str()));
	}

	void PythonModule::SetValue(string _name, bool _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyDict_SetItemString(dictionary, _name.c_str(), PyBool_FromLong((long)_value));
	}
	
	void PythonModule::GetValue(string _name, int& _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject* aux;
		aux = PyDict_GetItemString(dictionary, _name.c_str());
		_value = PyLong_AsLong(aux);

	}

	void PythonModule::GetValue(string _name, float& _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject* aux;
			aux = PyDict_GetItemString(dictionary, _name.c_str());
		if (aux)
		{
			double help = PyFloat_AsDouble(aux);
			_value = (float)help;
		}
	}

	void PythonModule::GetValue(string _name, string& _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject* aux;
		aux = PyDict_GetItemString(dictionary, _name.c_str());
		_value = PyUnicode_AsUTF8(aux);
	}

	void PythonModule::GetValue(string _name, bool& _value)
	{
		// TODO-STUDENT - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject* aux;
		aux = PyDict_GetItemString(dictionary, _name.c_str());
		int val = PyObject_IsTrue(aux);

		if (val == 0)
			_value = false;
		else
			_value = true;
	}
}
