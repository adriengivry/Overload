/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <memory>
#include <vector>

#include "OvUI/Widgets/AWidget.h"

namespace OvUI::Internal
{
	/**
	* The base class for any widget container
	*/
	class WidgetContainer
	{
	public:
		/**
		* Remove a widget from the container
		* @param p_widget
		*/
		void RemoveWidget(Widgets::AWidget& p_widget);

		/**
		* Remove all widgets from the container
		*/
		void RemoveAllWidgets();

		/**
		* Consider a widget
		*/
		void ConsiderWidget(Widgets::AWidget& p_widget);

		/**
		* Unconsider a widget
		* @param p_widget
		*/
		void UnconsiderWidget(Widgets::AWidget& p_widget);

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
			m_widgets.emplace_back(std::make_shared<T>(p_args...));
			T& instance = *std::dynamic_pointer_cast<T>(m_widgets.back());
			instance.SetParent(this);
			return instance;
		}

		/**
		* Returns the widgets
		*/
		std::vector<std::shared_ptr<Widgets::AWidget>>& GetWidgets();

	protected:
		std::vector<std::shared_ptr<Widgets::AWidget>> m_widgets;
		bool m_reversedDrawOrder = false;
	};
}