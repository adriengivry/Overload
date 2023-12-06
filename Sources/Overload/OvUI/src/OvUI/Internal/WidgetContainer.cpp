/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <algorithm>

#include "OvUI/Internal/WidgetContainer.h"

void OvUI::Internal::WidgetContainer::RemoveWidget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::shared_ptr<Widgets::AWidget> p_instance)
	{ 
		return p_instance.get() == &p_widget;
	});

	if (found != m_widgets.end())
	{
		m_widgets.erase(found);
	}
}

void OvUI::Internal::WidgetContainer::RemoveAllWidgets()
{
	m_widgets.clear();
}

void OvUI::Internal::WidgetContainer::ConsiderWidget(Widgets::AWidget& p_widget)
{
	m_widgets.emplace_back(&p_widget);
	p_widget.SetParent(this);
}

void OvUI::Internal::WidgetContainer::UnconsiderWidget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::shared_ptr<Widgets::AWidget> p_instance)
	{
		return p_instance.get() == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(nullptr);
		m_widgets.erase(found);
	}
}

void OvUI::Internal::WidgetContainer::CollectGarbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::shared_ptr<Widgets::AWidget> p_instance)
	{
		return p_instance && p_instance->IsDestroyed();
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
			widget->Draw();
	}
}

void OvUI::Internal::WidgetContainer::ReverseDrawOrder(const bool reversed)
{
	m_reversedDrawOrder = reversed;
}

std::vector<std::shared_ptr<OvUI::Widgets::AWidget>>& OvUI::Internal::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
