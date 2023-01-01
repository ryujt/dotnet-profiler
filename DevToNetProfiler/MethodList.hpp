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
	* MethodList 객체를 초기화 합니다.
	* @param corProfilerInfo 프로파일러 인터페이스의 참조 주소
	*/
    void Initialize(ICorProfilerInfo* corProfilerInfo) {
        _pICorProfilerInfo = corProfilerInfo;
    }

	/**
	* 함수가 실행 상태로 진입하고 있다.
	* @param id 실행상태 중인 함수의 ID
	* @return 메소드 정보가 담긴 객체
	*/
	MethodInfo* enter(FunctionID id) {
		PerformanceCounter::getIncetance().enter();
		return getMethodInfo(id);
	}

	/**
	* 함수가 종료 중에 다른 함수를 실행하고 있다.
	* @param id 실행상태 중인 함수의 ID
	* @return 메소드 정보가 담긴 객체
	*/
	MethodInfo* leave(FunctionID id) {
		MethodInfo* methodInfo = getMethodInfo(id);
		methodInfo->latency = PerformanceCounter::getIncetance().leave();
		return methodInfo;
	}

	/**
	* 함수가 실행 상태로 진입하고 있다.
	* @param id 실행상태 중인 함수의 ID
	* @return 메소드 정보가 담긴 객체
	*/
	MethodInfo* tailCall(FunctionID id) {
		MethodInfo* methodInfo = getMethodInfo(id);
		methodInfo->latency = PerformanceCounter::getIncetance().leave();
		return methodInfo;
	}

	/**
	* FunctionID를 이용해서 메소드 정보 객체를 가져온다.
	* @param id 찾고자 하는 함수의 ID
	* @return 메소드 정보가 담긴 객체 (없으면 nullptr)
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

		// TODO: try finally 처리 (BOOST)
		pIMetaDataImport->Release();

		wstring ws(szMethodName);
		return string(ws.begin(), ws.end());
	}
};