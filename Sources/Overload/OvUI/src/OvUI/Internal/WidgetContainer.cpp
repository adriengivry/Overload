/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "OvUI/Internal/WidgetContainer.h"

OvUI::Internal::WidgetContainer::~WidgetContainer()
{
	RemoveAllWidgets();
}

void OvUI::Internal::WidgetContainer::RemoveWidget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
		{
			return p_instance.get() == &p_widget;
		});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(nullptr);

		m_widgets.erase(found);
	}
}

void OvUI::Internal::WidgetContainer::RemoveAllWidgets()
{
	m_widgets.clear();
}

void OvUI::Internal::WidgetContainer::TransferOwnership(Widgets::AWidget& p_widget, WidgetContainer& p_widgetCoontainer)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
		{
			return p_instance.get() == &p_widget;
		});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(&p_widgetCoontainer);

		p_widgetCoontainer.m_widgets.push_back(std::move(*found));

		m_widgets.erase(found);
	}
}

void OvUI::Internal::WidgetContainer::CollectGarbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::unique_ptr<OvUI::Widgets::AWidget>& p_item)
	{
		bool toDestroy = p_item && p_item.get()->IsDestroyed();

		return toDestroy;
	}), m_widgets.end());
}

void OvUI::Internal::WidgetContainer::DrawWidgets()
{
	CollectGarbages();

	if (m_reversedDrawOrder)
	{
		for (auto it = m_widgets.crbegin(); it != m_widgets.crend(); ++it)
			it->get()->Draw();
	}
	else
	{ 
		for (const auto& widget : m_widgets)
		{
			widget->Draw();
		}
	}
}

void OvUI::Internal::WidgetContainer::ReverseDrawOrder(const bool reversed)
{
    m_reversedDrawOrder = reversed;
}

std::vector<std::unique_ptr<OvUI::Widgets::AWidget>>& OvUI::Internal::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
