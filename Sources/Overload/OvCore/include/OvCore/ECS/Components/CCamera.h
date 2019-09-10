/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvRendering/LowRenderer/Camera.h"
#include "AComponent.h"

namespace OvCore::ECS { class Actor; }

namespace OvCore::ECS::Components
{
	/**
	* Represents a camera entity. Its position will determine its view matrix
	*/
	class API_OVCORE CCamera : public AComponent
	{
	public:
		/**
		* Constructor
		* @param p_owner
		*/
		CCamera(ECS::Actor& p_owner);

		/**
		* Destructor
		*/
		~CCamera();

		/**
		* Returns the name of the component
		*/
		std::string GetName() override;

		/**
		* Sets the fov of the camera to the given value
		* @param p_value
		*/
		void SetFov(float p_value);

		/**
		* Sets the near of the camera to the given value
		* @param p_value
		*/
		void SetNear(float p_value);

		/**
		* Sets the far of the camera to the given value
		* @param p_value
		*/
		void SetFar(float p_value);

		/**
		* Sets the clear color of the camera to the given value
		* @param p_value
		*/
		void SetClearColor(const OvMaths::FVector3& p_clearColor);

		/**
		* Returns the fov of the camera
		*/
		float GetFov() const;

		/**
		* Returns the near of the camera
		*/
		float GetNear() const;

		/**
		* Returns the far of the camera
		*/
		float GetFar() const;

		/**
		* Returns the clear color of the camera
		*/
		const OvMaths::FVector3& GetClearColor() const;

		/**
		* Returns the OvRendering camera instance attached to this component
		*/
		OvRendering::LowRenderer::Camera& GetCamera();

		/**
		* Serialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Deserialize the component
		* @param p_doc
		* @param p_node
		*/
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		/**
		* Defines how the component should be drawn in the inspector
		* @param p_root
		*/
		virtual void OnInspector(OvUI::Internal::WidgetContainer& p_root) override;

	private:
		OvRendering::LowRenderer::Camera m_camera;
		OvTools::Eventing::ListenerID m_transformNotificationHandlerID;
	};
}