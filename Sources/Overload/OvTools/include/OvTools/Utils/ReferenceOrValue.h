/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <variant>

namespace OvTools::Utils
{
	/**
	* A simple class that can represent a reference or a value of the given type.
	* The usage of the data is the same for both usages.
	* The goal is to be able to instanciate objects that can be a reference or a value without
	* taking care of what it is.
	*/
	template <typename T>
	class ReferenceOrValue
	{
	public:
		/**
		* Construct the ReferenceOrValue instance with a reference
		* @param p_reference
		*/
		ReferenceOrValue(std::reference_wrapper<T> p_reference) : m_data{ &p_reference.get() }
		{
		}

		/**
		* Construct the ReferenceOrValue instance with a value
		* @param p_value
		*/
		ReferenceOrValue(T p_value = T()) : m_data{ p_value }
		{
		}

		/**
		* Make the ReferenceOrValue a reference
		* @param p_reference
		*/
		void MakeReference(T& p_reference)
		{
			m_data = &p_reference;
		}

		/**
		* Make the ReferenceOrValue a value
		* @param p_value
		*/
		void MakeValue(T p_value = T())
		{
			m_data = p_value;
		}

		/**
		* Implicit conversion of a ReferenceOrValue to a T
		*/
		operator T&()
		{
			return Get();
		}

		/**
		* Assignment operator thats call the setter of the ReferenceOrValue instance
		* @param p_value
		*/
		ReferenceOrValue<T>& operator=(T p_value)
		{
			Set(p_value);
			return *this;
		}

		/**
		* Returns the value (From reference or directly from the value)
		*/
		T& Get() const
		{
			if (auto pval = std::get_if<T>(&m_data))
				return *pval;
			else
				return *std::get<T*>(m_data);
		}

		/**
		* Sets the value (To the reference or directly to the value)
		* @param p_value
		*/
		void Set(T p_value)
		{
			if (auto pval = std::get_if<T>(&m_data))
				* pval = p_value;
			else
				*std::get<T*>(m_data) = p_value;
		}

	private:
		std::variant<T, T*> m_data;
	};
}