#include "vxpch.h"
#include "ConsolePanel.h"

#include "Vortex/Core/Application.h"
#include "Vortex/Core/Log.h"

#include "Vortex/Scene/Scene.h"

#include "Vortex/Editor/UI/UI.h"

#include "Vortex/Input/Input.h"

namespace Vortex {

	static ConsolePanel* s_Instance = nullptr;

	static const Math::vec4 s_InfoTint = Math::vec4(0.0f, 0.431372549f, 1.0f, 1.0f);
	static const Math::vec4 s_WarningTint = Math::vec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
	static const Math::vec4 s_ErrorTint = Math::vec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);

	ConsolePanel::ConsolePanel()
	{
		VX_CORE_ASSERT(s_Instance == nullptr, "Console Instance was already created!");
		s_Instance = this;

		m_MessageBuffer.reserve(1000);
	}

	ConsolePanel::~ConsolePanel()
	{
		s_Instance = nullptr;
	}

	void ConsolePanel::OnGuiRender()
	{
		if (!IsOpen)
			return;

		Gui::Begin(m_PanelName.c_str(), &IsOpen);

		const ImVec2 contentRegionAvail = Gui::GetContentRegionAvail();
		Math::vec2 consoleSize = { contentRegionAvail.x, contentRegionAvail.y };
		consoleSize.y -= 32.0f;

		RenderMenu({ consoleSize.x, 28.0f });
		RenderConsole(consoleSize);

		Gui::End();
	}

	void ConsolePanel::ClearMessages()
	{
		std::scoped_lock<std::mutex> lock(m_MessageBufferMutex);
		m_MessageBuffer.clear();
	}

	void ConsolePanel::RenderMenu(const Math::vec2& size)
	{
		UI::ScopedStyleStack frame(ImGuiStyleVar_FrameBorderSize, 0.0f, ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		Gui::BeginChild("Toolbar", { size.x, size.y });

		if (Gui::Button("Clear", { 75.0f, 28.0f }))
			ClearMessages();

		Gui::SameLine();

		const auto& style = Gui::GetStyle();
		const std::string clearOnPlayText = fmt::format("{} Clear on Play", m_ClearOnPlay ? (const char*)VX_ICON_CHECK : (const char*)VX_ICON_TIMES);
		ImVec4 textColor = m_ClearOnPlay ? style.Colors[ImGuiCol_Text] : style.Colors[ImGuiCol_TextDisabled];
		auto color = GetToolbarButtonColor(m_ClearOnPlay);
		if (UI::ColoredButton(clearOnPlayText.c_str(), *(ImVec4*)&color, textColor, ImVec2(110.0f, 28.0f)))
			m_ClearOnPlay = !m_ClearOnPlay;

		{
			const ImVec2 buttonSize(28.0f, 28.0f);

			Gui::SameLine(Gui::GetContentRegionAvail().x - 100.0f, 0.0f);
			textColor = (m_MessageFilters & (int16_t)ConsoleMessageFlags::Info) ? *(ImVec4*)&s_InfoTint : style.Colors[ImGuiCol_TextDisabled];
			color = GetToolbarButtonColor(m_MessageFilters & (int16_t)ConsoleMessageFlags::Info);
			if (UI::ColoredButton((const char*)VX_ICON_INFO_CIRCLE, *(ImVec4*)&color, textColor, buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessageFlags::Info;

			Gui::SameLine();
			textColor = (m_MessageFilters & (int16_t)ConsoleMessageFlags::Warning) ? *(ImVec4*)&s_WarningTint : style.Colors[ImGuiCol_TextDisabled];
			color = GetToolbarButtonColor(m_MessageFilters & (int16_t)ConsoleMessageFlags::Warning);
			if (UI::ColoredButton((const char*)VX_ICON_EXCLAMATION_TRIANGLE, *(ImVec4*)&color, textColor, buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessageFlags::Warning;

			Gui::SameLine();
			textColor = (m_MessageFilters & (int16_t)ConsoleMessageFlags::Error) ? *(ImVec4*)&s_ErrorTint : style.Colors[ImGuiCol_TextDisabled];
			color = GetToolbarButtonColor(m_MessageFilters & (int16_t)ConsoleMessageFlags::Error);
			if (UI::ColoredButton((const char*)VX_ICON_EXCLAMATION_CIRCLE, *(ImVec4*)&color, textColor, buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessageFlags::Error;
		}

		Gui::EndChild();
	}

	void ConsolePanel::RenderConsole(const Math::vec2& size)
	{
		static const char* s_Columns[] = { "Type", "Time", "Message" };

		UI::Table("Console##internal", s_Columns, 3, *(ImVec2*)&size, [&]()
		{
			std::scoped_lock<std::mutex> lock(m_MessageBufferMutex);

			float scrollY = Gui::GetScrollY();
			if (scrollY < m_PreviousScrollY)
				m_EnableScrollToLatest = false;

			if (scrollY >= Gui::GetScrollMaxY())
				m_EnableScrollToLatest = true;

			m_PreviousScrollY = scrollY;

			float rowHeight = 28.0f;
			for (uint32_t i = 0; i < m_MessageBuffer.size(); i++)
			{
				const ConsoleMessage& msg = m_MessageBuffer[i];

				if (!(m_MessageFilters & (int16_t)msg.Flags))
					continue;

				Gui::PushID(&msg);

				const bool clicked = UI::TableRowClickable(msg.ShortMessage.c_str(), rowHeight) || (Gui::IsItemHovered() && Input::IsMouseButtonDown(MouseButton::Left));
				const Math::vec4& color = GetMessageColor(msg);

				if (m_ContextScene->IsRunning())
				{
					std::string icon;
					if (msg.Flags & (int16_t)ConsoleMessageFlags::None)
						icon = (const char*)VX_ICON_COMMENT;
					if (msg.Flags & (int16_t)ConsoleMessageFlags::Info)
						icon = (const char*)VX_ICON_INFO;
					else if (msg.Flags & (int16_t)ConsoleMessageFlags::Warning)
						icon = (const char*)VX_ICON_EXCLAMATION_TRIANGLE;
					else if (msg.Flags & (int16_t)ConsoleMessageFlags::Error)
						icon = (const char*)VX_ICON_EXCLAMATION_CIRCLE;

					UI::ShiftCursorX(16.0f);
					Gui::TextColored(*(ImVec4*)&color, icon.c_str());
				}
				else
				{
					UI::Draw::Separator(ImVec2(4.0f, rowHeight), *(ImVec4*)&color);
					Gui::SameLine();
					Gui::Text(GetMessageType(msg));
				}

				Gui::TableNextColumn();
				UI::ShiftCursorX(4.0f);

				std::stringstream timeString;
				tm timeBuffer;
				localtime_s(&timeBuffer, &msg.Time);
				timeString << std::put_time(&timeBuffer, "%I:%M:%S %p");

				std::string&& timeFmt = timeString.str();
				if (size_t pos = timeFmt.find("AM"); pos != std::string::npos)
					timeFmt.replace(timeFmt.begin() + pos, timeFmt.end(), "am");
				else if (size_t pos = timeFmt.find("PM"); pos != std::string::npos)
					timeFmt.replace(timeFmt.begin() + pos, timeFmt.end(), "pm");

				Gui::Text(timeFmt.c_str());

				Gui::TableNextColumn();
				UI::ShiftCursorX(4.0f);
				Gui::TextWrapped(msg.ShortMessage.c_str());

				if (i == m_MessageBuffer.size() - 1 && m_ScrollToLatest)
				{
					Gui::ScrollToItem();
					m_ScrollToLatest = false;
				}

				if (clicked)
				{
					Gui::OpenPopup("Detailed Message");
					const Application& app = Application::Get();
					const Window& window = app.GetWindow();
					const Math::vec2& size = window.GetSize();
					const Math::vec2& position = window.GetPosition();
					ImGui::SetNextWindowSize({ (float)size.x * 0.5f, (float)size.y * 0.5f });
					ImGui::SetNextWindowPos({ position.x + (float)size.x / 2.0f, position.y + (float)size.y / 2.5f }, 0, { 0.5f, 0.5f });
					m_DetailedPanelOpen = true;
				}

				if (m_DetailedPanelOpen)
				{
					UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
					UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 8.0f));

					if (Gui::BeginPopupModal("Detailed Message", &m_DetailedPanelOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
					{
						ImGui::TextWrapped(msg.LongMessage.c_str());
						if (ImGui::Button("Copy To Clipboard", ImVec2(120.0f, 28.0f)))
						{
							ImGui::SetClipboardText(msg.LongMessage.c_str());
						}
						ImGui::EndPopup();
					}
				}

				Gui::PopID();
			}
		});
	}

	const char* ConsolePanel::GetMessageType(const ConsoleMessage& message) const
	{
		if (message.Flags & (int16_t)ConsoleMessageFlags::Info)    return "Info";
		if (message.Flags & (int16_t)ConsoleMessageFlags::Warning) return "Warning";
		if (message.Flags & (int16_t)ConsoleMessageFlags::Error)   return "Error";

		return "Unknown";
	}

	const Math::vec4& ConsolePanel::GetMessageColor(const ConsoleMessage& message) const
	{
		if (message.Flags & (int16_t)ConsoleMessageFlags::Info)    return s_InfoTint;
		if (message.Flags & (int16_t)ConsoleMessageFlags::Warning) return s_WarningTint;
		if (message.Flags & (int16_t)ConsoleMessageFlags::Error)   return s_ErrorTint;

		return s_InfoTint;
	}

	Math::vec4 ConsolePanel::GetToolbarButtonColor(const bool value) const
	{
		const auto& style = Gui::GetStyle();
		return *(Math::vec4*)(value ? &style.Colors[ImGuiCol_Header] : &style.Colors[ImGuiCol_FrameBg]);
	}

	void ConsolePanel::PushMessage(const ConsoleMessage& message)
	{
		if (s_Instance == nullptr)
			return;

		{
			std::scoped_lock<std::mutex> lock(s_Instance->m_MessageBufferMutex);
			s_Instance->m_MessageBuffer.push_back(message);
		}

		if (s_Instance->m_EnableScrollToLatest)
			s_Instance->m_ScrollToLatest = true;
	}

}
