#pragma once

#include "ProfilerCallback.h"
#include "PerformanceCounter.hpp"
#include <string>
#include <map>

#define NAME_BUFFER_SIZE 1024

using namespace std;

class MethodInfo {
public:
	MethodInfo(FunctionID id, string name) {
		_id = id;
		_name = name;
	}

	FunctionID getID() { return _id; }
	string getName() { return _name; }

	double latency = 0;

private:
	FunctionID _id;
	string _name = "";
};

class MethodList {
public:
    static MethodList& getIncetance() {
        static MethodList instance;
        return instance;
    }

	/**
	* MethodList ��ü�� �ʱ�ȭ �մϴ�.
	* @param corProfilerInfo �������Ϸ� �������̽��� ���� �ּ�
	*/
    void Initialize(ICorProfilerInfo* corProfilerInfo) {
        _pICorProfilerInfo = corProfilerInfo;
    }

	/**
	* �Լ��� ���� ���·� �����ϰ� �ִ�.
	* @param id ������� ���� �Լ��� ID
	* @return �޼ҵ� ������ ��� ��ü
	*/
	MethodInfo* enter(FunctionID id) {
		PerformanceCounter::getIncetance().enter();
		return getMethodInfo(id);
	}

	/**
	* �Լ��� ���� �߿� �ٸ� �Լ��� �����ϰ� �ִ�.
	* @param id ������� ���� �Լ��� ID
	* @return �޼ҵ� ������ ��� ��ü
	*/
	MethodInfo* leave(FunctionID id) {
		MethodInfo* methodInfo = getMethodInfo(id);
		methodInfo->latency = PerformanceCounter::getIncetance().leave();
		return methodInfo;
	}

	/**
	* �Լ��� ���� ���·� �����ϰ� �ִ�.
	* @param id ������� ���� �Լ��� ID
	* @return �޼ҵ� ������ ��� ��ü
	*/
	MethodInfo* tailCall(FunctionID id) {
		MethodInfo* methodInfo = getMethodInfo(id);
		methodInfo->latency = PerformanceCounter::getIncetance().leave();
		return methodInfo;
	}

	/**
	* FunctionID�� �̿��ؼ� �޼ҵ� ���� ��ü�� �����´�.
	* @param id ã���� �ϴ� �Լ��� ID
	* @return �޼ҵ� ������ ��� ��ü (������ nullptr)
	*/
	MethodInfo* getMethodInfo(FunctionID id) {
		auto item = _methods.find(id);
		if (item != _methods.end()) {
			return item->second;
		}

		MethodInfo* methodInfo = new MethodInfo(id, _getMethodName(id));
		_methods.insert(pair<FunctionID, MethodInfo*>(id, methodInfo));
		return methodInfo;
	}

private:
	MethodList() {}
    MethodList(const MethodList& ref) {}
    MethodList& operator=(const MethodList& ref) {}
    ~MethodList() {}

    ICorProfilerInfo* _pICorProfilerInfo = nullptr;
	map<FunctionID, MethodInfo*> _methods;

	string _getMethodName(FunctionID id) {
		IMetaDataImport* pIMetaDataImport = 0;
		mdToken funcToken = 0;
		WCHAR szFunction[NAME_BUFFER_SIZE];
		WCHAR szClass[NAME_BUFFER_SIZE];

		HRESULT hr = _pICorProfilerInfo->GetTokenAndMetaDataFromFunction(id, IID_IMetaDataImport, (LPUNKNOWN*)&pIMetaDataImport, &funcToken);
		if (FAILED(hr)) return "";

		mdTypeDef classTypeDef;
		ULONG cchFunction;
		ULONG cchClass;

		hr = pIMetaDataImport->GetMethodProps(funcToken, &classTypeDef, szFunction, NAME_BUFFER_SIZE, &cchFunction, 0, 0, 0, 0, 0);
		if (FAILED(hr)) return "";

		hr = pIMetaDataImport->GetTypeDefProps(classTypeDef, szClass, NAME_BUFFER_SIZE, &cchClass, 0, 0);
		if (FAILED(hr)) return "";

		WCHAR szMethodName[NAME_BUFFER_SIZE];
		_snwprintf_s(szMethodName, NAME_BUFFER_SIZE, NAME_BUFFER_SIZE, L"%s.%s", szClass, szFunction);

		// TODO: try finally ó�� (BOOST)
		pIMetaDataImport->Release();

		wstring ws(szMethodName);
		return string(ws.begin(), ws.end());
	}
};