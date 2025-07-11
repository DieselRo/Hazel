#pragma once

#include "Hazel/Core.h"
#include <string>
#include <functional>

// -------------- fmt include ------------------
// Because spdlog vendors fmt, you may use:
#include <spdlog/fmt/bundled/format.h>

// or, if you’ve installed fmt separately:
// #include <fmt/format.h>
#include <ostream>

// -------------- Hazel event system -----------

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};
	

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
/*
// ─── generic formatter for any class derived from Hazel::Event ───
#include <type_traits>
#include <string_view>
#include <spdlog/fmt/bundled/format.h>   // or <fmt/format.h>

namespace fmt {

	template <typename T, typename Char>
	struct formatter<
		T,
		Char,
		std::enable_if_t<std::is_base_of_v<Hazel::Event, T>, void>>        // enable only for Event-derived
		: formatter<std::basic_string_view<Char>, Char>                    // note the 2-parameter base!
	{
		template <typename FormatContext>
		auto format(const T& e, FormatContext& ctx) const
		{
			auto sv = std::basic_string_view<Char>(e.ToString());
			return formatter<std::basic_string_view<Char>, Char>::format(sv, ctx);
		}
	};

} // namespace fmt
*/

// ── generic formatter for any class derived from Hazel::Event ────────────
#include <type_traits>
#include <string_view>

namespace fmt {

	// primary template (in fmt) is:  formatter<T, Char = char, Enable = void>
	// we specialise **all three parameters** and use SFINAE on Enable
	template <typename T, typename Char>
	struct formatter<
		T,
		Char,
		std::enable_if_t<std::is_base_of_v<Hazel::Event, T>, void>>
		: formatter<std::basic_string_view<Char>, Char>
	{
		// inherit parse()

		template <typename FormatContext>
		auto format(const T& e, FormatContext& ctx) const
		{
			return formatter<std::basic_string_view<Char>, Char>::format(
				std::basic_string_view<Char>(e.ToString()), ctx);
		}
	};

} // namespace fmt
