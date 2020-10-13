/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvCore/API/Export.h"

#define OVSERVICE(Type) OvCore::Global::ServiceLocator::Get<Type>()

namespace OvCore::Global
{
	namespace Details
	{
		/**
		 * Helper for service locating provides:
		 *     1. Real O(1) access time
		 *     2. Type-safe
		 *     3. No RTTI required for service identification
		 *     4. No key-value container required for service storage
		 *     5. No type casting required for service retrieving
		 */
		template<typename T>
		class Locator
		{
		public:
			/**
			 * Set new instance of service
			 * @param p_service
			 */
			static void Set(T& p_service);

			/**
			 * Returns a provided service
			 */
			static T& Get();

		private:
			static inline T* _SERVICE = nullptr;
		};
	}

	/**
	* Provide a way to access to core services
	*/
	class API_OVCORE ServiceLocator
	{
	public:
		/**
		* Register a service in the service locator
		* @param p_service
		*/
		template<typename T>
		static void Provide(T& p_service);
		
		/**
		* Returns a service of the given type (Make sure that your provided the service before calling this method)
		*/
		template<typename T>
		static T& Get();
	};
}

#include <OvCore/Global/ServiceLocator.inl>