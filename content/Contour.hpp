#pragma once
#include <cinttypes>
#include <vector>
#include <optional>

#include <glm/vec2.hpp>
#include "../geometry/Rect.hpp"
#include "../geometry/Transform2d.hpp"

#include "Segment.hpp"

namespace pf {
	enum class ArcDirection {
		/// Clockwise, starting from the +x axis.
		CW,
		/// Counterclockwise, starting from the +x axis.
		CCW,
	};

	struct Contour {
		struct Point {
			glm::vec2 point;
			int kind;
		};

		using container_t = std::vector<Point>;

		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;
		using reverse_iterator = container_t::reverse_iterator;
		using const_reverse_iterator = container_t::const_reverse_iterator;

		static Contour withCapacity(std::size_t cap);
		static Contour fromRect(const RectF& rect);
		static Contour fromRectRounded(const RectF& rect, glm::vec2 radius);

		Contour(const Contour&) = default;
		Contour(Contour&&) noexcept = default;
		Contour& operator=(const Contour&) = default;
		Contour& operator=(Contour&&) noexcept = default;
		~Contour() = default;

		Contour();

		void clear() noexcept;
		bool empty() const noexcept;
		bool isClosed() const noexcept;
		std::size_t size() const noexcept;

		void close();

		Point& front();
		const Point& front() const;

		Point& back();
		const Point& back() const;

		Point& operator[](std::size_t index);
		const Point& operator[](std::size_t index) const;

		void pushPoint(const glm::vec2& p, int kind, bool updateBounds);
		void pushSegment(const Segment& seg, bool updateBounds);
		void pushEllipse(const Transform2F& form);
		void pushArc(const Transform2F& form, float s, float e, ArcDirection dir);
		void pushArcFromUnitChord(const Transform2F& form, LineSegment2F chord, ArcDirection dir);

		void pushSVGArc(const glm::vec2& radius, float x_rot, bool large_arc, ArcDirection dir, const glm::vec2& to);

		void pushQuadratic(const glm::vec2& p0, const glm::vec2& p1);
		void pushCubic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);
		void pushEndpoint(const glm::vec2& p);

		void transform(const Transform2F& form);
		Contour transformed(const Transform2F& form) const;

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

		std::vector<Point> points;
		RectF bounds;
		bool closed;
	};
};