#pragma once

#include "OvDebug/Utils/Assertion.h"

namespace OvCore::Global {
	template<typename T>
	void Details::Locator<T>::Set(T& p_service) 
	{
		OVASSERT(!_SERVICE, "Service is already provided");
		_SERVICE = &p_service;
	}

	template<typename T>
	T& Details::Locator<T>::Get()
	{
		OVASSERT(_SERVICE, "Service is not provided");
		return *_SERVICE;
	}

	template<typename T>
	void ServiceLocator::Provide(T& p_service)
	{
		Details::Locator<T>::Set(p_service);
	}

	template<typename T>
	T& ServiceLocator::Get()
	{
		return Details::Locator<T>::Get();
	}
}
