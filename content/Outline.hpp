#pragma once
#include <cinttypes>
#include <vector>
#include <optional>
#include <glm/vec2.hpp>

#include "../geometry/Rect.hpp"
#include "../geometry/Transform2d.hpp"

#include "Contour.hpp"

namespace pf {

	struct Outline {
		using container_t = std::vector<Contour>;
		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;
		using reverse_iterator = container_t::reverse_iterator;
		using const_reverse_iterator = container_t::const_reverse_iterator;

		static Outline withCapacity(std::size_t cap);
		static Outline fromRect(const RectF& rect);
		static Outline fromRectRounded(const RectF& rect, const glm::vec2& radius);

		Outline(const Outline&) = default;
		Outline(Outline&&) noexcept = default;
		~Outline() = default;
		Outline& operator=(const Outline&) = default;
		Outline& operator=(Outline&&) noexcept = default;

		Outline();

		void clear();

		void closeAllContours() noexcept;

		// number of contours in the outline.
		std::size_t size() const noexcept;
		bool empty() const noexcept;

		void push(const Outline& outline);
		void push(const Contour& contour);
		void push(Outline&& outline);
		void push(Contour&& contour);
		Contour pop();

		void transform(const Transform2F& form);
		Outline transformed(const Transform2F& form) const;

		void dilate(float amount);
		void dilate(const glm::vec2& amount);

		void recalculateBounds() noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;

		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator rend() const noexcept;

		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		std::vector<Contour> contours;
		RectF bounds;
	};
};

/*
pf::PointFlags operator|(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator^(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator&(pf::PointFlags lh, pf::PointFlags rh) noexcept;
pf::PointFlags operator~(pf::PointFlags lh) noexcept;
*/