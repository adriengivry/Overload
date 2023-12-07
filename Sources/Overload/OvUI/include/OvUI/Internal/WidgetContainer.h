/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <memory>
#include <vector>

#include "OvUI/Widgets/AWidget.h"
#include "OvUI/Internal/EMemoryMode.h"

namespace OvUI::Internal
{
	/**
	* The base class for any widget container
	*/
	class WidgetContainer
	{
	public:
		/**
		* Constructor
		*/
		WidgetContainer() = default;

		/**
		* Destructor. Handle the memory de-allocation of every widgets that are internally managed
		*/
		virtual ~WidgetContainer();

		/**
		* Remove a widget from the container
		* @param p_widget
		*/
		void RemoveWidget(Widgets::AWidget& p_widget);

		/**
		* Remove all widgets from the container
		*/
		void RemoveAllWidgets();

		void TransferOwnership(Widgets::AWidget& p_widget, WidgetContainer& p_widgetCoontainer);

		/**
		* Collect garbages by removing widgets marked as "Destroyed"
		*/
		void CollectGarbages();

		/**
		* Draw every widgets
		*/
		void DrawWidgets();

        /**
        * Allow the user to reverse the draw order of this widget container
        */
        void ReverseDrawOrder(bool reversed = true);

		/**
		* Create a widget
		* @param p_args
		*/
		template <typename T, typename ... Args>
		T& CreateWidget(Args&&... p_args)
		{
			m_widgets.emplace_back(std::make_unique<T>(p_args...));
			T& instance = *static_cast<T*>(m_widgets.back().get());
			instance.SetParent(this);
			return instance;
		}

		/**
		* Returns the widgets and their memory management mode
		*/
		std::vector<std::unique_ptr<Widgets::AWidget>>& GetWidgets();

	protected:
		std::vector<std::unique_ptr<Widgets::AWidget>> m_widgets;
        bool m_reversedDrawOrder = false;
	};
}